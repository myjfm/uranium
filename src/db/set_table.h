// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#ifndef URANIUM_DB_SET_TABLE_H_
#define URANIUM_DB_SET_TABLE_H_

#include <rocksdb/db.h>
#include <rocksdb/merge_operator.h>

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

  Status Add(const std::string& key,
             const google::protobuf::RepeatedPtrField<api::Value>& values);

  Status Length(const std::string& key, int64_t* length);

  Status IsMember(const std::string& key,
                  const std::string& value,
                  bool* yes);

  Status GetAll(const std::string& key,
                google::protobuf::RepeatedPtrField<api::Value>* values);

  Status Remove(const std::string& key,
                const google::protobuf::RepeatedPtrField<api::Value>& values);

  Status RemoveAll(const std::string& key);

 private:
  rocksdb::DB* db_ { nullptr };
  internal::TableOptions options_;
  enum MergeType : uint8_t {
    kNone   = 0,
    kAdd    = 1,
    kRemove = 2,
  };

  class SetMergeOperator : public rocksdb::MergeOperator {
    virtual bool FullMergeV2(
        const rocksdb::MergeOperator::MergeOperationInput &merge_in,
        rocksdb::MergeOperator::MergeOperationOutput *merge_out) const override;

    virtual bool PartialMerge(const rocksdb::Slice &key,
                              const rocksdb::Slice &left_operand,
                              const rocksdb::Slice &right_operand,
                              std::string *new_value,
                              rocksdb::Logger *logger) const override;
    virtual const char* Name() const override {
      return "Set FLAT Merge";
    }
  };


};

}  // namespace uranium

#endif  // URANIUM_DB_SET_TABLE_H_
