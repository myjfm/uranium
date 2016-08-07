// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#ifndef URANIUM_DB_META_TABLE_H_
#define URANIUM_DB_META_TABLE_H_

#include <memory>
#include <vector>

#include <rocksdb/db.h>

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"

namespace uranium {

// This is a light-weighted RocksDB instance, which is used to stores all
// meta data of uranium.
// NOTE:
// This class is not thread safe, needs external lock protected when accessing
// by multiple threads.
class MetaTable final {
 public:
  MetaTable() {}
  virtual ~MetaTable() {}

  MetaTable(const MetaTable&) = delete;
  MetaTable& operator=(const MetaTable&) = delete;

  Status Init(const std::string& path);

  Status Close() {
    if (db_) {
      delete db_;
      db_ = nullptr;
    }
    return Status::OK();
  }

  Status LoadAllTables(std::vector<internal::TableOptions>* tables);

  Status CreateKVTable(const std::string& table_path,
                       const std::string& table_name,
                       const admin::CommonTableOptions& common_options,
                       const admin::KVTableOptions& table_options);
  Status CreateListTable(const std::string& table_path,
                         const std::string& table_name,
                         const admin::CommonTableOptions& common_options,
                         const admin::ListTableOptions& table_options);
  Status CreateHashTable(const std::string& table_path,
                         const std::string& table_name,
                         const admin::CommonTableOptions& common_options,
                         const admin::HashTableOptions& table_options);
  Status CreateSetTable(const std::string& table_path,
                        const std::string& table_name,
                        const admin::CommonTableOptions& common_options,
                        const admin::SetTableOptions& table_options);
  Status CreateSchemaTable(const std::string& table_path,
                           const std::string& table_name,
                           const admin::CommonTableOptions& common_options,
                           const admin::SchemaTableOptions& table);

  Status GetKVTableOptions(const std::string& table_name,
                           admin::CommonTableOptions* common_options,
                           admin::KVTableOptions* options);
  Status GetListTableOptions(const std::string& table_name,
                             admin::CommonTableOptions* common_options,
                             admin::ListTableOptions* options);
  Status GetHashTableOptions(const std::string& table_name,
                             admin::CommonTableOptions* common_options,
                             admin::HashTableOptions* options);
  Status GetSetTableOptions(const std::string& table_name,
                            admin::CommonTableOptions* common_options,
                            admin::SetTableOptions* options);
  Status GetSchemaTableOptions(const std::string& table_name,
                               admin::CommonTableOptions* common_options,
                               admin::SchemaTableOptions* options);

 private:
  Status LoadKVTables(std::vector<internal::TableOptions>* tables);
  Status LoadListTables(std::vector<internal::TableOptions>* tables);
  Status LoadHashTables(std::vector<internal::TableOptions>* tables);
  Status LoadSetTables(std::vector<internal::TableOptions>* tables);
  Status LoadSchemaTables(std::vector<internal::TableOptions>* tables);

  rocksdb::DB* db_ { nullptr };

  internal::TableOptionsList kv_table_options_list_;
  internal::TableOptionsList list_table_options_list_;
  internal::TableOptionsList hash_table_options_list_;
  internal::TableOptionsList set_table_options_list_;
  internal::TableOptionsList schema_table_options_list_;
};

}  // namespace uranium

#endif  // URANIUM_DB_META_TABLE_H_

