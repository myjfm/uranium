// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#include "table_manager_impl.h"

#include <unistd.h>

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"

namespace uranium {

TableManagerImpl::~TableManagerImpl() {
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

// Not thread safe, should only call once
Status TableManagerImpl::Init(const std::string& db_paths) {
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
    } else if (table_option.options().table_type() ==
        common::TableType::SCHEMA) {
      auto schema_table = std::shared_ptr<SchemaTable>(new SchemaTable());
      s = schema_table->Init(table_option);
      if (!s.ok()) {
        return s;
      }
      auto schema_tables = schema_tables_.load(std::memory_order_relaxed);
      (*schema_tables)[table_option.options().table_name().name()] =
        schema_table;
      if (db_path_info_.PathExists(table_option.table_path())) {
        db_path_info_.IncreasePathWeight(table_option.table_path());
      }
    } else {
      return Status::NotSupported("unknown table type");
    }
  }
  return Status::OK();
}

std::shared_ptr<KVTable> TableManagerImpl::GetKVTable(const std::string& name) {
  auto tables = kv_tables_.load(std::memory_order_relaxed);
  assert(tables);
  auto itr = tables->find(name);
  if (itr == tables->end()) {
    return std::shared_ptr<KVTable>();
  }
  return itr->second;
}

std::shared_ptr<ListTable> TableManagerImpl::GetListTable(
    const std::string& name) {
  auto tables = list_tables_.load(std::memory_order_relaxed);
  assert(tables);
  auto itr = tables->find(name);
  if (itr == tables->end()) {
    return std::shared_ptr<ListTable>();
  }
  return itr->second;
}

std::shared_ptr<HashTable> TableManagerImpl::GetHashTable(
    const std::string& name) {
  auto tables = hash_tables_.load(std::memory_order_relaxed);
  assert(tables);
  auto itr = tables->find(name);
  if (itr == tables->end()) {
    return std::shared_ptr<HashTable>();
  }
  return itr->second;
}

std::shared_ptr<SetTable> TableManagerImpl::GetSetTable(
    const std::string& name) {
  auto tables = set_tables_.load(std::memory_order_relaxed);
  assert(tables);
  auto itr = tables->find(name);
  if (itr == tables->end()) {
    return std::shared_ptr<SetTable>();
  }
  return itr->second;
}

std::shared_ptr<SchemaTable> TableManagerImpl::GetSchemaTable(
    const std::string& name) {
  auto tables = schema_tables_.load(std::memory_order_relaxed);
  assert(tables);
  auto itr = tables->find(name);
  if (itr == tables->end()) {
    return std::shared_ptr<SchemaTable>();
  }
  return itr->second;
}

Status TableManagerImpl::CreateKVTable(const std::string& table_name,
                                       const admin::KVTableOptions& options) {
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
  return Status::OK();
}

Status TableManagerImpl::CreateListTable(
    const std::string& table_name,
    const admin::ListTableOptions& options) {
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
  return Status::OK();
}

Status TableManagerImpl::CreateHashTable(
    const std::string& table_name,
    const admin::HashTableOptions& options) {
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
  return Status::OK();
}

Status TableManagerImpl::CreateSetTable(const std::string& table_name,
                                        const admin::SetTableOptions& options) {
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
  return Status::OK();
}

Status TableManagerImpl::CreateSchemaTable(
    const std::string& table_name,
    const admin::SchemaTableOptions& options) {
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
  return Status::OK();
}

Status TableManagerImpl::UpdateKVTable(const std::string& table_name,
                                       const admin::KVTableOptions& options) {
  return Status::NotSupported("update table is not yet supported");
}

Status TableManagerImpl::UpdateListTable(
    const std::string& table_name,
    const admin::ListTableOptions& options) {
  return Status::NotSupported("update table is not yet supported");
}

Status TableManagerImpl::UpdateHashTable(
    const std::string& table_name,
    const admin::HashTableOptions& options) {
  return Status::NotSupported("update table is not yet supported");
}

Status TableManagerImpl::UpdateSetTable(const std::string& table_name,
                                        const admin::SetTableOptions& options) {
  return Status::NotSupported("update table is not yet supported");
}

Status TableManagerImpl::UpdateSchemaTable(
    const std::string& table_name,
    const admin::SchemaTableOptions& options) {
  return Status::NotSupported("update table is not yet supported");
}

Status TableManagerImpl::DropKVTable(const std::string& table_name) {
  return Status::NotSupported("drop table is not yet supported");
}

Status TableManagerImpl::DropListTable(const std::string& table_name) {
  return Status::NotSupported("drop table is not yet supported");
}

Status TableManagerImpl::DropHashTable(const std::string& table_name) {
  return Status::NotSupported("drop table is not yet supported");
}

Status TableManagerImpl::DropSetTable(const std::string& table_name) {
  return Status::NotSupported("drop table is not yet supported");
}

Status TableManagerImpl::DropSchemaTable(const std::string& table_name){
  return Status::NotSupported("drop table is not yet supported");
}

Status TableManagerImpl::GetKVTableOptions(const std::string& table_name,
                                           admin::KVTableOptions* options) {
  std::unique_lock<std::mutex> guard(table_admin_mutex_);
  return Status::OK();
}

Status TableManagerImpl::GetListTableOptions(const std::string& table_name,
                                             admin::ListTableOptions* options) {
  return Status::OK();
}

Status TableManagerImpl::GetHashTableOptions(const std::string& table_name,
                                             admin::HashTableOptions* options) {
  return Status::OK();
}

Status TableManagerImpl::GetSetTableOptions(const std::string& table_name,
                                            admin::SetTableOptions* options) {
  return Status::OK();
}

Status TableManagerImpl::GetSchemaTableOptions(
    const std::string& table_name,
    admin::SchemaTableOptions* options) {
  return Status::OK();
}

void TableManagerImpl::CloseKVTables() {
  auto tables = kv_tables_.load(std::memory_order_relaxed);
  assert(tables);
  for (auto& table : *tables) {
    table.second->Close();
  }
}

void TableManagerImpl::CloseListTables() {
  auto tables = list_tables_.load(std::memory_order_relaxed);
  assert(tables);
  for (auto& table : *tables) {
    table.second->Close();
  }
}

void TableManagerImpl::CloseHashTables() {
  auto tables = hash_tables_.load(std::memory_order_relaxed);
  assert(tables);
  for (auto& table : *tables) {
    table.second->Close();
  }
}

void TableManagerImpl::CloseSetTables() {
  auto tables = set_tables_.load(std::memory_order_relaxed);
  assert(tables);
  for (auto& table : *tables) {
    table.second->Close();
  }
}

void TableManagerImpl::CloseSchemaTables() {
  auto tables = schema_tables_.load(std::memory_order_relaxed);
  assert(tables);
  for (auto& table : *tables) {
    table.second->Close();
  }
}

}  // namespace uranium

