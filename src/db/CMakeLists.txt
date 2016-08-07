# Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
cmake_minimum_required(VERSION 2.8)
project(uranium C CXX)

set(DB_FILES
  "table.h"
  "db_path_info.h"
  "db_path_info.cc"
  "kv_table.h"
  "kv_table.cc"
  "list_table.h"
  "list_table.cc"
  "hash_table.h"
  "hash_table.cc"
  "set_table.h"
  "set_table.cc"
  "schema_table.h"
  "schema_table.cc"
  "meta_table.h"
  "meta_table.cc"
  "table_manager.h"
  "table_manager_impl.h"
  "table_manager_impl.cc")

add_library(db ${DB_FILES})
set_target_properties(db PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(db
  ${PROTOBUF}
  ${GRPCPP}
  ${GRPC}
  ${ROCKSDB}
  ${BZ2}
  ${LIBZ}
  ${LZ4}
  ${SNAPPY})

