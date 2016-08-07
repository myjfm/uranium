#!/bin/sh

function build_gflags() {
  local CUR_PATH=`pwd`
  cd $CUR_PATH/gflags && \
    mkdir -p tmp_build && \
    cd tmp_build && \
    cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../output -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF .. && \
    cd $CUR_PATH && \
    return 0
  cd $CUR_PATH
  return 1
}

function build_bzip2() {
  local CUR_PATH=`pwd`
  cd $CUR_PATH/bzip2 && \
    mkdir -p build && \
    make install PREFIX=`pwd`/output && \
    cd $CUR_PATH && \
    return 0
  cd $CUR_PATH
  return 1
}

function build_lz4() {
  local CUR_PATH=`pwd`
  cd $CUR_PATH/lz4 && \
    mkdir -p build && \
    make && \
    make install PREFIX=`pwd`/output && \
    cd $CUR_PATH && \
    return 0
  cd $CUR_PATH
  return 1
}

function build_zlib() {
  local CUR_PATH=`pwd`
  cd $CUR_PATH/zlib && \
    mkdir -p build && \
    ./configure --prefix=`pwd`/output && \
    make && \
    make install && \
    cd $CUR_PATH && \
    return 0
  cd $CUR_PATH
  return 1
}

function build_snappy() {
  local CUR_PATH=`pwd`
  cd $CUR_PATH/snappy && \
    mkdir -p build && \
    ./autogen.sh && \
    ./configure --prefix=`pwd`/output && \
    make && \
    make install && \
    cd $CUR_PATH && \
    return 0
  cd $CUR_PATH
  return 1
}

function build_rocksdb() {
  local CUR_PATH=`pwd`
  cd $CUR_PATH/rocksdb && \
    mkdir -p build && \
    C_INCLUDE_PATH=$CUR_PATH/snappy/output/include:$CUR_PATH/zlib/output/include:$CUR_PATH/bzip2/output/include:$CUR_PATH/lz4/output/include:$C_INCLUDE_PATH CPLUS_INCLUDE_PATH=$CUR_PATH/snappy/output/include:$CUR_PATH/zlib/output/include:$CUR_PATH/bzip2/output/include:$CUR_PATH/lz4/output/include:$CPLUS_INCLUDE_PATH LD_LIBRARY_PATH=$CUR_PATH/snappy/output/lib:$CUR_PATH/zlib/output/lib:$CUR_PATH/bzip2/output/lib:$CUR_PATH/lz4/output/lib:$LD_LIBRARY_PATH make static_lib && make install INSTALL_PATH=`pwd`/output && \
    cd $CUR_PATH && \
    return 0
  cd $CUR_PATH
  return 1
}

function build_protobuf() {
  local CUR_PATH=`pwd`
  cd $CUR_PATH/protobuf && \
    mkdir -p output && \
    sh -x autogen.sh && \
    ./configure --prefix=`pwd`/output && \
    C_INCLUDE_PATH=$CUR_PATH/zlib/output/include:$C_INCLUDE_PATH CPLUS_INCLUDE_PATH=$CUR_PATH/zlib/output/include:$CPLUS_INCLUDE_PATH LD_LIBRARY_PATH=$CUR_PATH/zlib/output/lib:$LD_LIBRARY_PATH make && \
    make install && \
    cd $CUR_PATH && \
    return 0
  cd $CUR_PATH
  return 1
}

function build_grpc() {
  local CUR_PATH=`pwd`
  cd $CUR_PATH/grpc && \
    git submodule update --init && \
    mkdir -p output && \
    make static HAS_PKG_CONFIG=false PROTOC=$CUR_PATH/protobuf/output/bin/protoc CFLAGS_opt="-I$CUR_PATH/protobuf/output/include -I$CUR_PATH/zlib/output/include" CPPFLAGS_opt="-I$CUR_PATH/protobuf/output/include -I$CUR_PATH/zlib/output/include" LDFLAGS_opt="-L$CUR_PATH/protobuf/output/lib -L$CUR_PATH/zlib/output/lib" && \
    make install prefix=`pwd`/output HAS_PKG_CONFIG=false PROTOC=$CUR_PATH/protobuf/output/bin/protoc CFLAGS_opt="-I$CUR_PATH/protobuf/output/include -I$CUR_PATH/zlib/output/include" CPPFLAGS_opt="-I$CUR_PATH/protobuf/output/include -I$CUR_PATH/zlib/output/include" LDFLAGS_opt="-L$CUR_PATH/protobuf/output/lib -L$CUR_PATH/zlib/output/lib" && \
    cd $CUR_PATH && \
    return 0
  cd $CUR_PATH
  return 1
}

echo 'start building third parties...' && \
  build_gflags && \
  build_bzip2 && \
  build_lz4 && \
  build_zlib && \
  build_snappy && \
  build_rocksdb && \
  build_protobuf && \
  build_grpc && \
  exit 0
exit 1

