// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_TABLE_MANAGER_IMPL_H_
#define URANIUM_DB_TABLE_MANAGER_IMPL_H_

#include "table_manager.h"

#include <atomic>

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
    schema_tables_.store(new SchemaTableMapType,
                         std::memory_order_relaxed);
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
      } else if (table_option.options().has_schemaless_table_options()) {
        auto sltable = std::shared_ptr<SchemalessTable>(new SchemalessTable());
        s = sltable->Init(table_option);
        if (!s.ok()) {
          return s;
        }
        auto sltables = schemaless_tables_.load(std::memory_order_relaxed);
        (*sltables)[table_option.options().table_name().name()] = sltable;
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
      return std::shared_ptr<SchemalessTable>(nullptr);
    }
    return itr->second;
  }

  virtual std::shared_ptr<SchemaTable> GetSchemaTable(
      const std::string& name) override {
    auto tables = schema_tables_.load(std::memory_order_relaxed);
    assert(tables);
    auto itr = tables->find(name);
    if (itr == tables->end()) {
      return std::shared_ptr<SchemaTable>(nullptr);
    }
    return itr->second;
  }

  virtual Status CreateTable(const admin::TableOptions& options) override {
    return Status::OK();
  }

  virtual Status UpdateTable(const admin::TableOptions& options) override {
    return Status::OK();
  }

  virtual Status DropTable(const std::string& table_name) override {
    return Status::OK();
  }

  virtual Status GetTableOptions(const std::string& table_name,
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
  // avoid lock required when adding a table or removing a table
  std::atomic<SchemalessTableMapType*> schemaless_tables_ { nullptr };
  std::atomic<SchemaTableMapType*> schema_tables_ { nullptr };

  DbPathInfo db_path_info_;

  MetaTable meta_table_;
};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_MANAGER_IMPL_H_

