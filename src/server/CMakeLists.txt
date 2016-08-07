# Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
cmake_minimum_required(VERSION 2.8)
project(uranium C CXX)

set(SERVER_FILES
  "uranium.cc"
  "uranium_kv_service_impl.h"
  "uranium_kv_service_impl.cc"
  "uranium_list_service_impl.h"
  "uranium_list_service_impl.cc"
  "uranium_hash_service_impl.h"
  "uranium_hash_service_impl.cc"
  "uranium_set_service_impl.h"
  "uranium_set_service_impl.cc"
  "uranium_schema_service_impl.h"
  "uranium_schema_service_impl.cc"
  "uranium_admin_service_impl.h"
  "uranium_admin_service_impl.cc")

add_executable(uranium ${SERVER_FILES})
set_target_properties(uranium PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(uranium ${GFLAGS} network db common)

