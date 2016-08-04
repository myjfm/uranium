//
// Created by a on 16/8/4.
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

