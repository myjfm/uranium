// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_TABLE_H_
#define URANIUM_DB_TABLE_H_

#include "common/status.h"

namespace uranium {

class Table {
 public:
  Table() {}
  virtual ~Table() {}

  Table(const Table&) = delete;
  Table& operator=(const Table&) = delete;

  class TableOptions;
  virtual Status Init(const TableOptions& config) = 0;

};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_H_

