// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_META_TABLE_H_
#define URANIUM_DB_META_TABLE_H_

#include <memory>

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "kv_table.h"

namespace uranium {

// This is a light-weighted RocksDB instance, which is used to stores all
// meta data of uranium.
// NOTE:
// This class is not thread safe, needs external lock protected when accessing
// by multiple threads.
class MetaTable final {
 public:
  MetaTable() {}
  virtual ~MetaTable() {}

  MetaTable(const MetaTable&) = delete;
  MetaTable& operator=(const MetaTable&) = delete;

  Status Init(const std::string& path) {
    auto s = rocksdb::DB::Open(rocksdb::Options(), path + "uranium.meta", &db_);
    if (!s.ok()) {
      return Status(s);
    }
    return Status::OK();
  }

  Status Close() {
    if (db_) {
      delete db_;
      db_ = nullptr;
    }
    return Status::OK();
  }

  Status LoadAllTables(std::vector<internal::TableOptions>* tables) {
    assert(db_);
    std::string str_kv_table_options_list;
    auto rs = db_->Get(rocksdb::ReadOptions(),
                       "kv_tables",
                       &str_kv_table_options_list);
    if (!rs.ok()) {
      return Status(rs);
    }
    if (!kv_table_options_list_.ParseFromString(
          str_kv_table_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0;
         i < kv_table_options_list_.options_list_size();
         ++i) {
      tables->push_back(kv_table_options_list_.options_list(i));
    }

    std::string str_list_table_options_list;
    rs = db_->Get(rocksdb::ReadOptions(),
                  "list_tables",
                  &str_list_table_options_list);
    if (!rs.ok()) {
      return Status(rs);
    }
    if (!list_table_options_list_.ParseFromString(
        str_list_table_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0;
         i < list_table_options_list_.options_list_size();
         ++i) {
      tables->push_back(list_table_options_list_.options_list(i));
    }

    std::string str_hash_table_options_list;
    rs = db_->Get(rocksdb::ReadOptions(),
                  "hash_tables",
                  &str_hash_table_options_list);
    if (!rs.ok()) {
      return Status(rs);
    }
    if (!hash_table_options_list_.ParseFromString(
        str_hash_table_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0;
         i < hash_table_options_list_.options_list_size();
         ++i) {
      tables->push_back(hash_table_options_list_.options_list(i));
    }

    std::string str_set_table_options_list;
    rs = db_->Get(rocksdb::ReadOptions(),
                  "set_tables",
                  &str_set_table_options_list);
    if (!rs.ok()) {
      return Status(rs);
    }
    if (!set_table_options_list_.ParseFromString(
        str_set_table_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0;
         i < set_table_options_list_.options_list_size();
         ++i) {
      tables->push_back(set_table_options_list_.options_list(i));
    }

    std::string str_schema_table_options_list;
    rs = db_->Get(rocksdb::ReadOptions(),
                  "schema_tables",
                  &str_schema_table_options_list);
    if (!rs.ok()) {
      return Status(rs);
    }
    if (!schema_table_options_list_.ParseFromString(
          str_schema_table_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0; i < schema_table_options_list_.options_list_size(); ++i) {
      tables->push_back(schema_table_options_list_.options_list(i));
    }
    return Status::OK();
  }

  Status CreateKVTable(const std::string& table_path,
                       const std::string& table_name,
                       const admin::KVTableOptions& table_options) {
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::KV);
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

  Status CreateListTable(const std::string& table_path,
                         const std::string& table_name,
                         const admin::ListTableOptions& table_options) {
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::LIST);
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

  Status CreateHashTable(const std::string& table_path,
                         const std::string& table_name,
                         const admin::HashTableOptions& table_options) {
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::HASH);
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

  Status CreateSetTable(const std::string& table_path,
                        const std::string& table_name,
                        const admin::SetTableOptions& table_options) {
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::SET);
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

  Status CreateSchemaTable(const std::string& table_path,
                           const std::string& table_name,
                           const admin::SchemaTableOptions& table) {
    admin::TableOptions admin_table_options;
    admin_table_options.set_table_type(common::TableType::SCHEMA);
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
    schema_table_options_list_.add_options_list()->CopyFrom(table);
    return Status::OK();
  }

  Status GetKVTableOptions(const std::string& table_name,
                           admin::KVTableOptions* options) {
    for (auto itr = kv_table_options_list_.options_list().begin();
         itr != kv_table_options_list_.options_list().end();
         itr++) {
      if (itr->options().table_name().name() == table_name) {
        options->CopyFrom(itr->options().kv_table_options());
        return Status::OK();
      }
    }
    return Status::NotFound();
  }

  Status GetListTableOptions(const std::string& table_name,
                             admin::ListTableOptions* options) {
    for (auto itr = list_table_options_list_.options_list().begin();
         itr != list_table_options_list_.options_list().end();
         itr++) {
      if (itr->options().table_name().name() == table_name) {
        options->CopyFrom(itr->options().list_table_options());
        return Status::OK();
      }
    }
    return Status::NotFound();
  }

  Status GetHashTableOptions(const std::string& table_name,
                             admin::HashTableOptions* options) {
    for (auto itr = hash_table_options_list_.options_list().begin();
         itr != hash_table_options_list_.options_list().end();
         itr++) {
      if (itr->options().table_name().name() == table_name) {
        options->CopyFrom(itr->options().hash_table_options());
        return Status::OK();
      }
    }
    return Status::NotFound();
  }

  Status GetSetTableOptions(const std::string& table_name,
                            admin::SetTableOptions* options) {
    for (auto itr = set_table_options_list_.options_list().begin();
         itr != set_table_options_list_.options_list().end();
         itr++) {
      if (itr->options().table_name().name() == table_name) {
        options->CopyFrom(itr->options().set_table_options());
        return Status::OK();
      }
    }
    return Status::NotFound();
  }

  Status GetSchemaTableOptions(const std::string& table_name,
                               admin::SchemaTableOptions* options) {
    for (auto itr = schema_table_options_list_.options_list().begin();
         itr != schema_table_options_list_.options_list().end();
         itr++) {
      if (itr->options().table_name().name() == table_name) {
        options->CopyFrom(itr->options().schema_table_options());
        return Status::OK();
      }
    }
    return Status::NotFound();
  }

 private:
  rocksdb::DB* db_ { nullptr };

  internal::TableOptionsList kv_table_options_list_;
  internal::TableOptionsList list_table_options_list_;
  internal::TableOptionsList hash_table_options_list_;
  internal::TableOptionsList set_table_options_list_;
  internal::TableOptionsList schema_table_options_list_;
};

}  // namespace uranium

#endif  // URANIUM_DB_META_TABLE_H_

