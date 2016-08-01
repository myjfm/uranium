cmake_minimum_required(VERSION 2.8)
project(uranium C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_library(GFLAGS NAMES libgflags.a PATHS /usr/local/lib)
IF(NOT GFLAGS)
  MESSAGE(FATAL_ERROR "could not find gflags library")
ELSE()
  MESSAGE(STATUS "gflags library: " ${GFLAGS})
ENDIF()

find_library(PROTOBUF NAMES libprotobuf.a PATHS /usr/local/lib)
IF(NOT PROTOBUF)
  MESSAGE(FATAL_ERROR "could not find protobuf library")
ELSE()
  MESSAGE(STATUS "protobuf library: " ${PROTOBUF})
ENDIF()

find_library(GRPC NAMES libgrpc.a PATHS /usr/local/lib)
IF(NOT GRPC)
  MESSAGE(FATAL_ERROR "could not find grpc library")
ELSE()
  MESSAGE(STATUS "grpc library: " ${GRPC})
ENDIF()

find_library(GRPCPP NAMES libgrpc++.a PATHS /usr/local/lib)
IF(NOT GRPCPP)
  MESSAGE(FATAL_ERROR "could not find grpc++ library")
ELSE()
  MESSAGE(STATUS "grpc++ library: " ${GRPCPP})
ENDIF()

find_library(SSL NAMES libssl.a PATHS /usr/local/lib)
IF(NOT SSL)
  MESSAGE(FATAL_ERROR "could not find ssl library")
ELSE()
  MESSAGE(STATUS "ssl library: " ${SSL})
ENDIF()

find_library(CRYPTO NAMES libcrypto.a PATHS /usr/local/lib)
IF(NOT CRYPTO)
  MESSAGE(FATAL_ERROR "could not find crypto library")
ELSE()
  MESSAGE(STATUS "crypto library: " ${CRYPTO})
ENDIF()

find_library(LIBZ NAMES z PATHS /usr/local/lib)
IF(NOT LIBZ)
  MESSAGE(FATAL_ERROR "could not find libz library")
ELSE()
  MESSAGE(STATUS "libz library: " ${LIBZ})
ENDIF()

find_library(LZ4 NAMES lz4 PATHS /usr/local/lib)
IF(NOT LZ4)
  MESSAGE(FATAL_ERROR "could not find lz4 library")
ELSE()
  MESSAGE(STATUS "lz4 library: " ${LZ4})
ENDIF()

find_library(BZ2 NAMES bz2 PATHS /usr/local/lib)
IF(NOT BZ2)
  MESSAGE(FATAL_ERROR "could not find bz2 library")
ELSE()
  MESSAGE(STATUS "bz2 library: " ${BZ2})
ENDIF()

find_library(SNAPPY NAMES snappy PATHS /usr/local/lib)
IF(NOT SNAPPY)
  MESSAGE(FATAL_ERROR "could not find snappy library")
ELSE()
  MESSAGE(STATUS "snappy library: " ${SNAPPY})
ENDIF()

find_library(ROCKSDB NAMES rocksdb PATHS /usr/local/lib)
IF(NOT ROCKSDB)
  MESSAGE(FATAL_ERROR "could not find rocksdb library")
ELSE()
  MESSAGE(STATUS "rocksdb library: " ${ROCKSDB})
ENDIF()

#find_package(gflags REQUIRED)
#find_package(grpc REQUIRED)
#find_package(grpc++ REQUIRED)

INCLUDE_DIRECTORIES(AFTER "src")

set(SOURCE_FILES
  "src/common/string_piece.h"
  "src/common/string_piece.cc"
  "src/common/string_utils.h"
  "src/common/string_utils.cc"
  "src/common/status.h"
  "src/db/table.h"
  "src/db/db_path_info.h"
  "src/db/schemaless_table.h"
  "src/db/schema_table.h"
  "src/db/meta_table.h"
  "src/db/table_manager.h"
  "src/db/table_manager_impl.h"
  "src/network/cpp/common.pb.cc"
  "src/network/cpp/common.pb.h"
  "src/network/cpp/uranium.grpc.pb.cc"
  "src/network/cpp/uranium.grpc.pb.h"
  "src/network/cpp/uranium.pb.cc"
  "src/network/cpp/uranium.pb.h"
  "src/network/cpp/uranium_admin.grpc.pb.cc"
  "src/network/cpp/uranium_admin.grpc.pb.h"
  "src/network/cpp/uranium_admin.pb.cc"
  "src/network/cpp/uranium_admin.pb.h"
  "src/network/cpp/uranium_internal.pb.cc"
  "src/network/cpp/uranium_internal.pb.h"
  "src/server/uranium.cc"
  "src/server/uranium_schema_service_impl.h"
  "src/server/uranium_schemaless_service_impl.h"
  "src/server/uranium_admin_service_impl.h")

add_executable(uranium ${SOURCE_FILES})
set_target_properties(uranium PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(uranium ${SSL} ${CRYPTO} ${LIBZ} ${LZ4} ${SNAPPY} ${BZ2} ${GFLAGS} ${PROTOBUF} ${GRPC} ${GRPCPP} ${ROCKSDB})

