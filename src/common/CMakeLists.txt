# Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
cmake_minimum_required(VERSION 2.8)
project(uranium C CXX)

set(COMMON_FILES
  "coding.h"
  "coding.cc"
  "endian.h"
  "string_piece.h"
  "string_piece.cc"
  "string_utils.h"
  "string_utils.cc"
  "status.h")

add_library(common ${COMMON_FILES})
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(common)

