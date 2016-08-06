// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#include "schema_table.h"

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"

namespace uranium {

Status SchemaTable::Init(const internal::TableOptions& config) {
  std::vector<rocksdb::ColumnFamilyDescriptor> cfds;
  rocksdb::Options opt;
  opt.create_if_missing = true;
  opt.error_if_exists = false;

  cfds.push_back(rocksdb::ColumnFamilyDescriptor(
      rocksdb::kDefaultColumnFamilyName, opt));
  std::vector<rocksdb::ColumnFamilyHandle*> cfhs;
  std::string name =
      config.table_path() + config.options().table_name().name();
  rocksdb::Status s = rocksdb::DB::Open(opt, name, cfds, &cfhs, &db_);
  if (!s.ok()) {
    return Status(s);
  }
  return Status::OK();
}

}  // namespace uranium
