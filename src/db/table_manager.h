// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_TABLE_MANAGER_H_
#define URANIUM_DB_TABLE_MANAGER_H_

#include "common/status.h"

namespace uranium {

class TableManager {
 public:
  TableManager() {}
  virtual ~TableManager() {}

  TableManager(const TableManager&) = delete;
  TableManager& operator=(const TableManager&) = delete;

  Status Init(const std::string& db_path) {
    (void) db_path;
    return Status::OK();
  }
};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_MANAGER_H_

