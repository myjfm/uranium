// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#include "hash_table.h"

#include <rocksdb/cache.h>
#include <rocksdb/filter_policy.h>
#include <rocksdb/memtablerep.h>
#include <rocksdb/slice_transform.h>
#include <rocksdb/table.h>
#include <rocksdb/write_batch.h>

#include "common/coding.h"
#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

Status HashTable::Init(const internal::TableOptions& config) {
  assert(config.options().table_type() == common::TableType::HASH);
  assert(config.options().has_hash_table_options());
  if (config.options().hash_table_options().type() !=
      admin::StorageType::FLAT) {
    // currently we only support FLAT storage type
    return Status::NotSupported("Not supported storage type");
  }

  rocksdb::Options opt;
  opt.create_if_missing = true;
  opt.error_if_exists = false;
  opt.write_buffer_size = 1024 * 1024;
  opt.max_write_buffer_number = 2;
  opt.num_levels = 3;
  opt.max_background_flushes = 2;
  opt.max_background_compactions = 2;
  rocksdb::BlockBasedTableOptions block_based_table_options;
  // enable bloom filter to optimize read performance
  block_based_table_options.filter_policy.reset(
      rocksdb::NewBloomFilterPolicy(10));
  block_based_table_options.block_cache = rocksdb::NewLRUCache(10485760);
  opt.table_factory.reset(
      rocksdb::NewBlockBasedTableFactory(block_based_table_options));
  if (config.options().has_common_table_options()) {
    const admin::CommonTableOptions& copt =
      config.options().common_table_options();
    if (copt.mem_table_memory() > 0) {
      opt.write_buffer_size = copt.mem_table_memory();
    }
    if (copt.mem_table_num() > 0) {
      opt.write_buffer_size = copt.mem_table_num();
    }
    if (copt.max_flush_threads() > 0) {
      opt.max_background_flushes = copt.max_flush_threads();
    }
    if (copt.max_compaction_threads() > 0) {
      opt.max_background_compactions = copt.max_compaction_threads();
    }
    if (copt.num_levels() > 0) {
      opt.num_levels = copt.num_levels();
    }
    if (copt.rocks_table_type() == admin::RocksTableType::PLAIN) {
      // plaintable mode must turn on mmap
      opt.allow_mmap_reads = true;
      rocksdb::PlainTableOptions plain_table_options;
      if (copt.plain_encoding_type() == "prefix") {
        plain_table_options.encoding_type = rocksdb::kPrefix;
      } else {
        plain_table_options.encoding_type = rocksdb::kPlain;
      }
      opt.table_factory.reset(
          rocksdb::NewPlainTableFactory(plain_table_options));
      int fixed_prefix_transform = 8;
      if (copt.fixed_prefix_transform() > 0) {
        fixed_prefix_transform = copt.fixed_prefix_transform();
      }
      opt.prefix_extractor.reset(
          rocksdb::NewFixedPrefixTransform(fixed_prefix_transform));
      opt.memtable_factory.reset(rocksdb::NewHashSkipListRepFactory());
    } else {
      int64_t lru_cache_size = 10485760;
      if (copt.lru_cache_size() > 0) {
        lru_cache_size = copt.lru_cache_size();
      }
      block_based_table_options.filter_policy.reset(
          rocksdb::NewBloomFilterPolicy(10));
      block_based_table_options.block_cache =
        rocksdb::NewLRUCache(lru_cache_size);
      opt.table_factory.reset(
          rocksdb::NewBlockBasedTableFactory(block_based_table_options));
    }
  }

  opt.merge_operator.reset(new HashMergeOperator());

  std::string name =
      config.table_path() + config.options().table_name().name();
  auto s = rocksdb::DB::Open(opt, name, &db_);
  if (!s.ok()) {
    return Status(s);
  }

  options_.CopyFrom(config);
  return Status::OK();
}

Status HashTable::Get(
    const std::string& key,
    const google::protobuf::RepeatedPtrField<api::Field>& fields,
    google::protobuf::RepeatedPtrField<api::FieldValue>* fvs) {
  assert(db_);
  assert(fvs);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  std::set<StringPiece> fs;
  for (auto itr = fields.begin(); itr != fields.end(); ++itr) {
    fs.insert(itr->field());
  }
  StringPiece values(tmp_values);
  StringPiece k;
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&values, &k)) {
    if (!GetLengthPrefixedStringPiece(&values, &v)) {
      return Status::Corruption("data corrupted");
    }
    if (fs.find(k) != fs.end()) {
      fvs->Add()->mutable_key()->set_field(k.data(), k.size());
      fvs->Add()->mutable_value()->set_value(v.data(), v.size());
    }
  }
  return Status::OK();
}

Status HashTable::GetAll(
      const std::string& key,
      google::protobuf::RepeatedPtrField<api::FieldValue>* fvs) {
  assert(db_);
  assert(fvs);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  StringPiece values(tmp_values);
  StringPiece k;
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&values, &k)) {
    if (!GetLengthPrefixedStringPiece(&values, &v)) {
      return Status::Corruption("data corrupted");
    }
    fvs->Add()->mutable_key()->set_field(k.data(), k.size());
    fvs->Add()->mutable_value()->set_value(v.data(), v.size());
  }
  return Status::OK();
}

Status HashTable::GetAllFields(
      const std::string& key,
      google::protobuf::RepeatedPtrField<api::Field>* fields) {
  assert(db_);
  assert(fields);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  StringPiece values(tmp_values);
  StringPiece k;
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&values, &k)) {
    if (!GetLengthPrefixedStringPiece(&values, &v)) {
      return Status::Corruption("data corrupted");
    }
    fields->Add()->set_field(k.data(), k.size());
  }
  return Status::OK();
}

Status HashTable::GetAllValues(
      const std::string& key,
      google::protobuf::RepeatedPtrField<api::Value>* values) {
  assert(db_);
  assert(values);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  StringPiece vs(tmp_values);
  StringPiece k;
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&vs, &k)) {
    if (!GetLengthPrefixedStringPiece(&vs, &v)) {
      return Status::Corruption("data corrupted");
    }
    values->Add()->set_value(v.data(), v.size());
  }
  return Status::OK();
}

Status HashTable::Length(const std::string& key, int64_t* length) {
  assert(db_);
  assert(length);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  StringPiece vs(tmp_values);
  StringPiece k;
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&vs, &k)) {
    if (!GetLengthPrefixedStringPiece(&vs, &v)) {
      return Status::Corruption("data corrupted");
    }
    (*length)++;
  }
  return Status::OK();
}

Status HashTable::Set(
      const std::string& key,
      const google::protobuf::RepeatedPtrField<api::FieldValue>& fvs) {
  assert(db_);
  std::string marshaled_value;
  MarshalSetValue(fvs, &marshaled_value);
  rocksdb::WriteBatch wb;
  wb.Merge(key, marshaled_value);
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  return Status(rs);
}

Status HashTable::SetX(const std::string& key, const api::FieldValue& fv) {
  assert(db_);
  std::string marshaled_value;
  MarshalSetXValue(fv, &marshaled_value);
  rocksdb::WriteBatch wb;
  wb.Merge(key, marshaled_value);
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  return Status(rs);
}

Status HashTable::Remove(
      const std::string& key,
      const google::protobuf::RepeatedPtrField<api::Field>& fields) {
  assert(db_);
  std::string marshaled_value;
  MarshalRemoveValue(fields, &marshaled_value);
  rocksdb::WriteBatch wb;
  wb.Merge(key, marshaled_value);
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  return Status(rs);
}

Status HashTable::Exists(const std::string& key,
                         const api::Field& field,
                         bool* exists) {
  assert(db_);
  assert(exists);
  std::string tmp_values;
  auto rs = db_->Get(rocksdb::ReadOptions(), key, &tmp_values);
  if (!rs.ok()) {
    return Status(rs);
  }
  StringPiece vs(tmp_values);
  StringPiece k;
  StringPiece v;
  while (GetLengthPrefixedStringPiece(&vs, &k)) {
    if (!GetLengthPrefixedStringPiece(&vs, &v)) {
      return Status::Corruption("data corrupted");
    }
    if (k == field.field()) {
      *exists = true;
      return Status::OK();
    }
  }
  *exists = false;
  return Status::OK();
}

Status HashTable::RemoveAll(const std::string& key) {
  assert(db_);
  auto rs = db_->Delete(rocksdb::WriteOptions(), key);
  return Status(rs);
}

void HashTable::MarshalSetValue(
    const google::protobuf::RepeatedPtrField<api::FieldValue>& fvs,
    std::string* value) {
  value->append(1, static_cast<char>(MergeType::kSet));
  for (auto itr = fvs.begin(); itr != fvs.end(); ++itr) {
    PutLengthPrefixedStringPiece(value, itr->key().field());
    PutLengthPrefixedStringPiece(value, itr->value().value());
  }
}

void HashTable::MarshalSetXValue(const api::FieldValue& fv,
                                 std::string* value) {
  value->append(1, static_cast<char>(MergeType::kSetX));
  PutLengthPrefixedStringPiece(value, fv.key().field());
  PutLengthPrefixedStringPiece(value, fv.value().value());
}

void HashTable::MarshalRemoveValue(
    const google::protobuf::RepeatedPtrField<api::Field>& fields,
    std::string* value) {
  value->append(1, static_cast<char>(MergeType::kRemove));
  for (auto itr = fields.begin(); itr != fields.end(); ++itr) {
    PutLengthPrefixedStringPiece(value, itr->field());
  }
}

bool HashTable::HashMergeOperator::FullMergeV2(
    const MergeOperationInput &merge_in,
    MergeOperationOutput *merge_out) const {
  std::map<StringPiece, StringPiece> cells;
  if (merge_in.existing_value) {
    StringPiece s(merge_in.existing_value->data(),
                  merge_in.existing_value->size());
    StringPiece k;
    StringPiece v;
    while (GetLengthPrefixedStringPiece(&s, &k)) {
      if (!GetLengthPrefixedStringPiece(&s, &v)) {
        return false;
      }
      cells[k] = v;
    }
  }
  for (auto& operand : merge_in.operand_list) {
    if (operand.size() <= 1) {
      return false;
    }
    MergeType merge_type = static_cast<MergeType>(operand[0]);
    StringPiece o(operand.data() + 1, operand.size() - 1);
    switch (merge_type) {
      case MergeType::kSet:
        {
          StringPiece tmp_k;
          StringPiece tmp_v;
          while (GetLengthPrefixedStringPiece(&o, &tmp_k)) {
            if (!GetLengthPrefixedStringPiece(&o, &tmp_v)) {
              return false;
            }
            cells[tmp_k] = tmp_v;
          }
        }
        break;
      case MergeType::kSetX:
        {
          StringPiece tmp_k;
          StringPiece tmp_v;
          while (GetLengthPrefixedStringPiece(&o, &tmp_k)) {
            if (GetLengthPrefixedStringPiece(&o, &tmp_v)) {
              return false;
            }
            if (cells.find(tmp_k) != cells.end()) {
              cells[tmp_k] = tmp_v;
            }
          }
        }
        break;
      case MergeType::kRemove:
        if (cells.size() > 0) {
          StringPiece tmp;
          while (GetLengthPrefixedStringPiece(&o, &tmp)) {
            auto itr = cells.find(tmp);
            if (itr != cells.end()) {
              cells.erase(itr);
            }
          }
        }
        break;
      default:
        return false;
    }
  }
  for (auto itr = cells.begin(); itr != cells.end(); ++itr) {
    PutLengthPrefixedStringPiece(&(merge_out->new_value), itr->first);
    PutLengthPrefixedStringPiece(&(merge_out->new_value), itr->second);
  }
  return true;
}

bool HashTable::HashMergeOperator::PartialMerge(
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
    case MergeType::kSet:
    case MergeType::kSetX:
      new_value->append(1, left_operand[0]);
      {
        std::map<StringPiece, StringPiece> m;
        StringPiece k;
        StringPiece v;
        while (GetLengthPrefixedStringPiece(&tmp_left, &k)) {
          if (!GetLengthPrefixedStringPiece(&tmp_left, &v)) {
            return false;
          }
          m[k] = v;
        }
        while (GetLengthPrefixedStringPiece(&tmp_right, &k)) {
          if (!GetLengthPrefixedStringPiece(&tmp_right, &v)) {
            return false;
          }
          m[k] = v;
        }
        for (auto& mm : m) {
          PutLengthPrefixedStringPiece(new_value, mm.first);
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
