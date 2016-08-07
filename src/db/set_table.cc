// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#include "set_table.h"

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

Status SetTable::Init(const internal::TableOptions& config) {
  assert(config.options().table_type() == common::TableType::SET);
  assert(config.options().has_set_table_options());
  if (config.options().set_table_options().type() !=
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

  opt.merge_operator.reset(new SetMergeOperator());

  std::string name =
      config.table_path() + config.options().table_name().name();
  auto s = rocksdb::DB::Open(opt, name, &db_);
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
