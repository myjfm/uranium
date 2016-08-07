// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#include "meta_table.h"

#include <memory>

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"

namespace uranium {

Status MetaTable::Init(const std::string& path) {
  rocksdb::Options opt;
  opt.create_if_missing = true;
  opt.error_if_exists = false;
  opt.num_levels = 2;
  auto s = rocksdb::DB::Open(opt, path + "uranium.meta", &db_);
  if (!s.ok()) {
    return Status(s);
  }
  return Status::OK();
}

Status MetaTable::LoadAllTables(std::vector<internal::TableOptions>* tables) {
  assert(db_);
  auto s = LoadKVTables(tables);
  if (!s.ok()) {
    return s;
  }

  s = LoadListTables(tables);
  if (!s.ok()) {
    return s;
  }

  s = LoadHashTables(tables);
  if (!s.ok()) {
    return s;
  }

  s = LoadSetTables(tables);
  if (!s.ok()) {
    return s;
  }

  s = LoadSchemaTables(tables);
  return s;
}

Status MetaTable::LoadKVTables(std::vector<internal::TableOptions>* tables) {
  assert(db_);
  std::string str_options_list;
  auto rs = db_->Get(rocksdb::ReadOptions(), "kv_tables", &str_options_list);
  if (rs.ok()) {
    if (!kv_table_options_list_.ParseFromString(str_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0; i < kv_table_options_list_.options_list_size(); ++i) {
      tables->push_back(kv_table_options_list_.options_list(i));
    }
  } else if (rs.IsNotFound()) {
    return Status::OK();
  }
  return Status(rs);
}

Status MetaTable::LoadListTables(std::vector<internal::TableOptions>* tables) {
  assert(db_);
  std::string str_options_list;
  auto rs = db_->Get(rocksdb::ReadOptions(), "list_tables", &str_options_list);
  if (rs.ok()) {
    if (!list_table_options_list_.ParseFromString(str_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0; i < list_table_options_list_.options_list_size(); ++i) {
      tables->push_back(list_table_options_list_.options_list(i));
    }
  } else if (rs.IsNotFound()) {
    return Status::OK();
  }
  return Status(rs);
}

Status MetaTable::LoadHashTables(std::vector<internal::TableOptions>* tables) {
  assert(db_);
  std::string str_options_list;
  auto rs = db_->Get(rocksdb::ReadOptions(), "hash_tables", &str_options_list);
  if (rs.ok()) {
    if (!hash_table_options_list_.ParseFromString(str_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0; i < hash_table_options_list_.options_list_size(); ++i) {
      tables->push_back(hash_table_options_list_.options_list(i));
    }
  } else if (rs.IsNotFound()) {
    return Status::OK();
  }
  return Status(rs);
}

Status MetaTable::LoadSetTables(std::vector<internal::TableOptions>* tables) {
  assert(db_);
  std::string str_options_list;
  auto rs = db_->Get(rocksdb::ReadOptions(), "set_tables", &str_options_list);
  if (rs.ok()) {
    if (!set_table_options_list_.ParseFromString(str_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0; i < set_table_options_list_.options_list_size(); ++i) {
      tables->push_back(set_table_options_list_.options_list(i));
    }
  } else if (rs.IsNotFound()) {
    return Status::OK();
  }
  return Status(rs);
}

Status MetaTable::LoadSchemaTables(std::vector<internal::TableOptions>* tables){
  assert(db_);
  std::string str_options_list;
  auto rs = db_->Get(rocksdb::ReadOptions(),
                     "schema_tables",
                     &str_options_list);
  if (rs.ok()) {
    if (!schema_table_options_list_.ParseFromString(str_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0; i < schema_table_options_list_.options_list_size(); ++i) {
      tables->push_back(schema_table_options_list_.options_list(i));
    }
  } else if (rs.IsNotFound()) {
    return Status::OK();
  }
  return Status(rs);
}

Status MetaTable::CreateKVTable(const std::string& table_path,
                                const std::string& table_name,
                                const admin::CommonTableOptions& common_options,
                                const admin::KVTableOptions& table_options) {
  admin::TableOptions admin_table_options;
  admin_table_options.set_table_type(common::TableType::KV);
  admin_table_options.mutable_common_table_options()->CopyFrom(common_options);
  admin_table_options.mutable_kv_table_options()->CopyFrom(table_options);
  admin_table_options.mutable_table_name()->set_name(table_name);

  internal::TableOptions internal_table_options;
  internal_table_options.set_table_path(table_path);
  internal_table_options.mutable_options()->CopyFrom(admin_table_options);

  internal::TableOptionsList tmp_kv_table_options_list;
  tmp_kv_table_options_list.CopyFrom(kv_table_options_list_);
  tmp_kv_table_options_list.add_options_list()->CopyFrom(
      internal_table_options);
  std::string str_kv_table_options_list;
  tmp_kv_table_options_list.SerializeToString(&str_kv_table_options_list);
  auto rs = db_->Put(rocksdb::WriteOptions(),
                     "kv_tables",
                     str_kv_table_options_list);
  if (!rs.ok()) {
    return Status(rs);
  }
  kv_table_options_list_.add_options_list()->CopyFrom(internal_table_options);
  return Status::OK();
}

Status MetaTable::CreateListTable(
    const std::string& table_path,
    const std::string& table_name,
    const admin::CommonTableOptions& common_options,
    const admin::ListTableOptions& table_options) {
  admin::TableOptions admin_table_options;
  admin_table_options.set_table_type(common::TableType::LIST);
  admin_table_options.mutable_common_table_options()->CopyFrom(common_options);
  admin_table_options.mutable_list_table_options()->CopyFrom(table_options);
  admin_table_options.mutable_table_name()->set_name(table_name);

  internal::TableOptions internal_table_options;
  internal_table_options.set_table_path(table_path);
  internal_table_options.mutable_options()->CopyFrom(admin_table_options);

  internal::TableOptionsList tmp_list_table_options_list;
  tmp_list_table_options_list.CopyFrom(list_table_options_list_);
  tmp_list_table_options_list.add_options_list()->CopyFrom(
      internal_table_options);
  std::string str_list_table_options_list;
  tmp_list_table_options_list.SerializeToString(&str_list_table_options_list);
  auto rs = db_->Put(rocksdb::WriteOptions(),
                     "list_tables",
                     str_list_table_options_list);
  if (!rs.ok()) {
    return Status(rs);
  }
  list_table_options_list_.add_options_list()->CopyFrom(internal_table_options);
  return Status::OK();
}

Status MetaTable::CreateHashTable(
    const std::string& table_path,
    const std::string& table_name,
    const admin::CommonTableOptions& common_options,
    const admin::HashTableOptions& table_options) {
  admin::TableOptions admin_table_options;
  admin_table_options.set_table_type(common::TableType::HASH);
  admin_table_options.mutable_common_table_options()->CopyFrom(common_options);
  admin_table_options.mutable_hash_table_options()->CopyFrom(table_options);
  admin_table_options.mutable_table_name()->set_name(table_name);

  internal::TableOptions internal_table_options;
  internal_table_options.set_table_path(table_path);
  internal_table_options.mutable_options()->CopyFrom(admin_table_options);

  internal::TableOptionsList tmp_hash_table_options_list;
  tmp_hash_table_options_list.CopyFrom(hash_table_options_list_);
  tmp_hash_table_options_list.add_options_list()->CopyFrom(
      internal_table_options);
  std::string str_hash_table_options_list;
  tmp_hash_table_options_list.SerializeToString(&str_hash_table_options_list);
  auto rs = db_->Put(rocksdb::WriteOptions(),
                     "hash_tables",
                     str_hash_table_options_list);
  if (!rs.ok()) {
    return Status(rs);
  }
  hash_table_options_list_.add_options_list()->CopyFrom(internal_table_options);
  return Status::OK();
}

Status MetaTable::CreateSetTable(
    const std::string& table_path,
    const std::string& table_name,
    const admin::CommonTableOptions& common_options,
    const admin::SetTableOptions& table_options) {
  admin::TableOptions admin_table_options;
  admin_table_options.set_table_type(common::TableType::SET);
  admin_table_options.mutable_common_table_options()->CopyFrom(common_options);
  admin_table_options.mutable_set_table_options()->CopyFrom(table_options);
  admin_table_options.mutable_table_name()->set_name(table_name);

  internal::TableOptions internal_table_options;
  internal_table_options.set_table_path(table_path);
  internal_table_options.mutable_options()->CopyFrom(admin_table_options);

  internal::TableOptionsList tmp_set_table_options_list;
  tmp_set_table_options_list.CopyFrom(set_table_options_list_);
  tmp_set_table_options_list.add_options_list()->CopyFrom(
      internal_table_options);
  std::string str_set_table_options_list;
  tmp_set_table_options_list.SerializeToString(&str_set_table_options_list);
  auto rs = db_->Put(rocksdb::WriteOptions(),
                     "set_tables",
                     str_set_table_options_list);
  if (!rs.ok()) {
    return Status(rs);
  }
  set_table_options_list_.add_options_list()->CopyFrom(internal_table_options);
  return Status::OK();
}

Status MetaTable::CreateSchemaTable(
    const std::string& table_path,
    const std::string& table_name,
    const admin::CommonTableOptions& common_options,
    const admin::SchemaTableOptions& table_options) {
  admin::TableOptions admin_table_options;
  admin_table_options.set_table_type(common::TableType::SCHEMA);
  admin_table_options.mutable_common_table_options()->CopyFrom(common_options);
  admin_table_options.mutable_schema_table_options()->CopyFrom(table_options);
  admin_table_options.mutable_table_name()->set_name(table_name);

  internal::TableOptions internal_table_options;
  internal_table_options.set_table_path(table_path);
  internal_table_options.mutable_options()->CopyFrom(admin_table_options);

  internal::TableOptionsList tmp_schema_table_options_list;
  tmp_schema_table_options_list.CopyFrom(schema_table_options_list_);
  tmp_schema_table_options_list.add_options_list()->CopyFrom(
      internal_table_options);
  std::string str_schema_table_options_list;
  tmp_schema_table_options_list.SerializeToString(
      &str_schema_table_options_list);
  auto rs = db_->Put(rocksdb::WriteOptions(),
                     "schema_tables",
                     str_schema_table_options_list);
  if (!rs.ok()) {
    return Status(rs);
  }
  schema_table_options_list_.add_options_list()->CopyFrom(
      internal_table_options);
  return Status::OK();
}

Status MetaTable::GetKVTableOptions(const std::string& table_name,
                                    admin::CommonTableOptions* common_options,
                                    admin::KVTableOptions* options) {
  for (auto itr = kv_table_options_list_.options_list().begin();
       itr != kv_table_options_list_.options_list().end();
       itr++) {
    if (itr->options().table_name().name() == table_name) {
      common_options->CopyFrom(itr->options().common_table_options());
      options->CopyFrom(itr->options().kv_table_options());
      return Status::OK();
    }
  }
  return Status::NotFound();
}

Status MetaTable::GetListTableOptions(const std::string& table_name,
                                      admin::CommonTableOptions* common_options,
                                      admin::ListTableOptions* options) {
  for (auto itr = list_table_options_list_.options_list().begin();
       itr != list_table_options_list_.options_list().end();
       itr++) {
    if (itr->options().table_name().name() == table_name) {
      common_options->CopyFrom(itr->options().common_table_options());
      options->CopyFrom(itr->options().list_table_options());
      return Status::OK();
    }
  }
  return Status::NotFound();
}

Status MetaTable::GetHashTableOptions(const std::string& table_name,
                                      admin::CommonTableOptions* common_options,
                                      admin::HashTableOptions* options) {
  for (auto itr = hash_table_options_list_.options_list().begin();
       itr != hash_table_options_list_.options_list().end();
       itr++) {
    if (itr->options().table_name().name() == table_name) {
      common_options->CopyFrom(itr->options().common_table_options());
      options->CopyFrom(itr->options().hash_table_options());
      return Status::OK();
    }
  }
  return Status::NotFound();
}

Status MetaTable::GetSetTableOptions(const std::string& table_name,
                                     admin::CommonTableOptions* common_options,
                                     admin::SetTableOptions* options) {
  for (auto itr = set_table_options_list_.options_list().begin();
       itr != set_table_options_list_.options_list().end();
       itr++) {
    if (itr->options().table_name().name() == table_name) {
      common_options->CopyFrom(itr->options().common_table_options());
      options->CopyFrom(itr->options().set_table_options());
      return Status::OK();
    }
  }
  return Status::NotFound();
}

Status MetaTable::GetSchemaTableOptions(
    const std::string& table_name,
    admin::CommonTableOptions* common_options,
    admin::SchemaTableOptions* options) {
  for (auto itr = schema_table_options_list_.options_list().begin();
       itr != schema_table_options_list_.options_list().end();
       itr++) {
    if (itr->options().table_name().name() == table_name) {
      common_options->CopyFrom(itr->options().common_table_options());
      options->CopyFrom(itr->options().schema_table_options());
      return Status::OK();
    }
  }
  return Status::NotFound();
}

}  // namespace uranium
