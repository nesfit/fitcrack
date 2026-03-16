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
#include M2S(INCLUDE_PATH/inc_hash_sha1.cl)
#endif

#define COMPARE_S M2S(INCLUDE_PATH/inc_comp_single.cl)
#define COMPARE_M M2S(INCLUDE_PATH/inc_comp_multi.cl)

typedef struct pwsafe2_tmp
{
  u32 digest[2];

  u32 P[18];

  u32 S0[256];
  u32 S1[256];
  u32 S2[256];
  u32 S3[256];

} pwsafe2_tmp_t;

KERNEL_FQ KERNEL_FA void m09000_init (KERN_ATTR_TMPS (pwsafe2_tmp_t))
{
  /**
   * base
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  u32 w0[4];

  w0[0] = pws[gid].i[ 0];
  w0[1] = pws[gid].i[ 1];
  w0[2] = pws[gid].i[ 2];
  w0[3] = pws[gid].i[ 3];

  u32 w1[4];

  w1[0] = pws[gid].i[ 4];
  w1[1] = pws[gid].i[ 5];
  w1[2] = pws[gid].i[ 6];
  w1[3] = pws[gid].i[ 7];

  u32 w2[4];

  w2[0] = pws[gid].i[ 8];
  w2[1] = pws[gid].i[ 9];
  w2[2] = pws[gid].i[10];
  w2[3] = pws[gid].i[11];

  u32 w3[4];

  w3[0] = pws[gid].i[12];
  w3[1] = pws[gid].i[13];
  w3[2] = pws[gid].i[14];
  w3[3] = pws[gid].i[15];

  const u32 pw_len = pws[gid].pw_len;

  append_0x80_4x4 (w0, w1, w2, w3, pw_len);

  /**
   * salt
   */

  const u32 salt_len = salt_bufs[SALT_POS_HOST].salt_len;

  u32 salt_buf[2];

  salt_buf[0] = salt_bufs[SALT_POS_HOST].salt_buf[0];
  salt_buf[1] = salt_bufs[SALT_POS_HOST].salt_buf[1];

  /**
   * initial sha1
   */

  w3[1] = w2[3] << 16 | w2[2] >> 16;
  w3[0] = w2[2] << 16 | w2[1] >> 16;
  w2[3] = w2[1] << 16 | w2[0] >> 16;
  w2[2] = w2[0] << 16 | w1[3] >> 16;
  w2[1] = w1[3] << 16 | w1[2] >> 16;
  w2[0] = w1[2] << 16 | w1[1] >> 16;
  w1[3] = w1[1] << 16 | w1[0] >> 16;
  w1[2] = w1[0] << 16 | w0[3] >> 16;
  w1[1] = w0[3] << 16 | w0[2] >> 16;
  w1[0] = w0[2] << 16 | w0[1] >> 16;
  w0[3] = w0[1] << 16 | w0[0] >> 16;
  w0[2] = w0[0] << 16;
  w0[1] = salt_buf[1];
  w0[0] = salt_buf[0];

  w0[0] = hc_swap32_S (w0[0]);
  w0[1] = hc_swap32_S (w0[1]);
  w0[2] = hc_swap32_S (w0[2]);
  w0[3] = hc_swap32_S (w0[3]);
  w1[0] = hc_swap32_S (w1[0]);
  w1[1] = hc_swap32_S (w1[1]);
  w1[2] = hc_swap32_S (w1[2]);
  w1[3] = hc_swap32_S (w1[3]);
  w2[0] = hc_swap32_S (w2[0]);
  w2[1] = hc_swap32_S (w2[1]);
  w2[2] = hc_swap32_S (w2[2]);
  w2[3] = hc_swap32_S (w2[3]);
  w3[0] = hc_swap32_S (w3[0]);
  w3[1] = hc_swap32_S (w3[1]);

  const u32 block_len = salt_len + 2 + pw_len;

  w3[2] = 0;
  w3[3] = block_len * 8;

  u32 digest[5];

  digest[0] = SHA1M_A;
  digest[1] = SHA1M_B;
  digest[2] = SHA1M_C;
  digest[3] = SHA1M_D;
  digest[4] = SHA1M_E;

  sha1_transform (w0, w1, w2, w3, digest);

  /**
   * blowfish setkey
   */

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

  blowfish_set_key (digest, 5, P, S0, S1, S2, S3);

  // store

  tmps[gid].digest[0] = salt_buf[0];
  tmps[gid].digest[1] = salt_buf[1];

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

KERNEL_FQ KERNEL_FA void m09000_loop (KERN_ATTR_TMPS (pwsafe2_tmp_t))
{
  /**
   * base
   */

  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  const u64 lid = get_local_id (0);

  // load

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

  for (u32 i = 0; i < 18; i++)
  {
    P[i] = tmps[gid].P[i];
  }
  for (u32 i = 0; i < 256; i++)
  {
    SET_KEY32 (S0, i, tmps[gid].S0[i]);
    SET_KEY32 (S1, i, tmps[gid].S1[i]);
    SET_KEY32 (S2, i, tmps[gid].S2[i]);
    SET_KEY32 (S3, i, tmps[gid].S3[i]);
  }

  u32 digest[2];

  digest[0] = tmps[gid].digest[0];
  digest[1] = tmps[gid].digest[1];

  // loop

  u32 L0 = digest[0];
  u32 R0 = digest[1];

  for (u32 i = 0; i < LOOP_CNT; i++)
  {
    BF_ENCRYPT (L0, R0);
  }

  tmps[gid].digest[0] = L0;
  tmps[gid].digest[1] = R0;
}

KERNEL_FQ KERNEL_FA void m09000_comp (KERN_ATTR_TMPS (pwsafe2_tmp_t))
{
  /**
   * base
   */

  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  const u64 lid = get_local_id (0);

  // load

  u32 digest[2];

  digest[0] = tmps[gid].digest[0];
  digest[1] = tmps[gid].digest[1];

  // final sha1

  u32 w0[4];
  u32 w1[4];
  u32 w2[4];
  u32 w3[4];

  w0[0] = hc_swap32_S (digest[0]);
  w0[1] = hc_swap32_S (digest[1]);
  w0[2] = 0x00008000;
  w0[3] = 0;
  w1[0] = 0;
  w1[1] = 0;
  w1[2] = 0;
  w1[3] = 0;
  w2[0] = 0;
  w2[1] = 0;
  w2[2] = 0;
  w2[3] = 0;
  w3[0] = 0;
  w3[1] = 0;
  w3[2] = 0;
  w3[3] = (8 + 2) * 8;

  u32 out[5];

  out[0] = 0; // yep, not a bug! context is zero here
  out[1] = 0;
  out[2] = 0;
  out[3] = 0;
  out[4] = 0;

  sha1_transform (w0, w1, w2, w3, out);

  const u32 r0 = out[0];
  const u32 r1 = out[1];
  const u32 r2 = out[2];
  const u32 r3 = out[3];

  #define il_pos 0

  #ifdef KERNEL_STATIC
  #include COMPARE_M
  #endif
}
