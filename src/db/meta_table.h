// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_META_TABLE_H_
#define URANIUM_DB_META_TABLE_H_

#include <memory>

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "schemaless_table.h"

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
    std::string str_schemaless_table_options_list;
    auto rs = db_->Get(rocksdb::ReadOptions(),
                       "schemaless_tables",
                       &str_schemaless_table_options_list);
    if (!rs.ok()) {
      return Status(rs);
    }
    if (!schemaless_table_options_list_.ParseFromString(
          str_schemaless_table_options_list)) {
      return Status::Corruption("meta db corrupts");
    }
    for (int i = 0;
         i < schemaless_table_options_list_.options_list_size();
         ++i) {
      tables->push_back(schemaless_table_options_list_.options_list(i));
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

  Status CreateTable(const internal::TableOptions& table) {
    if (table.options().has_schema_table_options()) {
      internal::TableOptionsList tmp_schema_table_options_list;
      tmp_schema_table_options_list.CopyFrom(schema_table_options_list_);
      tmp_schema_table_options_list.add_options_list()->CopyFrom(table);
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
    } else if (table.options().has_schemaless_table_options()) {
      internal::TableOptionsList tmp_schemaless_table_options_list;
      tmp_schemaless_table_options_list.CopyFrom(schemaless_table_options_list_);
      tmp_schemaless_table_options_list.add_options_list()->CopyFrom(table);
      std::string str_schemaless_table_options_list;
      tmp_schemaless_table_options_list.SerializeToString(
          &str_schemaless_table_options_list);
      auto rs = db_->Put(rocksdb::WriteOptions(),
                         "schemaless_tables",
                         str_schemaless_table_options_list);
      if (!rs.ok()) {
        return Status(rs);
      }
      schemaless_table_options_list_.add_options_list()->CopyFrom(table);
      return Status::OK();
    }/* else { */
      return Status::InvalidArgument("unknown table type");
    /*}*/
  }

 private:
  rocksdb::DB* db_ { nullptr };

  internal::TableOptionsList schemaless_table_options_list_;
  internal::TableOptionsList schema_table_options_list_;
};

}  // namespace uranium

#endif  // URANIUM_DB_META_TABLE_H_

