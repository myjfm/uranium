// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_META_TABLE_H_
#define URANIUM_DB_META_TABLE_H_

#include <memory>

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"
#include "schemaless_table.h"

namespace uranium {

class MetaTable final {
 public:
  MetaTable() {}
  virtual ~MetaTable() {}

  MetaTable(const MetaTable&) = delete;
  MetaTable& operator=(const MetaTable&) = delete;

  Status Init(const std::string& path) {
    auto s = rocksdb::DB::Open(rocksdb::Options(), path + "uranium.meta", &db_);
    if (!s.ok()) {
      return Status(s);
    }
    return Status::OK();
  }

  Status Close() {
    if (db_) {
      delete db_;
      db_ = nullptr;
    }
    return Status::OK();
  }

  Status LoadAllTables(std::vector<internal::TableOptions>* tables) {
    assert(db_);
    auto itr = std::unique_ptr<rocksdb::Iterator>(
        db_->NewIterator(rocksdb::ReadOptions()));
    for (itr->Seek(""); itr->Valid(); itr->Next()) {
      std::string table_name = itr->key().ToString();
      internal::TableOptions option;
      if (!option.ParseFromString(itr->value().ToString()) ||
          option.options().table_name().name() != table_name) {
        // TODO
        // log an error
        return Status::Corruption("meta db corrupts");
      }
      tables->push_back(std::move(option));
    }
    return Status::OK();
  }

 private:
  rocksdb::DB* db_ { nullptr };
};

}  // namespace uranium

#endif  // URANIUM_DB_META_TABLE_H_

