// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
//
#ifndef URANIUM_DB_PATH_INFO_H_
#define URANIUM_DB_PATH_INFO_H_

#include <map>
#include <string>
#include <vector>

#include "common/status.h"
#include "common/string_utils.h"

namespace uranium {

class DbPathInfo {
 public:
  DbPathInfo() = default;
  ~DbPathInfo() = default;

  Status Init(const std::string& db_paths) {
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

  size_t GetDbPathNumber() {
    return db_paths_.size();
  }

  const std::string& GetDbPath(size_t index) {
    assert(index < db_paths_.size());
    return db_paths_[index];
  }

  const std::vector<std::string>& GetAllDbPaths() {
    return db_paths_;
  }

  std::string GetLowestWeightPath() {
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

  void IncreasePathWeight(const std::string& path) {
    weighted_db_paths_[path]++;
  }

  bool PathExists(const std::string& path) {
    return (weighted_db_paths_.find(path) != weighted_db_paths_.end());
  }

 private:
  std::vector<std::string> db_paths_;
  std::map<std::string, int> weighted_db_paths_;
};

}  // namespace uranium

#endif  // URANIUM_DB_PATH_INFO_H_

