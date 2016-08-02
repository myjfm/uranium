// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_TABLE_MANAGER_H_
#define URANIUM_DB_TABLE_MANAGER_H_

#include <memory>

#include "common/status.h"
#include "schema_table.h"
#include "schemaless_table.h"

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

  virtual std::shared_ptr<SchemalessTable> GetSchemalessTable(
      const std::string& name) = 0;
  virtual std::shared_ptr<SchemaTable> GetSchemaTable(
      const std::string& name) = 0;

  virtual Status CreateTable(const admin::TableOptions& options) = 0;
  virtual Status UpdateTable(const admin::TableOptions& options) = 0;
  virtual Status DropTable(const std::string& table_name) = 0;
  virtual Status GetSchemalessTableOptions(const std::string& table_name,
                                           admin::TableOptions* options) = 0;
  virtual Status GetSchemaTableOptions(const std::string& table_name,
                                       admin::TableOptions* options) = 0;
};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_MANAGER_H_

