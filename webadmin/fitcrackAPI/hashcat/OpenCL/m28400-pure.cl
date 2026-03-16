/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#ifdef KERNEL_STATIC
#include M2S(INCLUDE_PATH/inc_vendor.h)
#include M2S(INCLUDE_PATH/inc_types.h)
#include M2S(INCLUDE_PATH/inc_platform.cl)
#include M2S(INCLUDE_PATH/inc_common.cl)
#include M2S(INCLUDE_PATH/inc_cipher_blowfish.cl)
#include M2S(INCLUDE_PATH/inc_hash_sha512.cl)
#endif

#if   VECT_SIZE == 1
#define uint_to_hex_lower8(i) make_u32x (u16_bin_to_u32_hex_msn ((i)))
#elif VECT_SIZE == 2
#define uint_to_hex_lower8(i) make_u32x (u16_bin_to_u32_hex_msn ((i).s0), u16_bin_to_u32_hex_msn ((i).s1))
#elif VECT_SIZE == 4
#define uint_to_hex_lower8(i) make_u32x (u16_bin_to_u32_hex_msn ((i).s0), u16_bin_to_u32_hex_msn ((i).s1), u16_bin_to_u32_hex_msn ((i).s2), u16_bin_to_u32_hex_msn ((i).s3))
#elif VECT_SIZE == 8
#define uint_to_hex_lower8(i) make_u32x (u16_bin_to_u32_hex_msn ((i).s0), u16_bin_to_u32_hex_msn ((i).s1), u16_bin_to_u32_hex_msn ((i).s2), u16_bin_to_u32_hex_msn ((i).s3), u16_bin_to_u32_hex_msn ((i).s4), u16_bin_to_u32_hex_msn ((i).s5), u16_bin_to_u32_hex_msn ((i).s6), u16_bin_to_u32_hex_msn ((i).s7))
#elif VECT_SIZE == 16
#define uint_to_hex_lower8(i) make_u32x (u16_bin_to_u32_hex_msn ((i).s0), u16_bin_to_u32_hex_msn ((i).s1), u16_bin_to_u32_hex_msn ((i).s2), u16_bin_to_u32_hex_msn ((i).s3), u16_bin_to_u32_hex_msn ((i).s4), u16_bin_to_u32_hex_msn ((i).s5), u16_bin_to_u32_hex_msn ((i).s6), u16_bin_to_u32_hex_msn ((i).s7), u16_bin_to_u32_hex_msn ((i).s8), u16_bin_to_u32_hex_msn ((i).s9), u16_bin_to_u32_hex_msn ((i).sa), u16_bin_to_u32_hex_msn ((i).sb), u16_bin_to_u32_hex_msn ((i).sc), u16_bin_to_u32_hex_msn ((i).sd), u16_bin_to_u32_hex_msn ((i).se), u16_bin_to_u32_hex_msn ((i).sf))
#endif

#define COMPARE_S M2S(INCLUDE_PATH/inc_comp_single.cl)
#define COMPARE_M M2S(INCLUDE_PATH/inc_comp_multi.cl)

typedef struct bcrypt_tmp
{
  u32 E[18];

  u32 P[18];

  u32 S0[256];
  u32 S1[256];
  u32 S2[256];
  u32 S3[256];

} bcrypt_tmp_t;

KERNEL_FQ KERNEL_FA void m28400_init (KERN_ATTR_TMPS (bcrypt_tmp_t))
{
  /**
   * base
   */

  const u64 gid = get_global_id (0);
  const u64 lid = get_local_id (0);

  if (gid >= GID_CNT) return;

  sha512_ctx_t ctx0;

  sha512_init (&ctx0);

  sha512_update_global_swap (&ctx0, pws[gid].i, pws[gid].pw_len);

  sha512_final (&ctx0);

  const u64 a = ctx0.h[0];
  const u64 b = ctx0.h[1];
  const u64 c = ctx0.h[2];
  const u64 d = ctx0.h[3];
  const u64 e = ctx0.h[4];
  const u64 f = ctx0.h[5];
  const u64 g = ctx0.h[6];
  const u64 h = ctx0.h[7];

  u32 E[18] = { 0 };

  E[ 0] = uint_to_hex_lower8 ((a >> 56) & 255) << 16
        | uint_to_hex_lower8 ((a >> 48) & 255) <<  0;
  E[ 1] = uint_to_hex_lower8 ((a >> 40) & 255) << 16
        | uint_to_hex_lower8 ((a >> 32) & 255) <<  0;
  E[ 2] = uint_to_hex_lower8 ((a >> 24) & 255) << 16
        | uint_to_hex_lower8 ((a >> 16) & 255) <<  0;
  E[ 3] = uint_to_hex_lower8 ((a >>  8) & 255) << 16
        | uint_to_hex_lower8 ((a >>  0) & 255) <<  0;

  E[ 4] = uint_to_hex_lower8 ((b >> 56) & 255) << 16
        | uint_to_hex_lower8 ((b >> 48) & 255) <<  0;
  E[ 5] = uint_to_hex_lower8 ((b >> 40) & 255) << 16
        | uint_to_hex_lower8 ((b >> 32) & 255) <<  0;
  E[ 6] = uint_to_hex_lower8 ((b >> 24) & 255) << 16
        | uint_to_hex_lower8 ((b >> 16) & 255) <<  0;
  E[ 7] = uint_to_hex_lower8 ((b >>  8) & 255) << 16
        | uint_to_hex_lower8 ((b >>  0) & 255) <<  0;

  E[ 8] = uint_to_hex_lower8 ((c >> 56) & 255) << 16
        | uint_to_hex_lower8 ((c >> 48) & 255) <<  0;
  E[ 9] = uint_to_hex_lower8 ((c >> 40) & 255) << 16
        | uint_to_hex_lower8 ((c >> 32) & 255) <<  0;
  E[10] = uint_to_hex_lower8 ((c >> 24) & 255) << 16
        | uint_to_hex_lower8 ((c >> 16) & 255) <<  0;
  E[11] = uint_to_hex_lower8 ((c >>  8) & 255) << 16
        | uint_to_hex_lower8 ((c >>  0) & 255) <<  0;

  E[12] = uint_to_hex_lower8 ((d >> 56) & 255) << 16
        | uint_to_hex_lower8 ((d >> 48) & 255) <<  0;
  E[13] = uint_to_hex_lower8 ((d >> 40) & 255) << 16
        | uint_to_hex_lower8 ((d >> 32) & 255) <<  0;
  E[14] = uint_to_hex_lower8 ((d >> 24) & 255) << 16
        | uint_to_hex_lower8 ((d >> 16) & 255) <<  0;
  E[15] = uint_to_hex_lower8 ((d >>  8) & 255) << 16
        | uint_to_hex_lower8 ((d >>  0) & 255) <<  0;

  E[16] = uint_to_hex_lower8 ((e >> 56) & 255) << 16
        | uint_to_hex_lower8 ((e >> 48) & 255) <<  0;
  E[17] = uint_to_hex_lower8 ((e >> 40) & 255) << 16
        | uint_to_hex_lower8 ((e >> 32) & 255) <<  0;

  for (u32 i = 0; i < 18; i++)
  {
    tmps[gid].E[i] = E[i];
  }

  /**
   * salt
   */

  u32 salt_buf[4];

  salt_buf[0] = salt_bufs[SALT_POS_HOST].salt_buf[0];
  salt_buf[1] = salt_bufs[SALT_POS_HOST].salt_buf[1];
  salt_buf[2] = salt_bufs[SALT_POS_HOST].salt_buf[2];
  salt_buf[3] = salt_bufs[SALT_POS_HOST].salt_buf[3];

  #ifdef DYNAMIC_LOCAL
  // from host
  #else
  LOCAL_VK u32 S0_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S1_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S2_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S3_all[FIXED_LOCAL_SIZE][256];
  #endif

  #ifdef BCRYPT_AVOID_BANK_CONFLICTS
  LOCAL_AS u32 *S0 = S + (FIXED_LOCAL_SIZE * 256 * 0);
  LOCAL_AS u32 *S1 = S + (FIXED_LOCAL_SIZE * 256 * 1);
  LOCAL_AS u32 *S2 = S + (FIXED_LOCAL_SIZE * 256 * 2);
  LOCAL_AS u32 *S3 = S + (FIXED_LOCAL_SIZE * 256 * 3);
  #else
  LOCAL_AS u32 *S0 = S0_all[lid];
  LOCAL_AS u32 *S1 = S1_all[lid];
  LOCAL_AS u32 *S2 = S2_all[lid];
  LOCAL_AS u32 *S3 = S3_all[lid];
  #endif

  u32 P[18];

  blowfish_set_key_salt (E, 18, salt_buf, P, S0, S1, S2, S3);

  // store

  for (u32 i = 0; i < 18; i++)
  {
    tmps[gid].P[i] = P[i];
  }

  for (u32 i = 0; i < 256; i++)
  {
    tmps[gid].S0[i] = GET_KEY32 (S0, i);
    tmps[gid].S1[i] = GET_KEY32 (S1, i);
    tmps[gid].S2[i] = GET_KEY32 (S2, i);
    tmps[gid].S3[i] = GET_KEY32 (S3, i);
  }
}

KERNEL_FQ KERNEL_FA void m28400_loop (KERN_ATTR_TMPS (bcrypt_tmp_t))
{
  /**
   * base
   */

  const u64 gid = get_global_id (0);
  const u64 lid = get_local_id (0);

  if (gid >= GID_CNT) return;

  // load

  u32 E[18];

  for (u32 i = 0; i < 18; i++)
  {
    E[i] = tmps[gid].E[i];
  }

  u32 P[18];

  for (u32 i = 0; i < 18; i++)
  {
    P[i] = tmps[gid].P[i];
  }

  #ifdef DYNAMIC_LOCAL
  // from host
  #else
  LOCAL_VK u32 S0_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S1_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S2_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S3_all[FIXED_LOCAL_SIZE][256];
  #endif

  #ifdef BCRYPT_AVOID_BANK_CONFLICTS
  LOCAL_AS u32 *S0 = S + (FIXED_LOCAL_SIZE * 256 * 0);
  LOCAL_AS u32 *S1 = S + (FIXED_LOCAL_SIZE * 256 * 1);
  LOCAL_AS u32 *S2 = S + (FIXED_LOCAL_SIZE * 256 * 2);
  LOCAL_AS u32 *S3 = S + (FIXED_LOCAL_SIZE * 256 * 3);
  #else
  LOCAL_AS u32 *S0 = S0_all[lid];
  LOCAL_AS u32 *S1 = S1_all[lid];
  LOCAL_AS u32 *S2 = S2_all[lid];
  LOCAL_AS u32 *S3 = S3_all[lid];
  #endif

  for (u32 i = 0; i < 256; i++)
  {
    SET_KEY32 (S0, i, tmps[gid].S0[i]);
    SET_KEY32 (S1, i, tmps[gid].S1[i]);
    SET_KEY32 (S2, i, tmps[gid].S2[i]);
    SET_KEY32 (S3, i, tmps[gid].S3[i]);
  }

  /**
   * salt
   */

  u32 salt_buf[4];

  salt_buf[0] = salt_bufs[SALT_POS_HOST].salt_buf[0];
  salt_buf[1] = salt_bufs[SALT_POS_HOST].salt_buf[1];
  salt_buf[2] = salt_bufs[SALT_POS_HOST].salt_buf[2];
  salt_buf[3] = salt_bufs[SALT_POS_HOST].salt_buf[3];

  /**
   * main loop
   */

  for (u32 i = 0; i < LOOP_CNT; i++)
  {
    for (u32 i = 0; i < 18; i++)
    {
      P[i] ^= E[i];
    }

    blowfish_encrypt (P, S0, S1, S2, S3);

    P[ 0] ^= salt_buf[0];
    P[ 1] ^= salt_buf[1];
    P[ 2] ^= salt_buf[2];
    P[ 3] ^= salt_buf[3];
    P[ 4] ^= salt_buf[0];
    P[ 5] ^= salt_buf[1];
    P[ 6] ^= salt_buf[2];
    P[ 7] ^= salt_buf[3];
    P[ 8] ^= salt_buf[0];
    P[ 9] ^= salt_buf[1];
    P[10] ^= salt_buf[2];
    P[11] ^= salt_buf[3];
    P[12] ^= salt_buf[0];
    P[13] ^= salt_buf[1];
    P[14] ^= salt_buf[2];
    P[15] ^= salt_buf[3];
    P[16] ^= salt_buf[0];
    P[17] ^= salt_buf[1];

    blowfish_encrypt (P, S0, S1, S2, S3);
  }

  // store

  for (u32 i = 0; i < 18; i++)
  {
    tmps[gid].P[i] = P[i];
  }

  for (u32 i = 0; i < 256; i++)
  {
    tmps[gid].S0[i] = GET_KEY32 (S0, i);
    tmps[gid].S1[i] = GET_KEY32 (S1, i);
    tmps[gid].S2[i] = GET_KEY32 (S2, i);
    tmps[gid].S3[i] = GET_KEY32 (S3, i);
  }
}

KERNEL_FQ KERNEL_FA void m28400_comp (KERN_ATTR_TMPS (bcrypt_tmp_t))
{
  /**
   * base
   */

  const u64 gid = get_global_id (0);
  const u64 lid = get_local_id (0);

  if (gid >= GID_CNT) return;

  // load

  u32 P[18];

  for (u32 i = 0; i < 18; i++)
  {
    P[i] = tmps[gid].P[i];
  }

  #ifdef DYNAMIC_LOCAL
  // from host
  #else
  LOCAL_VK u32 S0_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S1_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S2_all[FIXED_LOCAL_SIZE][256];
  LOCAL_VK u32 S3_all[FIXED_LOCAL_SIZE][256];
  #endif

  #ifdef BCRYPT_AVOID_BANK_CONFLICTS
  LOCAL_AS u32 *S0 = S + (FIXED_LOCAL_SIZE * 256 * 0);
  LOCAL_AS u32 *S1 = S + (FIXED_LOCAL_SIZE * 256 * 1);
  LOCAL_AS u32 *S2 = S + (FIXED_LOCAL_SIZE * 256 * 2);
  LOCAL_AS u32 *S3 = S + (FIXED_LOCAL_SIZE * 256 * 3);
  #else
  LOCAL_AS u32 *S0 = S0_all[lid];
  LOCAL_AS u32 *S1 = S1_all[lid];
  LOCAL_AS u32 *S2 = S2_all[lid];
  LOCAL_AS u32 *S3 = S3_all[lid];
  #endif

  for (u32 i = 0; i < 256; i++)
  {
    SET_KEY32 (S0, i, tmps[gid].S0[i]);
    SET_KEY32 (S1, i, tmps[gid].S1[i]);
    SET_KEY32 (S2, i, tmps[gid].S2[i]);
    SET_KEY32 (S3, i, tmps[gid].S3[i]);
  }

  /**
   * main
   */

  u32 L0;
  u32 R0;

  L0 = BCRYPTM_0;
  R0 = BCRYPTM_1;

  for (u32 i = 0; i < 64; i++)
  {
    BF_ENCRYPT (L0, R0);
  }

  const u32 r0 = L0;
  const u32 r1 = R0;

  L0 = BCRYPTM_2;
  R0 = BCRYPTM_3;

  for (u32 i = 0; i < 64; i++)
  {
    BF_ENCRYPT (L0, R0);
  }

  const u32 r2 = L0;
  const u32 r3 = R0;

  /*
  e = L0;
  f = R0;

  f &= ~0xff; // its just 23 not 24 !
  */

  #define il_pos 0

  #ifdef KERNEL_STATIC
  #include COMPARE_M
  #endif
}
