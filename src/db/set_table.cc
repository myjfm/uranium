// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#include "set_table.h"

#include <rocksdb/write_batch.h>

#include "common/coding.h"
#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

Status SetTable::Init(const internal::TableOptions& config) {
  assert(config.options().table_type() == common::TableType::SET);
  assert(config.options().has_set_table_options());
  if (config.options().set_table_options().type() !=
      admin::StorageType::FLAT) {
    // currently we only support FLAT storage type
    return Status::NotSupported("Not supported storage type");
  }

  std::vector<rocksdb::ColumnFamilyDescriptor> cfds;
  rocksdb::Options opt;
  opt.create_if_missing = true;
  opt.error_if_exists = false;
  opt.merge_operator.reset(new SetMergeOperator());

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

Status SetTable::Add(
    const std::string& key,
    const google::protobuf::RepeatedPtrField<api::Value>& values) {
  assert(db_);
  std::string tmp;
  tmp.append(1, static_cast<char>(MergeType::kAdd));
  for (auto itr = values.begin(); itr != values.end(); ++itr) {
    PutLengthPrefixedStringPiece(&tmp, itr->value());
  }
  db_->Merge(rocksdb::WriteOptions(), key, tmp);
  return Status::OK();
}

Status SetTable::Length(const std::string& key, int64_t* length) {
  assert(db_);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  StringPiece values(tmp_values);
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&values, &v)) {
    (*length)++;
  }
  return Status::OK();
}

Status SetTable::IsMember(const std::string& key,
    const std::string& value,
    bool* yes) {
  assert(db_);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  StringPiece values(tmp_values);
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&values, &v)) {
    if (v == value) {
      *yes = true;
      break;
    }
  }
  return Status::OK();
}

Status SetTable::GetAll(const std::string& key,
    google::protobuf::RepeatedPtrField<api::Value>* values) {
  assert(db_);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  StringPiece vs(tmp_values);
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&vs, &v)) {
    values->Add()->mutable_value()->append(v.data(), v.size());
  }
  return Status::OK();
}

Status SetTable::Remove(const std::string& key,
    const google::protobuf::RepeatedPtrField<api::Value>& values) {
  assert(db_);
  std::string tmp;
  tmp.append(1, static_cast<char>(MergeType::kRemove));
  for (auto itr = values.begin(); itr != values.end(); ++itr) {
    PutLengthPrefixedStringPiece(&tmp, itr->value());
  }
  db_->Merge(rocksdb::WriteOptions(), key, tmp);
  return Status::OK();
}

Status SetTable::RemoveAll(const std::string& key) {
  assert(db_);
  auto rs = db_->Delete(rocksdb::WriteOptions(), key);
  return Status(rs);
}

bool SetTable::SetMergeOperator::FullMergeV2(
    const MergeOperationInput &merge_in,
    MergeOperationOutput *merge_out) const {
  std::set<StringPiece> values;
  if (merge_in.existing_value) {
    StringPiece s(merge_in.existing_value->data(),
        merge_in.existing_value->size());
    StringPiece v;
    while (GetLengthPrefixedStringPiece(&s, &v)) {
      values.insert(v);
    }
  }
  for (auto& operand : merge_in.operand_list) {
    if (operand.size() <= 1) {
      return false;
    }
    MergeType merge_type = static_cast<MergeType>(operand[0]);
    StringPiece o(operand.data() + 1, operand.size() - 1);
    switch (merge_type) {
      case MergeType::kAdd:
        {
          StringPiece tmp_v;
          while (GetLengthPrefixedStringPiece(&o, &tmp_v)) {
            values.insert(tmp_v);
          }
        }
        break;
      case MergeType::kRemove:
        if (values.size() > 0) {
          StringPiece tmp_v;
          while (GetLengthPrefixedStringPiece(&o, &tmp_v)) {
            auto itr = values.find(tmp_v);
            if (itr != values.end()) {
              values.erase(itr);
            }
          }
        }
        break;
      default:
        return false;
    }
  }
  for (auto itr = values.begin(); itr != values.end(); ++itr) {
    PutLengthPrefixedStringPiece(&(merge_out->new_value), *itr);
  }
  return true;
}

bool SetTable::SetMergeOperator::PartialMerge(
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
    case MergeType::kRemove:
      new_value->append(1, left_operand[0]);
      {
        std::set<StringPiece> s;
        StringPiece k;
        while (GetLengthPrefixedStringPiece(&tmp_left, &k)) {
          s.insert(k);
        }
        while (GetLengthPrefixedStringPiece(&tmp_right, &k)) {
          s.insert(k);
        }
        for (auto itr = s.begin(); itr != s.end(); ++itr) {
          PutLengthPrefixedStringPiece(new_value, *itr);
        }
      }
      return true;
    case MergeType::kAdd:
      new_value->append(1, left_operand[0]);
      {
        std::set<StringPiece> s;
        StringPiece v;
        while (GetLengthPrefixedStringPiece(&tmp_left, &v)) {
          s.insert(v);
        }
        while (GetLengthPrefixedStringPiece(&tmp_right, &v)) {
          s.insert(v);
        }
        for (auto& ss : s) {
          PutLengthPrefixedStringPiece(new_value, ss);
        }
      }
      return true;
    default:
      break;
  }
  return false;
}

}  // namespace uranium
