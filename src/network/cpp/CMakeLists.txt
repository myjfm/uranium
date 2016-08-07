cmake_minimum_required(VERSION 2.8)
project(uranium C CXX)

set(NETWORK_FILES
  "common.pb.cc"
  "common.pb.h"
  "uranium.grpc.pb.cc"
  "uranium.grpc.pb.h"
  "uranium.pb.cc"
  "uranium.pb.h"
  "uranium_admin.grpc.pb.cc"
  "uranium_admin.grpc.pb.h"
  "uranium_admin.pb.cc"
  "uranium_admin.pb.h"
  "uranium_internal.pb.cc"
  "uranium_internal.pb.h")

add_library(network ${NETWORK_FILES})
set_target_properties(network PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(network
  ${SSL}
  ${CRYPTO}
  ${PROTOBUF}
  ${GRPCPP}
  ${GRPC}
  ${BZ2}
  ${LIBZ}
  ${LZ4}
  ${SNAPPY})

