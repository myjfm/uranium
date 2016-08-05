// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_HASH_TABLE_H_
#define URANIUM_DB_HASH_TABLE_H_

#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>

#include "common/status.h"
#include "table.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

class HashTable : public Table {
 public:
  HashTable() {}
  virtual ~HashTable() {}

  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;

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

#endif  // URANIUM_DB_HASH_TABLE_H_
