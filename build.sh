#!/bin/sh

CUR_PATH=`pwd`
cd third_party
sh -x build.sh && \
  cd $CUR_PATH && \
  mkdir -p build && \
  mkdir -p output && \
  cd build && \
  cmake -DCMAKE_INSTALL_PREFIX=$CUR_PATH/output .. && \
  make -j 4 && \
  make install && \
  exit 0

exit 1

