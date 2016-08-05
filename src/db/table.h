// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#ifndef URANIUM_DB_TABLE_H_
#define URANIUM_DB_TABLE_H_

#include "common/status.h"
#include "network/cpp/uranium_internal.pb.h"

namespace uranium {

class Table {
 public:
  Table() {}
  virtual ~Table() {}

  Table(const Table&) = delete;
  Table& operator=(const Table&) = delete;

  virtual Status Init(const internal::TableOptions& config) = 0;
  virtual Status Close() = 0;

};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_H_

