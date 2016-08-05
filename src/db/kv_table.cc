// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#include "kv_table.h"

#include <rocksdb/write_batch.h>

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

Status KVTable::Init(const internal::TableOptions& config) {
  assert(config.options().table_type() == common::TableType::KV);
  assert(config.options().has_kv_table_options());
  if (config.options().kv_table_options().type() !=
      admin::StorageType::FLAT) {
    // currently we only support FLAT storage type
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

Status KVTable::Get(
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

Status KVTable::Set(
    const google::protobuf::RepeatedPtrField<api::KeyValue>& kvs) {
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

Status KVTable::Remove(
    const google::protobuf::RepeatedPtrField<api::Key>& keys) {
  assert(db_);
  rocksdb::WriteBatch wb;
  for (auto itr = keys.begin(); itr != keys.end(); ++itr) {
    wb.Delete(itr->key());
  }
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  if (!rs.ok()) {
    return Status(rs);
  }
  return Status::OK();
}

}  // namespace uranium
