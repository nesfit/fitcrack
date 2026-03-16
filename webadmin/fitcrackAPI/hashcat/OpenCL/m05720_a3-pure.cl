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
#include M2S(INCLUDE_PATH/inc_hash_sha256.cl)
#endif

KERNEL_FQ KERNEL_FA void m05720_mxx (KERN_ATTR_VECTOR ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  const u32 pw_len = pws[gid].pw_len;

  u32x w[64] = { 0 };

  for (u32 i = 0, idx = 0; i < pw_len; i += 4, idx += 1)
  {
    w[idx] = hc_swap32 (pws[gid].i[idx]);
  }

  const u32 salt_iter = salt_bufs[SALT_POS_HOST].salt_iter;

  sha256_ctx_t ctx0;

  sha256_init (&ctx0);

  sha256_update_global (&ctx0, salt_bufs[SALT_POS_HOST].salt_buf, salt_bufs[SALT_POS_HOST].salt_len);

  /**
   * loop
   */

  u32x w0l = w[0];

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos += VECT_SIZE)
  {
    const u32x w0r = words_buf_r[il_pos / VECT_SIZE];

    const u32x w0 = w0l | hc_swap32 (w0r);

    w[0] = w0;

    sha256_ctx_vector_t ctx;

    sha256_init_vector_from_scalar (&ctx, &ctx0);

    sha256_update_vector (&ctx, w, pw_len);

    sha256_final_vector (&ctx);

    u32x digest[8];

    digest[0] = ctx.h[0];
    digest[1] = ctx.h[1];
    digest[2] = ctx.h[2];
    digest[3] = ctx.h[3];
    digest[4] = ctx.h[4];
    digest[5] = ctx.h[5];
    digest[6] = ctx.h[6];
    digest[7] = ctx.h[7];

    // iterations

    u32x wt0[4];
    u32x wt1[4];
    u32x wt2[4];
    u32x wt3[4];

    for (u32 i = 0; i < salt_iter; i++)
    {
      wt0[0] = digest[0];
      wt0[1] = digest[1];
      wt0[2] = digest[2];
      wt0[3] = digest[3];
      wt1[0] = digest[4];
      wt1[1] = digest[5];
      wt1[2] = digest[6];
      wt1[3] = digest[7];
      wt2[0] = 0x80000000;
      wt2[1] = 0;
      wt2[2] = 0;
      wt2[3] = 0;
      wt3[0] = 0;
      wt3[1] = 0;
      wt3[2] = 0;
      wt3[3] = 32 * 8;

      digest[0] = SHA256M_A;
      digest[1] = SHA256M_B;
      digest[2] = SHA256M_C;
      digest[3] = SHA256M_D;
      digest[4] = SHA256M_E;
      digest[5] = SHA256M_F;
      digest[6] = SHA256M_G;
      digest[7] = SHA256M_H;

      sha256_transform_vector (wt0, wt1, wt2, wt3, digest);
    }

    const u32x r0 = digest[DGST_R0];
    const u32x r1 = digest[DGST_R1];
    const u32x r2 = digest[DGST_R2];
    const u32x r3 = digest[DGST_R3];

    COMPARE_M_SIMD (r0, r1, r2, r3);
  }
}

KERNEL_FQ KERNEL_FA void m05720_sxx (KERN_ATTR_VECTOR ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  /**
   * digest
   */

  const u32 search[4] =
  {
    digests_buf[DIGESTS_OFFSET_HOST].digest_buf[DGST_R0],
    digests_buf[DIGESTS_OFFSET_HOST].digest_buf[DGST_R1],
    digests_buf[DIGESTS_OFFSET_HOST].digest_buf[DGST_R2],
    digests_buf[DIGESTS_OFFSET_HOST].digest_buf[DGST_R3]
  };

  /**
   * base
   */

  const u32 pw_len = pws[gid].pw_len;

  u32x w[64] = { 0 };

  for (u32 i = 0, idx = 0; i < pw_len; i += 4, idx += 1)
  {
    w[idx] = hc_swap32 (pws[gid].i[idx]);
  }

  const u32 salt_iter = salt_bufs[SALT_POS_HOST].salt_iter;

  sha256_ctx_t ctx0;

  sha256_init (&ctx0);

  sha256_update_global (&ctx0, salt_bufs[SALT_POS_HOST].salt_buf, salt_bufs[SALT_POS_HOST].salt_len);

  /**
   * loop
   */

  u32x w0l = w[0];

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos += VECT_SIZE)
  {
    const u32x w0r = words_buf_r[il_pos / VECT_SIZE];

    const u32x w0 = w0l | hc_swap32 (w0r);

    w[0] = w0;

    sha256_ctx_vector_t ctx;

    sha256_init_vector_from_scalar (&ctx, &ctx0);

    sha256_update_vector (&ctx, w, pw_len);

    sha256_final_vector (&ctx);

    u32x digest[8];

    digest[0] = ctx.h[0];
    digest[1] = ctx.h[1];
    digest[2] = ctx.h[2];
    digest[3] = ctx.h[3];
    digest[4] = ctx.h[4];
    digest[5] = ctx.h[5];
    digest[6] = ctx.h[6];
    digest[7] = ctx.h[7];

    // iterations

    u32x wt0[4];
    u32x wt1[4];
    u32x wt2[4];
    u32x wt3[4];

    for (u32 i = 0; i < salt_iter; i++)
    {
      wt0[0] = digest[0];
      wt0[1] = digest[1];
      wt0[2] = digest[2];
      wt0[3] = digest[3];
      wt1[0] = digest[4];
      wt1[1] = digest[5];
      wt1[2] = digest[6];
      wt1[3] = digest[7];
      wt2[0] = 0x80000000;
      wt2[1] = 0;
      wt2[2] = 0;
      wt2[3] = 0;
      wt3[0] = 0;
      wt3[1] = 0;
      wt3[2] = 0;
      wt3[3] = 32 * 8;

      digest[0] = SHA256M_A;
      digest[1] = SHA256M_B;
      digest[2] = SHA256M_C;
      digest[3] = SHA256M_D;
      digest[4] = SHA256M_E;
      digest[5] = SHA256M_F;
      digest[6] = SHA256M_G;
      digest[7] = SHA256M_H;

      sha256_transform_vector (wt0, wt1, wt2, wt3, digest);
    }

    const u32x r0 = digest[DGST_R0];
    const u32x r1 = digest[DGST_R1];
    const u32x r2 = digest[DGST_R2];
    const u32x r3 = digest[DGST_R3];

    COMPARE_S_SIMD (r0, r1, r2, r3);
  }
}
