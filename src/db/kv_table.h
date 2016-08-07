// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#ifndef URANIUM_DB_KV_TABLE_H_
#define URANIUM_DB_KV_TABLE_H_

#include <rocksdb/db.h>

#include "common/status.h"
#include "table.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

class KVTable : public Table {
 public:
  KVTable() {}
  virtual ~KVTable() {}

  KVTable(const KVTable&) = delete;
  KVTable& operator=(const KVTable&) = delete;

  Status Init(const internal::TableOptions& config) override;

  Status Close() override {
    if (db_) {
      delete db_;
    }
    return Status::OK();
  }

  Status Get(
      const google::protobuf::RepeatedPtrField<api::Key>& keys,
      google::protobuf::RepeatedPtrField<api::KeyValue>* values);

  Status Set(const google::protobuf::RepeatedPtrField<api::KeyValue>& kvs);

  Status Remove(const google::protobuf::RepeatedPtrField<api::Key>& keys);

 private:
  rocksdb::DB* db_ { nullptr };
  internal::TableOptions options_;
};

}  // namespace uranium

#endif  // URANIUM_DB_KV_TABLE_H_
