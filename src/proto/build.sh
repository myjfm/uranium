#!/bin/sh

protoc -I . --cpp_out=../network/cpp common.proto
protoc -I . --cpp_out=../network/cpp uranium_internal.proto
protoc -I . --grpc_out=../network/cpp --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` uranium.proto
protoc -I . --cpp_out=../network/cpp uranium.proto
protoc -I . --grpc_out=../network/cpp --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` uranium_admin.proto
protoc -I . --cpp_out=../network/cpp uranium_admin.proto
