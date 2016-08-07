// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#include "kv_table.h"

#include <rocksdb/cache.h>
#include <rocksdb/filter_policy.h>
#include <rocksdb/memtablerep.h>
#include <rocksdb/slice_transform.h>
#include <rocksdb/table.h>
#include <rocksdb/write_batch.h>

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

Status KVTable::Init(const internal::TableOptions& config) {
  assert(config.options().table_type() == common::TableType::KV);
  assert(config.options().has_kv_table_options());
  if (config.options().kv_table_options().type() !=
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

  std::string name =
      config.table_path() + config.options().table_name().name();
  auto s = rocksdb::DB::Open(opt, name, &db_);
  if (!s.ok()) {
    return Status(s);
  }

  options_.CopyFrom(config);
  return Status::OK();
}

Status KVTable::Get(
    const google::protobuf::RepeatedPtrField<api::Key>& keys,
    google::protobuf::RepeatedPtrField<api::KeyValue>* values) {
  assert(db_);
  std::vector<rocksdb::Slice> rocks_keys;
  rocks_keys.reserve(keys.size());
  for (auto itr = keys.begin(); itr != keys.end(); ++itr) {
    rocks_keys.push_back(itr->key());
  }
  std::vector<std::string> rocks_values(keys.size());
  auto rss = db_->MultiGet(rocksdb::ReadOptions(), rocks_keys, &rocks_values);
  for (int i = 0; i < rss.size(); ++i) {
    if (rss[i].ok()) {
      auto kv = values->Add();
      kv->mutable_key()->set_key(keys.Get(i).key());
      kv->mutable_value()->set_value(rocks_values[i]);
    }
  }
  return Status::OK();
}

Status KVTable::Set(
    const google::protobuf::RepeatedPtrField<api::KeyValue>& kvs) {
  assert(db_);
  rocksdb::WriteBatch wb;
  for (auto itr = kvs.begin(); itr != kvs.end(); ++itr) {
    wb.Put(itr->key().key(), itr->value().value());
  }
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  if (!rs.ok()) {
    return Status(rs);
  }
  return Status::OK();
}

Status KVTable::Remove(
    const google::protobuf::RepeatedPtrField<api::Key>& keys) {
  assert(db_);
  rocksdb::WriteBatch wb;
  for (auto itr = keys.begin(); itr != keys.end(); ++itr) {
    wb.Delete(itr->key());
  }
  auto rs = db_->Write(rocksdb::WriteOptions(), &wb);
  if (!rs.ok()) {
    return Status(rs);
  }
  return Status::OK();
}

}  // namespace uranium
