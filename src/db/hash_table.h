// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#ifndef URANIUM_DB_HASH_TABLE_H_
#define URANIUM_DB_HASH_TABLE_H_

#include <rocksdb/db.h>
#include <rocksdb/merge_operator.h>

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

  Status Get(
      const std::string& key,
      const google::protobuf::RepeatedPtrField<api::Field>& fields,
      google::protobuf::RepeatedPtrField<api::FieldValue>* fvs);

  Status GetAll(
      const std::string& key,
      google::protobuf::RepeatedPtrField<api::FieldValue>* fvs);

  Status GetAllFields(
      const std::string& key,
      google::protobuf::RepeatedPtrField<api::Field>* fields);

  Status GetAllValues(
      const std::string& key,
      google::protobuf::RepeatedPtrField<api::Value>* values);

  Status Length(const std::string& key, int64_t* length);

  Status Set(
      const std::string& key,
      const google::protobuf::RepeatedPtrField<api::FieldValue>& fvs);

  Status SetX(const std::string& key, const api::FieldValue& fv);

  Status Remove(
      const std::string& key,
      const google::protobuf::RepeatedPtrField<api::Field>& fields);

  Status Exists(const std::string& key, const api::Field& field, bool* exists);

  Status RemoveAll(const std::string& key);

 private:
  rocksdb::DB* db_ { nullptr };
  internal::TableOptions options_;
  enum class MergeType : uint8_t {
    kNone   = 0,
    kSet    = 1,
    kSetX   = 2,
    kRemove = 3,
  };

  class HashMergeOperator : public rocksdb::MergeOperator {
    virtual bool FullMergeV2(
        const rocksdb::MergeOperator::MergeOperationInput &merge_in,
        rocksdb::MergeOperator::MergeOperationOutput *merge_out) const override;

    virtual bool PartialMerge(const rocksdb::Slice &key,
                              const rocksdb::Slice &left_operand,
                              const rocksdb::Slice &right_operand,
                              std::string *new_value,
                              rocksdb::Logger *logger) const override;
    virtual const char* Name() const override {
      return "Hash FLAT Merge";
    }
  };

  void MarshalSetValue(
      const google::protobuf::RepeatedPtrField<api::FieldValue>& fvs,
      std::string* value);

  void MarshalSetXValue(const api::FieldValue& fv, std::string* value);

  void MarshalRemoveValue(
      const google::protobuf::RepeatedPtrField<api::Field>& fields,
      std::string* value);
};

}  // namespace uranium

#endif  // URANIUM_DB_HASH_TABLE_H_
