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

namespace uranium {

class DbPathInfo {
 public:
  DbPathInfo() = default;
  ~DbPathInfo() = default;

  Status Init(const std::string& db_paths);

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

  std::string GetLowestWeightPath();

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

