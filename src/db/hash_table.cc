// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#include "hash_table.h"

#include <rocksdb/write_batch.h>

#include "common/coding.h"
#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

Status HashTable::Init(const internal::TableOptions& config) {
  assert(config.options().table_type() == common::TableType::HASH);
  assert(config.options().has_hash_table_options());
  if (config.options().hash_table_options().type() !=
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

}  // namespace uranium
