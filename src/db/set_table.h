// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#ifndef URANIUM_DB_SET_TABLE_H_
#define URANIUM_DB_SET_TABLE_H_

#include <rocksdb/db.h>

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

  Status Init(const internal::TableOptions& config) override;

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
