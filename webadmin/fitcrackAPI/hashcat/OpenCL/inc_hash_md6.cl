/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#include "inc_vendor.h"
#include "inc_types.h"
#include "inc_platform.h"
#include "inc_common.h"
#include "inc_hash_md6.h"

CONSTANT_VK u64 MD6_S0 = 0x0123456789abcdefUL;
CONSTANT_VK u64 MD6_Smask = 0x7311c2812425cfa0UL;

/* w == 64, standard version */

CONSTANT_VK u64 MD6_Q[15] =
{
  0x7311c2812425cfa0UL,
  0x6432286434aac8e7UL,
  0xb60450e9ef68b7c1UL,
  0xe8fb23908d9f06f1UL,
  0xdd2e76cba691e5bfUL,
  0x0cd0d63b2c30bc41UL,
  0x1f8ccf6823058f8aUL,
  0x54e5ed5b88e3775dUL,
  0x4ad12aae0a6d6031UL,
  0x3e7f16bb88222e0dUL,
  0x8af8671d3fb50c2cUL,
  0x995ad1178bd25c31UL,
  0xc878c1dd04c4b633UL,
  0x3b72066c7a1552acUL,
  0x0d6f3522631effcbUL
};

/* hc optimized values */

CONSTANT_VK u64 MD6_256_DEFAULT_NODEID = 0x0100000000000000UL; // ell = 1, i = 0
CONSTANT_VK u64 MD6_Vs = 0x0068401000000000UL;
CONSTANT_VK u64 MD6_Ve = 0x0000000000000100UL;
