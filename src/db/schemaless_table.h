// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_SCHEMALESS_TABLE_H_
#define URANIUM_DB_SCHEMALESS_TABLE_H_

#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>

#include "common/status.h"
#include "table.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

class SchemalessTable : public Table {
 public:
  SchemalessTable() {}
  virtual ~SchemalessTable() {}

  SchemalessTable(const SchemalessTable&) = delete;
  SchemalessTable& operator=(const SchemalessTable&) = delete;

  Status Init(const internal::TableOptions& config) override {
    assert(config.options().has_schemaless_table_options());
    if (config.options().schemaless_table_options().type() !=
        admin::StorageType::FLAT) {
      return Status::NotSupported("Not supported storage type");
    }

    std::vector<rocksdb::ColumnFamilyDescriptor> cfds;
    rocksdb::Options opt;
    cfds.push_back(rocksdb::ColumnFamilyDescriptor(
          rocksdb::kDefaultColumnFamilyName, opt));
    std::vector<rocksdb::ColumnFamilyHandle*> cfhs;
    std::string name =
      config.table_path() + config.options().table_name().name();
    rocksdb::Status s = rocksdb::DB::Open(opt, name, cfds, &cfhs, &db_);
    if (!s.ok()) {
      return Status(s);
    }

    options_.CopyFrom(config);
    return Status::OK();
  }

  Status Close() override {
    if (db_) {
      delete db_;
    }
    return Status::OK();
  }

  Status KVGet(
      const google::protobuf::RepeatedPtrField<api::Key>& keys,
      google::protobuf::RepeatedPtrField<api::KeyValue>* values) {
    assert(db_);
    std::vector<rocksdb::Slice> rocks_keys(keys.size());
    for (auto itr = keys.begin(); itr != keys.end(); ++itr) {
      rocks_keys.push_back(itr->key());
    }
    std::vector<std::string> rocks_values(keys.size());
    auto rss = db_->MultiGet(rocksdb::ReadOptions(), rocks_keys, &rocks_values);
    for (int i = 0; i < rss.size(); ++i) {
      if (rss[i].ok()) {
        auto kv = values->Add();
        kv->mutable_key()->set_key(keys.Get(i).key());
        kv->mutable_value()->set_value(rocks_values[i]);
      }
    }
    return Status::OK();
  }

  Status KVSet(const google::protobuf::RepeatedPtrField<api::KeyValue>& kvs) {
    assert(db_);
    rocksdb::WriteBatch wb;
    for (auto itr = kvs.begin(); itr != kvs.end(); ++itr) {
      wb.Put(itr->key().key(), itr->value().value());
    }
    auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
    if (!rs.ok()) {
      return Status(rs);
    }
    return Status::OK();
  }

 private:
  rocksdb::DB* db_ { nullptr };
  internal::TableOptions options_;
};

}  // namespace uranium

#endif  // URANIUM_DB_SCHEMALESS_TABLE_H_
