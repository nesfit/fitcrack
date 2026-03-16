/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#define NEW_SIMD_CODE

#ifdef KERNEL_STATIC
#include M2S(INCLUDE_PATH/inc_vendor.h)
#include M2S(INCLUDE_PATH/inc_types.h)
#include M2S(INCLUDE_PATH/inc_platform.cl)
#include M2S(INCLUDE_PATH/inc_common.cl)
#include M2S(INCLUDE_PATH/inc_simd.cl)
#include M2S(INCLUDE_PATH/inc_hash_sha512.cl)
#endif

#define COMPARE_S M2S(INCLUDE_PATH/inc_comp_single.cl)
#define COMPARE_M M2S(INCLUDE_PATH/inc_comp_multi.cl)

typedef struct shiro1_sha512_tmp
{
  u32 dgst[16];

} shiro1_sha512_tmp_t;

KERNEL_FQ KERNEL_FA void m12150_init (KERN_ATTR_TMPS (shiro1_sha512_tmp_t))
{
  const u32 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  sha512_ctx_t ctx;

  sha512_init (&ctx);

  sha512_update_global_swap (&ctx, salt_bufs[SALT_POS_HOST].salt_buf, salt_bufs[SALT_POS_HOST].salt_len);

  sha512_update_global_swap (&ctx, pws[gid].i, pws[gid].pw_len);

  sha512_final (&ctx);

  tmps[gid].dgst[ 0] = h32_from_64_S (ctx.h[0]);
  tmps[gid].dgst[ 1] = l32_from_64_S (ctx.h[0]);
  tmps[gid].dgst[ 2] = h32_from_64_S (ctx.h[1]);
  tmps[gid].dgst[ 3] = l32_from_64_S (ctx.h[1]);
  tmps[gid].dgst[ 4] = h32_from_64_S (ctx.h[2]);
  tmps[gid].dgst[ 5] = l32_from_64_S (ctx.h[2]);
  tmps[gid].dgst[ 6] = h32_from_64_S (ctx.h[3]);
  tmps[gid].dgst[ 7] = l32_from_64_S (ctx.h[3]);
  tmps[gid].dgst[ 8] = h32_from_64_S (ctx.h[4]);
  tmps[gid].dgst[ 9] = l32_from_64_S (ctx.h[4]);
  tmps[gid].dgst[10] = h32_from_64_S (ctx.h[5]);
  tmps[gid].dgst[11] = l32_from_64_S (ctx.h[5]);
  tmps[gid].dgst[12] = h32_from_64_S (ctx.h[6]);
  tmps[gid].dgst[13] = l32_from_64_S (ctx.h[6]);
  tmps[gid].dgst[14] = h32_from_64_S (ctx.h[7]);
  tmps[gid].dgst[15] = l32_from_64_S (ctx.h[7]);
}

KERNEL_FQ KERNEL_FA void m12150_loop(KERN_ATTR_TMPS (shiro1_sha512_tmp_t))
{
  const u32 gid = get_global_id(0);

  if (gid >= GID_CNT) return;

  u32 w0[4];
  u32 w1[4];
  u32 w2[4];
  u32 w3[4];
  u32 w4[4];
  u32 w5[4];
  u32 w6[4];
  u32 w7[4];

  w0[0] = tmps[gid].dgst[ 0];
  w0[1] = tmps[gid].dgst[ 1];
  w0[2] = tmps[gid].dgst[ 2];
  w0[3] = tmps[gid].dgst[ 3];
  w1[0] = tmps[gid].dgst[ 4];
  w1[1] = tmps[gid].dgst[ 5];
  w1[2] = tmps[gid].dgst[ 6];
  w1[3] = tmps[gid].dgst[ 7];
  w2[0] = tmps[gid].dgst[ 8];
  w2[1] = tmps[gid].dgst[ 9];
  w2[2] = tmps[gid].dgst[10];
  w2[3] = tmps[gid].dgst[11];
  w3[0] = tmps[gid].dgst[12];
  w3[1] = tmps[gid].dgst[13];
  w3[2] = tmps[gid].dgst[14];
  w3[3] = tmps[gid].dgst[15];
  w4[0] = 0x80000000;
  w4[1] = 0;
  w4[2] = 0;
  w4[3] = 0;
  w5[0] = 0;
  w5[1] = 0;
  w5[2] = 0;
  w5[3] = 0;
  w6[0] = 0;
  w6[1] = 0;
  w6[2] = 0;
  w6[3] = 0;
  w7[0] = 0;
  w7[1] = 0;
  w7[2] = 0;
  w7[3] = 64 * 8;

  for (u32 i = 0; i < LOOP_CNT; i++)
  {
    u64 dgst[8];

    dgst[0] = SHA512M_A;
    dgst[1] = SHA512M_B;
    dgst[2] = SHA512M_C;
    dgst[3] = SHA512M_D;
    dgst[4] = SHA512M_E;
    dgst[5] = SHA512M_F;
    dgst[6] = SHA512M_G;
    dgst[7] = SHA512M_H;

    sha512_transform (w0, w1, w2, w3, w4, w5, w6, w7, dgst);

    // Update the digest_u32 array for the next iteration
    w0[0] = h32_from_64_S (dgst[0]);
    w0[1] = l32_from_64_S (dgst[0]);
    w0[2] = h32_from_64_S (dgst[1]);
    w0[3] = l32_from_64_S (dgst[1]);
    w1[0] = h32_from_64_S (dgst[2]);
    w1[1] = l32_from_64_S (dgst[2]);
    w1[2] = h32_from_64_S (dgst[3]);
    w1[3] = l32_from_64_S (dgst[3]);
    w2[0] = h32_from_64_S (dgst[4]);
    w2[1] = l32_from_64_S (dgst[4]);
    w2[2] = h32_from_64_S (dgst[5]);
    w2[3] = l32_from_64_S (dgst[5]);
    w3[0] = h32_from_64_S (dgst[6]);
    w3[1] = l32_from_64_S (dgst[6]);
    w3[2] = h32_from_64_S (dgst[7]);
    w3[3] = l32_from_64_S (dgst[7]);
  }

  tmps[gid].dgst[ 0] = w0[0];
  tmps[gid].dgst[ 1] = w0[1];
  tmps[gid].dgst[ 2] = w0[2];
  tmps[gid].dgst[ 3] = w0[3];
  tmps[gid].dgst[ 4] = w1[0];
  tmps[gid].dgst[ 5] = w1[1];
  tmps[gid].dgst[ 6] = w1[2];
  tmps[gid].dgst[ 7] = w1[3];
  tmps[gid].dgst[ 8] = w2[0];
  tmps[gid].dgst[ 9] = w2[1];
  tmps[gid].dgst[10] = w2[2];
  tmps[gid].dgst[11] = w2[3];
  tmps[gid].dgst[12] = w3[0];
  tmps[gid].dgst[13] = w3[1];
  tmps[gid].dgst[14] = w3[2];
  tmps[gid].dgst[15] = w3[3];
}

KERNEL_FQ KERNEL_FA void m12150_comp (KERN_ATTR_TMPS (shiro1_sha512_tmp_t))
{
  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  const u64 lid = get_local_id (0);

  const u32 r0 = tmps[gid].dgst[1];
  const u32 r1 = tmps[gid].dgst[0];
  const u32 r2 = tmps[gid].dgst[3];
  const u32 r3 = tmps[gid].dgst[2];

  #define il_pos 0

  #ifdef KERNEL_STATIC
  #include COMPARE_M
  #endif
}
