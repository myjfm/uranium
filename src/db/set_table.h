// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_SET_TABLE_H_
#define URANIUM_DB_SET_TABLE_H_

#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>

#include "common/coding.h"
#include "common/status.h"
#include "table.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

class SetTable : public Table {
 public:
  SetTable() {}
  virtual ~SetTable() {}

  SetTable(const SetTable&) = delete;
  SetTable& operator=(const SetTable&) = delete;

  Status Init(const internal::TableOptions& config) override {
    assert(config.options().table_type() == common::TableType::SET);
    assert(config.options().has_set_table_options());
    if (config.options().set_table_options().type() !=
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

  Status Close() override {
    if (db_) {
      delete db_;
    }
    return Status::OK();
  }

 private:

  rocksdb::DB* db_ { nullptr };
  internal::TableOptions options_;
};

}  // namespace uranium

#endif  // URANIUM_DB_SET_TABLE_H_