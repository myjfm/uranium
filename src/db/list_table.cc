// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#include "list_table.h"

#include <rocksdb/write_batch.h>

#include "common/coding.h"
#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

Status ListTable::Init(const internal::TableOptions& config) {
  assert(config.options().table_type() == common::TableType::LIST);
  assert(config.options().has_list_table_options());
  if (config.options().list_table_options().type() !=
      admin::StorageType::FLAT) {
    // currently we only support FLAT storage type
    return Status::NotSupported("Not supported storage type");
  }

  rocksdb::Options opt;
  opt.create_if_missing = true;
  opt.error_if_exists = false;

  opt.merge_operator.reset(new ListMergeOperator());

  std::vector<rocksdb::ColumnFamilyDescriptor> cfds;
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

Status ListTable::LPush(
    const std::string& key,
    const google::protobuf::RepeatedPtrField<api::Value>& values) {
  assert(db_);
  std::string marshaled_value;
  MarshalLPushValues(values, &marshaled_value);
  rocksdb::WriteBatch wb;
  wb.Merge(key, marshaled_value);
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  return Status(rs);
}

Status ListTable::LPop(const std::string& key, ::uranium::api::Value* value) {
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

Status ListTable::RPush(
    const std::string& key,
    const google::protobuf::RepeatedPtrField<api::Value>& values) {
  assert(db_);
  std::string marshaled_value;
  MarshalRPushValues(values, &marshaled_value);
  rocksdb::WriteBatch wb;
  wb.Merge(key, marshaled_value);
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  return Status(rs);
}

Status ListTable::RPop(const std::string& key, ::uranium::api::Value* value) {
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

Status ListTable::LPushX(
    const std::string& key,
    const google::protobuf::RepeatedPtrField<api::Value>& values) {
  assert(db_);
  std::string marshaled_value;
  MarshalLPushXValues(values, &marshaled_value);
  rocksdb::WriteBatch wb;
  wb.Merge(key, marshaled_value);
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  return Status(rs);
}

Status ListTable::RPushX(
    const std::string& key,
    const google::protobuf::RepeatedPtrField<api::Value>& values) {
  assert(db_);
  std::string marshaled_value;
  MarshalRPushXValues(values, &marshaled_value);
  rocksdb::WriteBatch wb;
  wb.Merge(key, marshaled_value);
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  return Status(rs);
}

Status ListTable::Index(const std::string& key,
                        int64_t index,
                        api::Value* value) {
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

Status ListTable::Set(const std::string& key,
                      int64_t index,
                      const std::string& value) {
  assert(db_);
  std::string marshaled_value;
  MarshalSetValue(value, index, &marshaled_value);
  rocksdb::WriteBatch wb;
  wb.Merge(key, marshaled_value);
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  return Status(rs);
}

Status ListTable::Range(
    const std::string& key,
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

Status ListTable::Length(const std::string& key, int64_t* length) {
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

Status ListTable::RemoveAll(const std::string& key) {
  assert(db_);
  auto rs = db_->Delete(rocksdb::WriteOptions(), key);
  if (!rs.ok()) {
    return Status(rs);
  }
  return Status::OK();
}

void ListTable::MarshalValues(
    const google::protobuf::RepeatedPtrField<api::Value>& values,
    std::string* result) {
  for (auto itr = values.begin(); itr != values.end(); itr++) {
    PutLengthPrefixedStringPiece(result, itr->value());
  }
}

void ListTable::UnmarshalValues(const std::string& marshaled_value,
                                int64_t num,  // <0 means unmarshal all values
                                std::vector<StringPiece>* values) {
  StringPiece input(marshaled_value);
  StringPiece output;
  while (GetLengthPrefixedStringPiece(&input, &output) &&
         (num-- > 0 || num < 0)) {
    values->push_back(output);
  }
}

bool ListTable::ListMergeOperator::FullMergeV2(
    const MergeOperationInput &merge_in,
    MergeOperationOutput *merge_out) const {
  std::deque<StringPiece> cells;
  if (merge_in.existing_value) {
    StringPiece s(merge_in.existing_value->data(),
                  merge_in.existing_value->size());
    StringPiece cell;
    while (GetLengthPrefixedStringPiece(&s, &cell)) {
      cells.push_back(cell);
    }
  }
  for (auto& operand : merge_in.operand_list) {
    if (operand.size() <= 1) {
      return false;
    }
    MergeType merge_type = static_cast<MergeType>(operand[0]);
    StringPiece o(operand.data() + 1, operand.size() - 1);
    switch (merge_type) {
      case MergeType::kLPush:
      {
        StringPiece tmp;
        while (GetLengthPrefixedStringPiece(&o, &tmp)) {
          cells.push_front(tmp);
        }
      }
        break;
      case MergeType::kRPush:
      {
        StringPiece tmp;
        while (GetLengthPrefixedStringPiece(&o, &tmp)) {
          cells.push_back(tmp);
        }
      }
        break;
      case MergeType::kLPushX:
        if (cells.size() > 0) {
          StringPiece tmp;
          while (GetLengthPrefixedStringPiece(&o, &tmp)) {
            cells.push_front(tmp);
          }
        }
        break;
      case MergeType::kRPushX:
        if (cells.size() > 0) {
          StringPiece tmp;
          while (GetLengthPrefixedStringPiece(&o, &tmp)) {
            cells.push_back(tmp);
          }
        }
        break;
      case MergeType::kSet:
        if (cells.size() > 0) {
          StringPiece tmp;
          uint64_t index;
          while (GetVarint64(&o, &index) &&
                 GetLengthPrefixedStringPiece(&o, &tmp)) {
            if (static_cast<size_t>(index) >= cells.size()) {
              continue;
            }
            cells[static_cast<size_t>(index)] = tmp;
          }
        }
        break;
      default:
        return false;
    }
  }
  for (auto itr = cells.begin(); itr != cells.end(); ++itr) {
    PutLengthPrefixedStringPiece(&(merge_out->new_value), *itr);
  }
  return true;
}

bool ListTable::ListMergeOperator::PartialMerge(
    const rocksdb::Slice& key,
    const rocksdb::Slice& left_operand,
    const rocksdb::Slice& right_operand,
    std::string* new_value,
    rocksdb::Logger* logger) const {
  if (left_operand.size() <= 1 ||
      right_operand.size() <= 1 ||
      (static_cast<MergeType>(left_operand[0]) !=
       static_cast<MergeType>(right_operand[0]))) {
    return false;
  }
  StringPiece tmp_left(left_operand.data() + 1, left_operand.size() - 1);
  StringPiece tmp_right(right_operand.data() + 1, right_operand.size() - 1);
  switch (static_cast<MergeType>(left_operand[0])) {
    case MergeType::kLPush:
    case MergeType::kRPush:
    case MergeType::kLPushX:
    case MergeType::kRPushX:
      new_value->append(1, left_operand[0]);
      new_value->append(tmp_left.data(), tmp_left.size());
      new_value->append(tmp_right.data(), tmp_right.size());
      return true;
    case MergeType::kSet:
      new_value->append(1, left_operand[0]);
      {
        std::map<int64_t, StringPiece> m;
        uint64_t index;
        StringPiece v;
        while (GetVarint64(&tmp_left, &index) &&
               GetLengthPrefixedStringPiece(&tmp_left, &v)) {
          auto itr = m.find(static_cast<int64_t>(index));
          if (itr == m.end()) {
            m[static_cast<int64_t>(index)] = v;
          }
        }
        while (GetVarint64(&tmp_right, &index) &&
               GetLengthPrefixedStringPiece(&tmp_right, &v)) {
          auto itr = m.find(static_cast<int64_t>(index));
          if (itr == m.end()) {
            m[static_cast<int64_t>(index)] = v;
          }
        }
        for (auto& mm : m) {
          PutVarint64(new_value, static_cast<uint64_t>(mm.first));
          PutLengthPrefixedStringPiece(new_value, mm.second);
        }
      }
      return true;
    default:
      break;
  }
  return false;
}

}  // namespace uranium
