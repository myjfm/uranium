// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#include "db_path_info.h"

#include "common/status.h"
#include "common/string_utils.h"

namespace uranium {

Status DbPathInfo::Init(const std::string& db_paths) {
  auto db_paths_list = StringUtils::SplitByString(db_paths, ",");
  if (db_paths_list.size() < 1) {
    return Status::InvalidArgument("invalid db_paths");
  }
  for (auto& db_path : db_paths_list) {
    if (db_path.size() > 0) {
      if (db_path[db_path.size() - 1] != '/') {
        db_path += "/";
      }
      db_paths_.push_back(db_path);
      weighted_db_paths_[db_path] = 0;
    }
  }
  return Status::OK();
}

std::string DbPathInfo::GetLowestWeightPath() {
  std::string result;
  int weight = INT_MAX;
  for (auto& weighted_path : weighted_db_paths_) {
    if (weight > weighted_path.second) {
      result = weighted_path.first;
      weight = weighted_path.second;
    }
  }
  return result;
}

}  // namespace uranium

