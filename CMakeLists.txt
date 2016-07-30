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

#find_package(gflags REQUIRED)
#find_package(grpc REQUIRED)
#find_package(grpc++ REQUIRED)

INCLUDE_DIRECTORIES(AFTER "src")

set(SOURCE_FILES
  "src/common/slice.h"
  "src/common/status.h"
  "src/db/table.h"
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
  "src/server/uranium.cc"
  "src/server/uranium_schema_service_impl.h"
  "src/server/uranium_schemaless_service_impl.h"
  "src/server/uranium_admin_service_impl.h")

add_executable(uranium ${SOURCE_FILES})
set_target_properties(uranium PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(uranium ${SSL} ${CRYPTO} ${LIBZ} ${GFLAGS} ${PROTOBUF} ${GRPC} ${GRPCPP})

