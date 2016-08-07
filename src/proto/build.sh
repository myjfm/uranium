#!/bin/sh

# Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#
protoc -I . --cpp_out=../network/cpp common.proto
protoc -I . --cpp_out=../network/cpp uranium_internal.proto
protoc -I . --grpc_out=../network/cpp --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` uranium.proto
protoc -I . --cpp_out=../network/cpp uranium.proto
protoc -I . --grpc_out=../network/cpp --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` uranium_admin.proto
protoc -I . --cpp_out=../network/cpp uranium_admin.proto
