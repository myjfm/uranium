// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_SCHEMALESS_TABLE_H_
#define URANIUM_DB_SCHEMALESS_TABLE_H_

#include <rocksdb/db.h>

#include "common/status.h"
#include "table.h"
#include "table_options.h"

namespace uranium {

class SchemalessTable : public Table {
 public:
  SchemalessTable() {}
  virtual ~SchemalessTable() {}

  SchemalessTable(const SchemalessTable&) = delete;
  SchemalessTable& operator=(const SchemalessTable&) = delete;

  Status Init(const TableOptions& config) override {
    std::vector<rocksdb::ColumnFamilyDescriptor> cfds;
    rocksdb::Options opt;
    cfds.push_back(rocksdb::ColumnFamilyDescriptor(
          rocksdb::kDefaultColumnFamilyName, opt));
    std::vector<rocksdb::ColumnFamilyHandle*> cfhs;
    rocksdb::Status s = rocksdb::DB::Open(opt, config.name, cfds, &cfhs, &db_);
    if (!s.ok()) {
      return Status(s);
    }
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
};

}  // namespace uranium

#endif  // URANIUM_DB_SCHEMALESS_TABLE_H_

