// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_LIST_TABLE_H_
#define URANIUM_DB_LIST_TABLE_H_

#include <rocksdb/db.h>
#include <rocksdb/write_batch.h>

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

  Status Init(const internal::TableOptions& config) override {
    assert(config.options().table_type() == common::TableType::LIST);
    assert(config.options().has_list_table_options());
    if (config.options().list_table_options().type() !=
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

  Status LPush(const std::string& key,
               const google::protobuf::RepeatedPtrField<api::Value>& values) {
    assert(db_);
    std::string marshaled_value;
    MarshalLPushValues(values, &marshaled_value);
    rocksdb::WriteBatch wb;
    wb.Merge(key, marshaled_value);
    auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
    return Status(rs);
  }

  Status LPop(const std::string& key, ::uranium::api::Value* value) {
    assert(db_);
    std::string marshaled_value;
    auto s = db_->Get(rocksdb::ReadOptions(), key, &marshaled_value);
    if (s.IsNotFound()) {
      return Status(s);
    }
    std::vector<StringPiece> values;
    values.reserve(1);
    UnmarshalValues(marshaled_value, 1, &values);
    if (values.size() == 1) {
      value->set_value(values[0].data(), values[0].size());
      return Status::OK();
    } else {
      return Status::Corruption("data corrupted");
    }
  }

  Status RPush(const std::string& key,
               const google::protobuf::RepeatedPtrField<api::Value>& values) {
    assert(db_);
    std::string marshaled_value;
    MarshalRPushValues(values, &marshaled_value);
    rocksdb::WriteBatch wb;
    wb.Merge(key, marshaled_value);
    auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
    return Status(rs);
  }

  Status RPop(const std::string& key, ::uranium::api::Value* value) {
    assert(db_);
    std::string marshaled_value;
    auto s = db_->Get(rocksdb::ReadOptions(), key, &marshaled_value);
    if (s.IsNotFound()) {
      return Status(s);
    }
    std::vector<StringPiece> values;
    values.reserve(16);
    UnmarshalValues(marshaled_value, -1, &values);
    if (values.size() > 0) {
      value->set_value(values[values.size() - 1].data(),
                       values[values.size() - 1].size());
      return Status::OK();
    } else {
      return Status::Corruption("data corrupted");
    }
  }

  Status LPushX(const std::string& key,
                const google::protobuf::RepeatedPtrField<api::Value>& values) {
    assert(db_);
    std::string marshaled_value;
    MarshalLPushXValues(values, &marshaled_value);
    rocksdb::WriteBatch wb;
    wb.Merge(key, marshaled_value);
    auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
    return Status(rs);
  }

  Status RPushX(const std::string& key,
                const google::protobuf::RepeatedPtrField<api::Value>& values) {
    assert(db_);
    std::string marshaled_value;
    MarshalRPushXValues(values, &marshaled_value);
    rocksdb::WriteBatch wb;
    wb.Merge(key, marshaled_value);
    auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
    return Status(rs);
  }

  Status Index(const std::string& key, int64_t index, api::Value* value) {
    assert(db_);
    std::string marshaled_value;
    auto s = db_->Get(rocksdb::ReadOptions(), key, &marshaled_value);
    if (s.IsNotFound()) {
      return Status(s);
    }
    std::vector<StringPiece> values;
    values.reserve(16);
    UnmarshalValues(marshaled_value, -1, &values);
    if (values.size() > 0) {
      index = (index < 0) ?
              (static_cast<int64_t>(values.size()) + index) :
              index;
      if (index < 0 || index >= values.size()) {
        return Status::OutOfRange("index is out of range");
      }
      value->set_value(values[index].data(), values[index].size());
      return Status::OK();
    } else {
      return Status::Corruption("data corrupted");
    }
  }

  Status Set(const std::string& key, int64_t index, const std::string& value) {
    assert(db_);
    std::string marshaled_value;
    MarshalSetValue(value, &marshaled_value);
    rocksdb::WriteBatch wb;
    wb.Merge(key, value);
    auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
    return Status(rs);
  }

  Status Range(const std::string& key,
               int64_t start,
               int64_t end,
               google::protobuf::RepeatedPtrField<api::Value>* values) {
    assert(db_);
    std::string marshaled_value;
    auto s = db_->Get(rocksdb::ReadOptions(), key, &marshaled_value);
    if (s.IsNotFound()) {
      return Status(s);
    }
    std::vector<StringPiece> unmarshaled_values;
    unmarshaled_values.reserve(16);
    UnmarshalValues(marshaled_value, -1, &unmarshaled_values);
    if (unmarshaled_values.size() > 0) {
      int64_t length = static_cast<int64_t>(unmarshaled_values.size());
      start = (start >= 0) ? start : (length + start);
      start = (start < 0) ? 0 : start;
      if (start >= length) {
        return Status::OutOfRange("start is too large");
      }
      end = (end >= 0) ? end : (length + end);
      if (end < 0) {
        return Status::OutOfRange("end is too small");
      }
      end = (end >= length) ? (length - 1) : end;
      if (start > end) {
        return Status::OutOfRange("start is larger than end");
      }
      for (size_t i = start; i <= end; ++i) {
        values->Add()->set_value(unmarshaled_values[i].data(),
                                 unmarshaled_values[i].size());
      }
      return Status::OK();
    } else {
      return Status::Corruption("data corrupted");
    }
  }

  Status Length(const std::string& key, int64_t* length) {
    assert(db_);
    std::string marshaled_value;
    auto s = db_->Get(rocksdb::ReadOptions(), key, &marshaled_value);
    if (s.IsNotFound()) {
      return Status(s);
    }
    std::vector<StringPiece> unmarshaled_values;
    unmarshaled_values.reserve(16);
    UnmarshalValues(marshaled_value, -1, &unmarshaled_values);
    if (unmarshaled_values.size() > 0) {
      *length = static_cast<int64_t>(unmarshaled_values.size());
      return Status::OK();
    } else {
      return Status::Corruption("data corrupted");
    }
  }

  Status RemoveAll(const std::string& key) {
    assert(db_);
    auto rs = db_->Delete(rocksdb::WriteOptions(), key);
    if (!rs.ok()) {
      return Status(rs);
    }
    return Status::OK();
  }

 private:
  enum class MergeType : uint8_t {
    kNone   = 0,
    kLPush  = 1,
    kRPush  = 2,
    kLPushX = 3,
    kRPushX = 4,
    kSet    = 5,
  };

  void MarshalLPushValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    result->append(1, static_cast<char>(MergeType::kLPush));
    MarshalValues(values, result);
  }

  void MarshalRPushValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    result->append(1, static_cast<char>(MergeType::kRPush));
    MarshalValues(values, result);
  }

  void MarshalLPushXValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    result->append(1, static_cast<char>(MergeType::kLPushX));
    MarshalValues(values, result);
  }

  void MarshalRPushXValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    result->append(1, static_cast<char>(MergeType::kRPushX));
    MarshalValues(values, result);
  }

  void MarshalSetValue(const std::string& value, std::string* result) {
    result->append(1, static_cast<char>(MergeType::kSet));
    PutLengthPrefixedStringPiece(result, value);
  }

  void MarshalValues(
      const google::protobuf::RepeatedPtrField<api::Value>& values,
      std::string* result) {
    for (auto itr = values.begin(); itr != values.end(); itr++) {
      PutLengthPrefixedStringPiece(result, itr->value());
    }
  }

  void UnmarshalValues(const std::string& marshaled_value,
                       int64_t num,  // <0 means unmarshal all values
                       std::vector<StringPiece>* values) {
    StringPiece input(marshaled_value);
    StringPiece output;
    while (GetLengthPrefixedStringPiece(&input, &output) &&
           (num-- > 0 || num < 0)) {
      values->push_back(output);
    }
  }

  rocksdb::DB* db_ { nullptr };
  internal::TableOptions options_;
};

}  // namespace uranium

#endif  // URANIUM_DB_LIST_TABLE_H_
