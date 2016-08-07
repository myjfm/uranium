cmake_minimum_required(VERSION 2.8)
project(uranium C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread -g")
set(CMAKE_FIND_LIBRARY_SUFFIXES .a${CMAKE_FIND_LIBRARY_SUFFIXES})

find_library(GFLAGS NAMES libgflags.a gflags PATHS third_party/gflags/output/lib NO_DEFAULT_PATH)
IF(NOT GFLAGS)
  MESSAGE(FATAL_ERROR "could not find gflags library")
ELSE()
  MESSAGE(STATUS "gflags library: " ${GFLAGS})
ENDIF()

find_library(PROTOBUF NAMES libprotobuf.a protobuf PATHS third_party/protobuf/output/lib NO_DEFAULT_PATH)
IF(NOT PROTOBUF)
  MESSAGE(FATAL_ERROR "could not find protobuf library")
ELSE()
  MESSAGE(STATUS "protobuf library: " ${PROTOBUF})
ENDIF()

find_library(GRPC NAMES libgrpc.a grpc PATHS third_party/grpc/output/lib NO_DEFAULT_PATH)
IF(NOT GRPC)
  MESSAGE(FATAL_ERROR "could not find grpc library")
ELSE()
  MESSAGE(STATUS "grpc library: " ${GRPC})
ENDIF()

find_library(GRPCPP NAMES libgrpc++.a grpc++ PATHS third_party/grpc/output/lib NO_DEFAULT_PATH)
IF(NOT GRPCPP)
  MESSAGE(FATAL_ERROR "could not find grpc++ library")
ELSE()
  MESSAGE(STATUS "grpc++ library: " ${GRPCPP})
ENDIF()

#find_library(SSL NAMES libssl.a ssl PATHS third_party/grpc/output/lib NO_DEFAULT_PATH)
#IF(NOT SSL)
#  MESSAGE(FATAL_ERROR "could not find ssl library")
#ELSE()
#  MESSAGE(STATUS "ssl library: " ${SSL})
#ENDIF()

#find_library(CRYPTO NAMES libcrypto.a crypto PATHS third_party/grpc/output/lib NO_DEFAULT_PATH)
#IF(NOT CRYPTO)
#  MESSAGE(FATAL_ERROR "could not find crypto library")
#ELSE()
#  MESSAGE(STATUS "crypto library: " ${CRYPTO})
#ENDIF()

find_library(LIBZ NAMES libz.a z PATHS third_party/zlib/output/lib NO_DEFAULT_PATH)
IF(NOT LIBZ)
  MESSAGE(FATAL_ERROR "could not find libz library")
ELSE()
  MESSAGE(STATUS "libz library: " ${LIBZ})
ENDIF()

find_library(LZ4 NAMES liblz4.a lz4 PATHS third_party/lz4/output/lib NO_DEFAULT_PATH)
IF(NOT LZ4)
  MESSAGE(FATAL_ERROR "could not find lz4 library")
ELSE()
  MESSAGE(STATUS "lz4 library: " ${LZ4})
ENDIF()

find_library(BZ2 NAMES libbz2.a bz2 PATHS third_party/bzip2/output/lib NO_DEFAULT_PATH)
IF(NOT BZ2)
  MESSAGE(FATAL_ERROR "could not find bz2 library")
ELSE()
  MESSAGE(STATUS "bz2 library: " ${BZ2})
ENDIF()

find_library(SNAPPY NAMES libsnappy.a snappy PATHS third_party/snappy/output/lib NO_DEFAULT_PATH)
IF(NOT SNAPPY)
  MESSAGE(FATAL_ERROR "could not find snappy library")
ELSE()
  MESSAGE(STATUS "snappy library: " ${SNAPPY})
ENDIF()

find_library(ROCKSDB NAMES librocksdb.a rocksdb PATHS third_party/rocksdb/output/lib NO_DEFAULT_PATH)
IF(NOT ROCKSDB)
  MESSAGE(FATAL_ERROR "could not find rocksdb library")
ELSE()
  MESSAGE(STATUS "rocksdb library: " ${ROCKSDB})
ENDIF()

INCLUDE_DIRECTORIES(AFTER
  "src"
  "third_party/grpc/output/include"
  "third_party/protobuf/output/include"
  "third_party/rocksdb/output/include"
  "third_party/zlib/output/include"
  "third_party/snappy/output/include"
  "third_party/bzip2/output/include"
  "third_party/lz4/output/include"
  "third_party/gflags/output/include")

add_subdirectory(src)

add_subdirectory(examples)

