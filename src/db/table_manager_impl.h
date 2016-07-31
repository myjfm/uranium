// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_TABLE_MANAGER_IMPL_H_
#define URANIUM_DB_TABLE_MANAGER_IMPL_H_

#include "common/status.h"
#include "table_manager.h"

namespace uranium {

class TableManagerImpl : public TableManager {
 public:
  TableManagerImpl() {}
  virtual ~TableManagerImpl() {}

  TableManagerImpl(const TableManagerImpl&) = delete;
  TableManagerImpl& operator=(const TableManagerImpl&) = delete;

  virtual Status Init(const std::string& db_paths) override {
    (void) db_paths;
    return Status::OK();
  }
  virtual Status Close() override {
    return Status::OK();
  }
  virtual std::shared_ptr<SchemalessTable> GetSchemalessTable(
      const std::string& name) override {
    return std::shared_ptr<SchemalessTable>(nullptr);
  }
  virtual std::shared_ptr<SchemaTable> GetSchemaTable(
      const std::string& name) override {
    return std::shared_ptr<SchemaTable>(nullptr);
  }

 private:
  std::map<std::string, std::shared_ptr<SchemalessTable>> schemaless_tables_;
  std::map<std::string, std::shared_ptr<SchemaTable>> schema_tables_;
};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_MANAGER_IMPL_H_

