// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#ifndef URANIUM_DB_SCHEMA_TABLE_H_
#define URANIUM_DB_SCHEMA_TABLE_H_

#include <rocksdb/db.h>

#include "common/status.h"
#include "table.h"
#include "network/cpp/uranium_internal.pb.h"

namespace uranium {

class SchemaTable : public Table {
 public:
  SchemaTable() {}
  virtual ~SchemaTable() {}

  SchemaTable(const SchemaTable&) = delete;
  SchemaTable& operator=(const SchemaTable&) = delete;

  Status Init(const internal::TableOptions& config) override;

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

#endif  // URANIUM_DB_SCHEMA_TABLE_H_

