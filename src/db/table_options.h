// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_TABLE_OPTIONS_H_
#define URANIUM_DB_TABLE_OPTIONS_H_

#include <string>

namespace uranium {

struct TableOptions {
 public:
  std::string path;
  std::string name;
};

}  // namespace uranium

#endif  // URANIUM_DB_TABLE_OPTIONS_H_

