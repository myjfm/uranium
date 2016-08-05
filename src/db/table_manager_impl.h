// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_TABLE_MANAGER_IMPL_H_
#define URANIUM_DB_TABLE_MANAGER_IMPL_H_

#include "table_manager.h"

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

  virtual ~TableManagerImpl();

  TableManagerImpl(const TableManagerImpl&) = delete;
  TableManagerImpl& operator=(const TableManagerImpl&) = delete;

  // Not thread safe, should only call once
  virtual Status Init(const std::string& db_paths) override;

  virtual Status Close() override {
    CloseKVTables();
    CloseListTables();
    CloseHashTables();
    CloseSetTables();
    CloseSchemaTables();
    return Status::OK();
  }

  virtual std::shared_ptr<KVTable> GetKVTable(const std::string& name) override;
  virtual std::shared_ptr<ListTable> GetListTable(
      const std::string& name) override;
  virtual std::shared_ptr<HashTable> GetHashTable(
      const std::string& name) override;
  virtual std::shared_ptr<SetTable> GetSetTable(
      const std::string& name) override;
  virtual std::shared_ptr<SchemaTable> GetSchemaTable(
      const std::string& name) override;

  virtual Status CreateKVTable(const std::string& table_name,
                               const admin::KVTableOptions& options) override;
  virtual Status CreateListTable(
      const std::string& table_name,
      const admin::ListTableOptions& options) override;

  virtual Status CreateHashTable(
      const std::string& table_name,
      const admin::HashTableOptions& options) override;
  virtual Status CreateSetTable(
      const std::string& table_name,
      const admin::SetTableOptions& options) override;

  virtual Status CreateSchemaTable(
      const std::string& table_name,
      const admin::SchemaTableOptions& options) override;

  virtual Status UpdateKVTable(const std::string& table_name,
                               const admin::KVTableOptions& options) override;
  virtual Status UpdateListTable(
      const std::string& table_name,
      const admin::ListTableOptions& options) override;
  virtual Status UpdateHashTable(
      const std::string& table_name,
      const admin::HashTableOptions& options) override;
  virtual Status UpdateSetTable(
      const std::string& table_name,
      const admin::SetTableOptions& options) override;
  virtual Status UpdateSchemaTable(
      const std::string& table_name,
      const admin::SchemaTableOptions& options) override;

  virtual Status DropKVTable(const std::string& table_name) override;
  virtual Status DropListTable(const std::string& table_name) override;
  virtual Status DropHashTable(const std::string& table_name) override;
  virtual Status DropSetTable(const std::string& table_name) override;
  virtual Status DropSchemaTable(const std::string& table_name) override;

  virtual Status GetKVTableOptions(const std::string& table_name,
                                   admin::KVTableOptions* options) override;
  virtual Status GetListTableOptions(
      const std::string& table_name,
      admin::ListTableOptions* options) override;
  virtual Status GetHashTableOptions(
      const std::string& table_name,
      admin::HashTableOptions* options) override;
  virtual Status GetSetTableOptions(
      const std::string& table_name,
      admin::SetTableOptions* options) override;
  virtual Status GetSchemaTableOptions(
      const std::string& table_name,
      admin::SchemaTableOptions* options) override;

 private:
  void CloseKVTables();
  void CloseListTables();
  void CloseHashTables();
  void CloseSetTables();
  void CloseSchemaTables();

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

