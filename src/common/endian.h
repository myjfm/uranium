// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.
//
// Copyright (c) 2013, Facebook, Inc.  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
#ifndef URANIUM_COMMON_ENDIAN_H_
#define URANIUM_COMMON_ENDIAN_H_

enum class Endian {
  kLittle = 0x03020100ul,
  kBig = 0x00010203ul,
};

static const union {
  unsigned char bytes[4];
  uint32_t value;
} host_order = { { 0, 1, 2, 3 } };

#define HOST_ORDER (host_order.value)
#define IS_LITTLE_ENDIAN (HOST_ORDER == static_cast<uint32_t>(Endian::kLittle))

#endif //URANIUM_COMMON_ENDIAN_H_

