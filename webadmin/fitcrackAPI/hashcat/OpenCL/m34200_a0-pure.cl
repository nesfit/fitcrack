/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

//#define NEW_SIMD_CODE

#ifdef KERNEL_STATIC
#include M2S(INCLUDE_PATH/inc_vendor.h)
#include M2S(INCLUDE_PATH/inc_types.h)
#include M2S(INCLUDE_PATH/inc_platform.cl)
#include M2S(INCLUDE_PATH/inc_common.cl)
#include M2S(INCLUDE_PATH/inc_rp.h)
#include M2S(INCLUDE_PATH/inc_rp.cl)
#include M2S(INCLUDE_PATH/inc_scalar.cl)
#endif

DECLSPEC u64 MurmurHash64A (const u64 seed, PRIVATE_AS const u32 *data, const u32 len)
{
#define M 0xc6a4a7935bd1e995
#define R 47

  // Initialize hash
  u64 hash = seed ^ (len * M);

  // Twice the number of u64 blocks
  const u32 num_u32_blocks = (len / 8) * 2;

  // Loop over one u64 at a time
  u32 i = 0;
  while (i < num_u32_blocks)
  {
    // Reconstruct u64 from two u32s
    u64 k = hl32_to_64 (data[i + 1], data[i]);

    k *= M;
    k ^= k >> R;
    k *= M;

    hash ^= k;
    hash *= M;

    i += 2;
  }

  // Up to 7 overflow bytes
  const u32 overflow = len & 7;

  if (overflow > 4)
  {
    hash ^= hl32_to_64 (data[i + 1], data[i]);
    hash *= M;
  }
  else if (overflow > 0)
  {
    hash ^= hl32_to_64 (0, data[i]);
    hash *= M;
  }

  hash ^= hash >> R;
  hash *= M;
  hash ^= hash >> R;

#undef M
#undef R

  return hash;
}

KERNEL_FQ KERNEL_FA void m34200_mxx (KERN_ATTR_RULES ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);

  /**
   * base
   */

  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  COPY_PW (pws[gid]);

  /**
   * salt
   */

  // Reconstruct seed from two u32s
  const u32 seed_lo = salt_bufs[SALT_POS_HOST].salt_buf[0];
  const u32 seed_hi = salt_bufs[SALT_POS_HOST].salt_buf[1];
  const u64 seed = hl32_to_64 (seed_hi, seed_lo);

  /**
   * loop
   */

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos++)
  {
    pw_t tmp = PASTE_PW;

    tmp.pw_len = apply_rules (rules_buf[il_pos].cmds, tmp.i, tmp.pw_len);

    u64x hash = MurmurHash64A (seed, tmp.i, tmp.pw_len);

    const u32x r0 = l32_from_64 (hash);
    const u32x r1 = h32_from_64 (hash);
    const u32x z = 0;

    COMPARE_M_SCALAR (r0, r1, z, z);
  }
}

KERNEL_FQ KERNEL_FA void m34200_sxx (KERN_ATTR_RULES ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);

  /**
   * base
   */

  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  /**
   * salt
   */

  // Reconstruct seed from two u32s
  const u32 seed_lo = salt_bufs[SALT_POS_HOST].salt_buf[0];
  const u32 seed_hi = salt_bufs[SALT_POS_HOST].salt_buf[1];
  const u64 seed = hl32_to_64 (seed_hi, seed_lo);

  /**
   * digest
   */

  const u32 search[4] =
  {
    digests_buf[DIGESTS_OFFSET_HOST].digest_buf[DGST_R0],
    digests_buf[DIGESTS_OFFSET_HOST].digest_buf[DGST_R1],
    0,
    0
  };

  /**
   * base
   */

  COPY_PW (pws[gid]);

  /**
   * loop
   */

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos++)
  {
    pw_t tmp = PASTE_PW;

    tmp.pw_len = apply_rules (rules_buf[il_pos].cmds, tmp.i, tmp.pw_len);

    u64x hash = MurmurHash64A (seed, tmp.i, tmp.pw_len);

    const u32 r0 = l32_from_64 (hash);
    const u32 r1 = h32_from_64 (hash);
    const u32 z = 0;

    COMPARE_S_SCALAR (r0, r1, z, z);
  }
}
