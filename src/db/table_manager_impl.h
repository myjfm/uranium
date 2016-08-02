// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_TABLE_MANAGER_IMPL_H_
#define URANIUM_DB_TABLE_MANAGER_IMPL_H_

#include "table_manager.h"

#include <unistd.h>

#include <atomic>
#include <mutex>

#include "common/status.h"
#include "db_path_info.h"
#include "meta_table.h"
#include "network/cpp/uranium_internal.pb.h"

namespace uranium {

// This class saves all tables both schema type and schemaless type
// You should always get the specified table pointer through it
// NOTE:
// The schema tables and schemaless tables are in different namespaces,
// so even if their names are the same, they also represents different tables.
class TableManagerImpl : public TableManager {
 public:
  TableManagerImpl() {
    schemaless_tables_.store(new SchemalessTableMapType,
                             std::memory_order_relaxed);
    schema_tables_.store(new SchemaTableMapType, std::memory_order_relaxed);
  }

  virtual ~TableManagerImpl() {
    auto schemaless_tables = schemaless_tables_.load(std::memory_order_relaxed);
    if (schemaless_tables) {
      delete schemaless_tables;
    }
    auto schema_tables = schema_tables_.load(std::memory_order_relaxed);
    if (schema_tables) {
      delete schema_tables;
    }

    meta_table_.Close();
  }

  TableManagerImpl(const TableManagerImpl&) = delete;
  TableManagerImpl& operator=(const TableManagerImpl&) = delete;

  // Not thread safe, should only call once
  virtual Status Init(const std::string& db_paths) override {
    auto s = db_path_info_.Init(db_paths);
    if (!s.ok()) {
      return s;
    }
    // meta table is always located in the first db_path
    s = meta_table_.Init(db_path_info_.GetDbPath(0));
    if (!s.ok()) {
      return s;
    }
    std::vector<internal::TableOptions> table_options_list;
    s = meta_table_.LoadAllTables(&table_options_list);
    if (!s.ok()) {
      return s;
    }
    for (auto& table_option : table_options_list) {
      if (table_option.options().has_schema_table_options()) {
        auto stable = std::shared_ptr<SchemaTable>(new SchemaTable());
        s = stable->Init(table_option);
        if (!s.ok()) {
          return s;
        }
        auto stables = schema_tables_.load(std::memory_order_relaxed);
        (*stables)[table_option.options().table_name().name()] = stable;
        if (db_path_info_.PathExists(table_option.table_path())) {
          db_path_info_.IncreasePathWeight(table_option.table_path());
        }
      } else if (table_option.options().has_schemaless_table_options()) {
        auto sltable = std::shared_ptr<SchemalessTable>(new SchemalessTable());
        s = sltable->Init(table_option);
        if (!s.ok()) {
          return s;
        }
        auto sltables = schemaless_tables_.load(std::memory_order_relaxed);
        (*sltables)[table_option.options().table_name().name()] = sltable;
        if (db_path_info_.PathExists(table_option.table_path())) {
          db_path_info_.IncreasePathWeight(table_option.table_path());
        }
      } else {
        return Status::NotSupported("unknown table type");
      }
    }
    return Status::OK();
  }

  virtual Status Close() override {
    CloseSchemalessTables();
    CloseSchemaTables();
    return Status::OK();
  }

  virtual std::shared_ptr<SchemalessTable> GetSchemalessTable(
      const std::string& name) override {
    auto tables = schemaless_tables_.load(std::memory_order_relaxed);
    assert(tables);
    auto itr = tables->find(name);
    if (itr == tables->end()) {
      return std::shared_ptr<SchemalessTable>();
    }
    return itr->second;
  }

  virtual std::shared_ptr<SchemaTable> GetSchemaTable(
      const std::string& name) override {
    auto tables = schema_tables_.load(std::memory_order_relaxed);
    assert(tables);
    auto itr = tables->find(name);
    if (itr == tables->end()) {
      return std::shared_ptr<SchemaTable>();
    }
    return itr->second;
  }

  virtual Status CreateTable(const admin::TableOptions& options) override {
    internal::TableOptions internal_table_options;
    internal_table_options.mutable_options()->CopyFrom(options);

    std::unique_lock<std::mutex> guard(table_admin_mutex_);
    std::string table_path = db_path_info_.GetLowestWeightPath();
    if (table_path.empty()) {
      return Status::IOError("no suitable table_path to create table");
    }
    internal_table_options.set_table_path(table_path);
    internal_table_options.set_status(internal::TableStatus::USING);
    if (options.has_schema_table_options()) {
      if (GetSchemaTable(options.table_name().name()).get()) {
        return Status::TableAlreadyExists("table already exists");
      }
      auto s = meta_table_.CreateTable(internal_table_options);
      if (!s.ok()) {
        return s;
      }
      db_path_info_.IncreasePathWeight(table_path);
      auto stable = std::shared_ptr<SchemaTable>(new SchemaTable());
      s = stable->Init(internal_table_options);
      if (!s.ok()) {
        return s;
      }
      auto tables = schema_tables_.load(std::memory_order_relaxed);
      auto new_tables = new SchemaTableMapType(*tables);
      (*new_tables)[options.table_name().name()] = stable;
      schema_tables_.store(new_tables, std::memory_order_relaxed);
      guard.unlock();
      // wait for enough time to make sure the reader released the table map
      usleep(1000);
      delete tables;
    } else if (options.has_schemaless_table_options()) {
      if (GetSchemalessTable(options.table_name().name()).get()) {
        return Status::TableAlreadyExists("table already exists");
      }
      auto s = meta_table_.CreateTable(internal_table_options);
      if (!s.ok()) {
        return s;
      }
      db_path_info_.IncreasePathWeight(table_path);
      auto sltable = std::shared_ptr<SchemalessTable>(new SchemalessTable());
      s = sltable->Init(internal_table_options);
      if (!s.ok()) {
        return s;
      }
      auto tables = schemaless_tables_.load(std::memory_order_relaxed);
      auto new_tables = new SchemalessTableMapType(*tables);
      (*new_tables)[options.table_name().name()] = sltable;
      schemaless_tables_.store(new_tables, std::memory_order_relaxed);
      guard.unlock();
      // wait for enough time to make sure the reader released the table map
      usleep(1000);
      delete tables;
    } else {
      return Status::InvalidArgument("unknown table type");
    }
    return Status::OK();
  }

  virtual Status UpdateTable(const admin::TableOptions& options) override {
    return Status::NotSupported("update table is not yet supported");
  }

  virtual Status DropTable(const std::string& table_name) override {
    return Status::NotSupported("drop table is not yet supported");
  }

  virtual Status GetSchemalessTableOptions(const std::string& table_name,
                                           admin::TableOptions* options) override {
    return Status::OK();
  }

  virtual Status GetSchemaTableOptions(const std::string& table_name,
                                       admin::TableOptions* options) override {
    return Status::OK();
  }

 private:
  void CloseSchemalessTables() {
    auto tables = schemaless_tables_.load(std::memory_order_relaxed);
    assert(tables);
    for (auto& table : *tables) {
      table.second->Close();
    }
  }

  void CloseSchemaTables() {
    auto tables = schema_tables_.load(std::memory_order_relaxed);
    assert(tables);
    for (auto& table : *tables) {
      table.second->Close();
    }
  }

  using SchemalessTableMapType =
    std::map<std::string, std::shared_ptr<SchemalessTable>>;
  using SchemaTableMapType =
    std::map<std::string, std::shared_ptr<SchemaTable>>;
  // Make sure lock is held when adding or removing a table
  // But no need lock held when read or write some tables
  std::mutex table_admin_mutex_;
  std::atomic<SchemalessTableMapType*> schemaless_tables_ { nullptr };
  std::atomic<SchemaTableMapType*> schema_tables_ { nullptr };

  DbPathInfo db_path_info_;

  MetaTable meta_table_;
};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_MANAGER_IMPL_H_

