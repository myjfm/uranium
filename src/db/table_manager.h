// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
#ifndef URANIUM_DB_TABLE_MANAGER_H_
#define URANIUM_DB_TABLE_MANAGER_H_

#include <memory>

#include "common/status.h"
#include "schema_table.h"
#include "kv_table.h"
#include "hash_table.h"
#include "list_table.h"
#include "set_table.h"

namespace uranium {

class TableManager {
 public:
  TableManager() {}
  virtual ~TableManager() {}

  TableManager(const TableManager&) = delete;
  TableManager& operator=(const TableManager&) = delete;

  virtual Status Init(const std::string& db_paths) = 0;

  // There should have no thread accessing tables before you call this method.
  virtual Status Close() = 0;

  virtual std::shared_ptr<KVTable> GetKVTable(const std::string& name) = 0;
  virtual std::shared_ptr<ListTable> GetListTable(const std::string& name) = 0;
  virtual std::shared_ptr<HashTable> GetHashTable(const std::string& name) = 0;
  virtual std::shared_ptr<SetTable> GetSetTable(const std::string& name) = 0;
  virtual std::shared_ptr<SchemaTable> GetSchemaTable(
      const std::string& name) = 0;

  virtual Status CreateKVTable(const std::string& table_name,
                               const admin::CommonTableOptions& common_options,
                               const admin::KVTableOptions& options) = 0;
  virtual Status CreateListTable(
      const std::string& table_name,
      const admin::CommonTableOptions& common_options,
      const admin::ListTableOptions& options) = 0;
  virtual Status CreateHashTable(
      const std::string& table_name,
      const admin::CommonTableOptions& common_options,
      const admin::HashTableOptions& options) = 0;
  virtual Status CreateSetTable(const std::string& table_name,
                                const admin::CommonTableOptions& common_options,
                                const admin::SetTableOptions& options) = 0;
  virtual Status CreateSchemaTable(
      const std::string& table_name,
      const admin::CommonTableOptions& common_options,
      const admin::SchemaTableOptions& options) = 0;

  virtual Status UpdateKVTable(const std::string& table_name,
                               const admin::CommonTableOptions& common_options,
                               const admin::KVTableOptions& options) = 0;
  virtual Status UpdateListTable(
      const std::string& table_name,
      const admin::CommonTableOptions& common_options,
      const admin::ListTableOptions& options) = 0;
  virtual Status UpdateHashTable(
      const std::string& table_name,
      const admin::CommonTableOptions& common_options,
      const admin::HashTableOptions& options) = 0;
  virtual Status UpdateSetTable(const std::string& table_name,
                                const admin::CommonTableOptions& common_options,
                                const admin::SetTableOptions& options) = 0;
  virtual Status UpdateSchemaTable(
      const std::string& table_name,
      const admin::CommonTableOptions& common_options,
      const admin::SchemaTableOptions& options) = 0;

  virtual Status DropKVTable(const std::string& table_name) = 0;
  virtual Status DropListTable(const std::string& table_name) = 0;
  virtual Status DropHashTable(const std::string& table_name) = 0;
  virtual Status DropSetTable(const std::string& table_name) = 0;
  virtual Status DropSchemaTable(const std::string& table_name) = 0;

  virtual Status GetKVTableOptions(const std::string& table_name,
                                   admin::TableOptions* options) = 0;
  virtual Status GetListTableOptions(const std::string& table_name,
                                     admin::TableOptions* options) = 0;
  virtual Status GetHashTableOptions(const std::string& table_name,
                                     admin::TableOptions* options) = 0;
  virtual Status GetSetTableOptions(const std::string& table_name,
                                    admin::TableOptions* options) = 0;
  virtual Status GetSchemaTableOptions(const std::string& table_name,
                                       admin::TableOptions* options) = 0;
};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_MANAGER_H_

