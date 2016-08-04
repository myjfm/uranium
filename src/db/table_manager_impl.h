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
#include "hash_table.h"
#include "kv_table.h"
#include "list_table.h"
#include "meta_table.h"
#include "network/cpp/uranium_internal.pb.h"
#include "set_table.h"

namespace uranium {

// This class saves all tables both schema type and schemaless type
// You should always get the specified table pointer through it
// NOTE:
// different type of tables are in different namespaces,
// so even if their names are the same, they also represents different tables.
class TableManagerImpl : public TableManager {
 public:
  TableManagerImpl() {
    kv_tables_.store(new KVTableMapType, std::memory_order_relaxed);
    list_tables_.store(new ListTableMapType, std::memory_order_relaxed);
    hash_tables_.store(new HashTableMapType, std::memory_order_relaxed);
    set_tables_.store(new SetTableMapType, std::memory_order_relaxed);
    schema_tables_.store(new SchemaTableMapType, std::memory_order_relaxed);
  }

  virtual ~TableManagerImpl() {
    auto kv_tables = kv_tables_.load(std::memory_order_relaxed);
    if (kv_tables) {
      delete kv_tables;
    }
    auto list_tables = kv_tables_.load(std::memory_order_relaxed);
    if (list_tables) {
      delete list_tables;
    }
    auto hash_tables = kv_tables_.load(std::memory_order_relaxed);
    if (hash_tables) {
      delete hash_tables;
    }
    auto set_tables = kv_tables_.load(std::memory_order_relaxed);
    if (set_tables) {
      delete set_tables;
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
      if (table_option.options().table_type() == common::TableType::KV) {
        auto kv_table = std::shared_ptr<KVTable>(new KVTable());
        s = kv_table->Init(table_option);
        if (!s.ok()) {
          return s;
        }
        auto kv_tables = kv_tables_.load(std::memory_order_relaxed);
        (*kv_tables)[table_option.options().table_name().name()] = kv_table;
        if (db_path_info_.PathExists(table_option.table_path())) {
          db_path_info_.IncreasePathWeight(table_option.table_path());
        }
      } else if (table_option.options().table_type() == common::TableType::LIST) {
        auto list_table = std::shared_ptr<ListTable>(new ListTable());
        s = list_table->Init(table_option);
        if (!s.ok()) {
          return s;
        }
        auto list_tables = list_tables_.load(std::memory_order_relaxed);
        (*list_tables)[table_option.options().table_name().name()] = list_table;
        if (db_path_info_.PathExists(table_option.table_path())) {
          db_path_info_.IncreasePathWeight(table_option.table_path());
        }
      } else if (table_option.options().table_type() == common::TableType::HASH) {
        auto hash_table = std::shared_ptr<HashTable>(new HashTable());
        s = hash_table->Init(table_option);
        if (!s.ok()) {
          return s;
        }
        auto hash_tables = hash_tables_.load(std::memory_order_relaxed);
        (*hash_tables)[table_option.options().table_name().name()] = hash_table;
        if (db_path_info_.PathExists(table_option.table_path())) {
          db_path_info_.IncreasePathWeight(table_option.table_path());
        }
      } else if (table_option.options().table_type() == common::TableType::SET) {
        auto set_table = std::shared_ptr<SetTable>(new SetTable());
        s = set_table->Init(table_option);
        if (!s.ok()) {
          return s;
        }
        auto set_tables = set_tables_.load(std::memory_order_relaxed);
        (*set_tables)[table_option.options().table_name().name()] = set_table;
        if (db_path_info_.PathExists(table_option.table_path())) {
          db_path_info_.IncreasePathWeight(table_option.table_path());
        }
      } else if (table_option.options().table_type() == common::TableType::SCHEMA) {
        auto schema_table = std::shared_ptr<SchemaTable>(new SchemaTable());
        s = schema_table->Init(table_option);
        if (!s.ok()) {
          return s;
        }
        auto schema_tables = schema_tables_.load(std::memory_order_relaxed);
        (*schema_tables)[table_option.options().table_name().name()] = schema_table;
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
    CloseKVTables();
    CloseListTables();
    CloseHashTables();
    CloseSetTables();
    CloseSchemaTables();
    return Status::OK();
  }

  virtual std::shared_ptr<KVTable> GetKVTable(
      const std::string& name) override {
    auto tables = kv_tables_.load(std::memory_order_relaxed);
    assert(tables);
    auto itr = tables->find(name);
    if (itr == tables->end()) {
      return std::shared_ptr<KVTable>();
    }
    return itr->second;
  }

  virtual std::shared_ptr<ListTable> GetListTable(
      const std::string& name) override {
    auto tables = list_tables_.load(std::memory_order_relaxed);
    assert(tables);
    auto itr = tables->find(name);
    if (itr == tables->end()) {
      return std::shared_ptr<ListTable>();
    }
    return itr->second;
  }

  virtual std::shared_ptr<HashTable> GetHashTable(
      const std::string& name) override {
    auto tables = hash_tables_.load(std::memory_order_relaxed);
    assert(tables);
    auto itr = tables->find(name);
    if (itr == tables->end()) {
      return std::shared_ptr<HashTable>();
    }
    return itr->second;
  }

  virtual std::shared_ptr<SetTable> GetSetTable(
      const std::string& name) override {
    auto tables = set_tables_.load(std::memory_order_relaxed);
    assert(tables);
    auto itr = tables->find(name);
    if (itr == tables->end()) {
      return std::shared_ptr<SetTable>();
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

  virtual Status CreateKVTable(const std::string& table_name,
                               const admin::KVTableOptions& options) override {
    internal::TableOptions internal_table_options;
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::KV);
    admin_table_options.mutable_kv_table_options()->CopyFrom(options);
    internal_table_options.mutable_options()->CopyFrom(admin_table_options);

    std::unique_lock<std::mutex> guard(table_admin_mutex_);
    std::string table_path = db_path_info_.GetLowestWeightPath();
    if (table_path.empty()) {
      return Status::IOError("no suitable table_path to create table");
    }
    internal_table_options.set_table_path(table_path);
    internal_table_options.set_status(internal::TableStatus::USING);
    if (GetKVTable(table_name).get()) {
      return Status::TableAlreadyExists("table already exists");
    }
    auto s = meta_table_.CreateKVTable(table_path, table_name, options);
    if (!s.ok()) {
      return s;
    }
    db_path_info_.IncreasePathWeight(table_path);
    auto stable = std::shared_ptr<KVTable>(new KVTable());
    s = stable->Init(internal_table_options);
    if (!s.ok()) {
      return s;
    }
    auto tables = kv_tables_.load(std::memory_order_relaxed);
    auto new_tables = new KVTableMapType(*tables);
    (*new_tables)[table_name] = stable;
    kv_tables_.store(new_tables, std::memory_order_relaxed);
    guard.unlock();
    // wait for enough time to make sure the reader released the table map
    usleep(1000);
    delete tables;
  }

  virtual Status CreateListTable(
      const std::string& table_name,
      const admin::ListTableOptions& options) override {
    internal::TableOptions internal_table_options;
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::LIST);
    admin_table_options.mutable_list_table_options()->CopyFrom(options);
    internal_table_options.mutable_options()->CopyFrom(admin_table_options);

    std::unique_lock<std::mutex> guard(table_admin_mutex_);
    std::string table_path = db_path_info_.GetLowestWeightPath();
    if (table_path.empty()) {
      return Status::IOError("no suitable table_path to create table");
    }
    internal_table_options.set_table_path(table_path);
    internal_table_options.set_status(internal::TableStatus::USING);
    if (GetListTable(table_name).get()) {
      return Status::TableAlreadyExists("table already exists");
    }
    auto s = meta_table_.CreateListTable(table_path, table_name, options);
    if (!s.ok()) {
      return s;
    }
    db_path_info_.IncreasePathWeight(table_path);
    auto stable = std::shared_ptr<ListTable>(new ListTable());
    s = stable->Init(internal_table_options);
    if (!s.ok()) {
      return s;
    }
    auto tables = list_tables_.load(std::memory_order_relaxed);
    auto new_tables = new ListTableMapType(*tables);
    (*new_tables)[table_name] = stable;
    list_tables_.store(new_tables, std::memory_order_relaxed);
    guard.unlock();
    // wait for enough time to make sure the reader released the table map
    usleep(1000);
    delete tables;
  }

  virtual Status CreateHashTable(
      const std::string& table_name,
      const admin::HashTableOptions& options) override {
    internal::TableOptions internal_table_options;
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::HASH);
    admin_table_options.mutable_hash_table_options()->CopyFrom(options);
    internal_table_options.mutable_options()->CopyFrom(admin_table_options);

    std::unique_lock<std::mutex> guard(table_admin_mutex_);
    std::string table_path = db_path_info_.GetLowestWeightPath();
    if (table_path.empty()) {
      return Status::IOError("no suitable table_path to create table");
    }
    internal_table_options.set_table_path(table_path);
    internal_table_options.set_status(internal::TableStatus::USING);
    if (GetHashTable(table_name).get()) {
      return Status::TableAlreadyExists("table already exists");
    }
    auto s = meta_table_.CreateHashTable(table_path, table_name, options);
    if (!s.ok()) {
      return s;
    }
    db_path_info_.IncreasePathWeight(table_path);
    auto stable = std::shared_ptr<HashTable>(new HashTable());
    s = stable->Init(internal_table_options);
    if (!s.ok()) {
      return s;
    }
    auto tables = hash_tables_.load(std::memory_order_relaxed);
    auto new_tables = new HashTableMapType(*tables);
    (*new_tables)[table_name] = stable;
    hash_tables_.store(new_tables, std::memory_order_relaxed);
    guard.unlock();
    // wait for enough time to make sure the reader released the table map
    usleep(1000);
    delete tables;
  }

  virtual Status CreateSetTable(
      const std::string& table_name,
      const admin::SetTableOptions& options) override {
    internal::TableOptions internal_table_options;
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::SET);
    admin_table_options.mutable_set_table_options()->CopyFrom(options);
    internal_table_options.mutable_options()->CopyFrom(admin_table_options);

    std::unique_lock<std::mutex> guard(table_admin_mutex_);
    std::string table_path = db_path_info_.GetLowestWeightPath();
    if (table_path.empty()) {
      return Status::IOError("no suitable table_path to create table");
    }
    internal_table_options.set_table_path(table_path);
    internal_table_options.set_status(internal::TableStatus::USING);
    if (GetSetTable(table_name).get()) {
      return Status::TableAlreadyExists("table already exists");
    }
    auto s = meta_table_.CreateSetTable(table_path, table_name, options);
    if (!s.ok()) {
      return s;
    }
    db_path_info_.IncreasePathWeight(table_path);
    auto stable = std::shared_ptr<SetTable>(new SetTable());
    s = stable->Init(internal_table_options);
    if (!s.ok()) {
      return s;
    }
    auto tables = set_tables_.load(std::memory_order_relaxed);
    auto new_tables = new SetTableMapType(*tables);
    (*new_tables)[table_name] = stable;
    set_tables_.store(new_tables, std::memory_order_relaxed);
    guard.unlock();
    // wait for enough time to make sure the reader released the table map
    usleep(1000);
    delete tables;
  }

  virtual Status CreateSchemaTable(
      const std::string& table_name,
      const admin::SchemaTableOptions& options) override {
    internal::TableOptions internal_table_options;
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::SCHEMA);
    admin_table_options.mutable_schema_table_options()->CopyFrom(options);
    internal_table_options.mutable_options()->CopyFrom(admin_table_options);

    std::unique_lock<std::mutex> guard(table_admin_mutex_);
    std::string table_path = db_path_info_.GetLowestWeightPath();
    if (table_path.empty()) {
      return Status::IOError("no suitable table_path to create table");
    }
    internal_table_options.set_table_path(table_path);
    internal_table_options.set_status(internal::TableStatus::USING);
    if (GetSetTable(table_name).get()) {
      return Status::TableAlreadyExists("table already exists");
    }
    auto s = meta_table_.CreateSchemaTable(table_path, table_name, options);
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
    (*new_tables)[table_name] = stable;
    schema_tables_.store(new_tables, std::memory_order_relaxed);
    guard.unlock();
    // wait for enough time to make sure the reader released the table map
    usleep(1000);
    delete tables;
  }

  virtual Status UpdateKVTable(const std::string& table_name,
                               const admin::KVTableOptions& options) override {
    return Status::NotSupported("update table is not yet supported");
  }

  virtual Status UpdateListTable(
      const std::string& table_name,
      const admin::ListTableOptions& options) override {
    return Status::NotSupported("update table is not yet supported");
  }

  virtual Status UpdateHashTable(
      const std::string& table_name,
      const admin::HashTableOptions& options) override {
    return Status::NotSupported("update table is not yet supported");
  }

  virtual Status UpdateSetTable(
      const std::string& table_name,
      const admin::SetTableOptions& options) override {
    return Status::NotSupported("update table is not yet supported");
  }

  virtual Status UpdateSchemaTable(
      const std::string& table_name,
      const admin::SchemaTableOptions& options) override {
    return Status::NotSupported("update table is not yet supported");
  }

  virtual Status DropKVTable(const std::string& table_name) override {
    return Status::NotSupported("drop table is not yet supported");
  }

  virtual Status DropListTable(const std::string& table_name) override {
    return Status::NotSupported("drop table is not yet supported");
  }

  virtual Status DropHashTable(const std::string& table_name) override {
    return Status::NotSupported("drop table is not yet supported");
  }

  virtual Status DropSetTable(const std::string& table_name) override {
    return Status::NotSupported("drop table is not yet supported");
  }

  virtual Status DropSchemaTable(const std::string& table_name) override {
    return Status::NotSupported("drop table is not yet supported");
  }

  virtual Status GetKVTableOptions(const std::string& table_name,
                                   admin::KVTableOptions* options) override {
    std::unique_lock<std::mutex> guard(table_admin_mutex_);
    return Status::OK();
  }

  virtual Status GetListTableOptions(
      const std::string& table_name,
      admin::ListTableOptions* options) override {
    return Status::OK();
  }

  virtual Status GetHashTableOptions(
      const std::string& table_name,
      admin::HashTableOptions* options) override {
    return Status::OK();
  }

  virtual Status GetSetTableOptions(
      const std::string& table_name,
      admin::SetTableOptions* options) override {
    return Status::OK();
  }

  virtual Status GetSchemaTableOptions(
      const std::string& table_name,
      admin::SchemaTableOptions* options) override {
    return Status::OK();
  }

 private:
  void CloseKVTables() {
    auto tables = kv_tables_.load(std::memory_order_relaxed);
    assert(tables);
    for (auto& table : *tables) {
      table.second->Close();
    }
  }

  void CloseListTables() {
    auto tables = list_tables_.load(std::memory_order_relaxed);
    assert(tables);
    for (auto& table : *tables) {
      table.second->Close();
    }
  }

  void CloseHashTables() {
    auto tables = hash_tables_.load(std::memory_order_relaxed);
    assert(tables);
    for (auto& table : *tables) {
      table.second->Close();
    }
  }

  void CloseSetTables() {
    auto tables = set_tables_.load(std::memory_order_relaxed);
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

  using KVTableMapType =
    std::map<std::string, std::shared_ptr<KVTable>>;
  using ListTableMapType =
  std::map<std::string, std::shared_ptr<ListTable>>;
  using HashTableMapType =
  std::map<std::string, std::shared_ptr<HashTable>>;
  using SetTableMapType =
  std::map<std::string, std::shared_ptr<SetTable>>;
  using SchemaTableMapType =
    std::map<std::string, std::shared_ptr<SchemaTable>>;
  // Make sure lock is held when adding or removing a table
  // But no need lock held when read or write some tables
  std::mutex table_admin_mutex_;
  std::atomic<KVTableMapType*> kv_tables_ { nullptr };
  std::atomic<ListTableMapType*> list_tables_ { nullptr };
  std::atomic<HashTableMapType*> hash_tables_ { nullptr };
  std::atomic<SetTableMapType*> set_tables_ { nullptr };
  std::atomic<SchemaTableMapType*> schema_tables_ { nullptr };

  DbPathInfo db_path_info_;

  MetaTable meta_table_;
};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_MANAGER_IMPL_H_

