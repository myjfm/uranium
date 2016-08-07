// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#ifndef URANIUM_DB_LIST_TABLE_H_
#define URANIUM_DB_LIST_TABLE_H_

#include <rocksdb/db.h>
#include <rocksdb/merge_operator.h>

#include "common/coding.h"
#include "common/status.h"
#include "table.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

class ListTable : public Table {
 public:
  ListTable() {}
  virtual ~ListTable() {}

  ListTable(const ListTable&) = delete;
  ListTable& operator=(const ListTable&) = delete;

  Status Init(const internal::TableOptions& config) override;

  Status Close() override {
    if (db_) {
      delete db_;
    }
    return Status::OK();
  }

  Status LPush(const std::string& key,
               const google::protobuf::RepeatedPtrField<api::Value>& values);

  Status LPop(const std::string& key, ::uranium::api::Value* value);

  Status RPush(const std::string& key,
               const google::protobuf::RepeatedPtrField<api::Value>& values);

  Status RPop(const std::string& key, ::uranium::api::Value* value);

  Status LPushX(const std::string& key,
                const google::protobuf::RepeatedPtrField<api::Value>& values);

  Status RPushX(const std::string& key,
                const google::protobuf::RepeatedPtrField<api::Value>& values);

  Status Index(const std::string& key, int64_t index, api::Value* value);

  Status Set(const std::string& key, int64_t index, const std::string& value);

  Status Range(const std::string& key,
               int64_t start,
               int64_t end,
               google::protobuf::RepeatedPtrField<api::Value>* values);

  Status Length(const std::string& key, int64_t* length);

  Status RemoveAll(const std::string& key);

 private:
  enum class MergeType : uint8_t {
    kNone   = 0,
    kLPush  = 1,
    kRPush  = 2,
    kLPushX = 3,
    kRPushX = 4,
    kSet    = 5,
  };

  class ListMergeOperator : public rocksdb::MergeOperator {
    virtual bool FullMergeV2(
        const rocksdb::MergeOperator::MergeOperationInput &merge_in,
        rocksdb::MergeOperator::MergeOperationOutput *merge_out) const override;

    virtual bool PartialMerge(const rocksdb::Slice &key,
                              const rocksdb::Slice &left_operand,
                              const rocksdb::Slice &right_operand,
                              std::string *new_value,
                              rocksdb::Logger *logger) const override;
    virtual const char* Name() const override {
      return "List FLAT Merge";
    }
  };

  static void MarshalLPushValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    result->append(1, static_cast<char>(MergeType::kLPush));
    MarshalValues(values, result);
  }

  static void MarshalRPushValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    result->append(1, static_cast<char>(MergeType::kRPush));
    MarshalValues(values, result);
  }

  static void MarshalLPushXValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    result->append(1, static_cast<char>(MergeType::kLPushX));
    MarshalValues(values, result);
  }

  static void MarshalRPushXValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    result->append(1, static_cast<char>(MergeType::kRPushX));
    MarshalValues(values, result);
  }

  static void MarshalSetValue(const std::string& value,
                              int64_t index,
                              std::string* result) {
    result->append(1, static_cast<char>(MergeType::kSet));
    PutVarint64(result, index);
    PutLengthPrefixedStringPiece(result, value);
  }

  static void MarshalValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result);

  static void UnmarshalValues(const std::string& marshaled_value,
                       int64_t num,  // <0 means unmarshal all values
                       std::vector<StringPiece>* values);

  rocksdb::DB* db_ { nullptr };
  internal::TableOptions options_;
};

}  // namespace uranium

#endif  // URANIUM_DB_LIST_TABLE_H_
