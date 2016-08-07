# Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
cmake_minimum_required(VERSION 2.8)
project(uranium C CXX)

add_subdirectory(network/cpp)
add_subdirectory(common)
add_subdirectory(db)
add_subdirectory(server)
