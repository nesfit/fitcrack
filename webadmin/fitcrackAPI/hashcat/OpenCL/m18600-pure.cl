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
#include M2S(INCLUDE_PATH/inc_cipher_blowfish.cl)
#include M2S(INCLUDE_PATH/inc_hash_sha1.cl)
#endif

#define COMPARE_S M2S(INCLUDE_PATH/inc_comp_single.cl)
#define COMPARE_M M2S(INCLUDE_PATH/inc_comp_multi.cl)

typedef struct odf11_tmp
{
  u32  ipad[5];
  u32  opad[5];

  u32  dgst[5];
  u32  out[5];

} odf11_tmp_t;

typedef struct odf11
{
  u32 iterations;
  u32 iv[2];
  u32 checksum[5];
  u32 encrypted_data[256];
  int encrypted_len;

} odf11_t;

DECLSPEC void hmac_sha1_run_V (PRIVATE_AS u32x *w0, PRIVATE_AS u32x *w1, PRIVATE_AS u32x *w2, PRIVATE_AS u32x *w3, PRIVATE_AS u32x *ipad, PRIVATE_AS u32x *opad, PRIVATE_AS u32x *digest)
{
  digest[0] = ipad[0];
  digest[1] = ipad[1];
  digest[2] = ipad[2];
  digest[3] = ipad[3];
  digest[4] = ipad[4];

  sha1_transform_vector (w0, w1, w2, w3, digest);

  w0[0] = digest[0];
  w0[1] = digest[1];
  w0[2] = digest[2];
  w0[3] = digest[3];
  w1[0] = digest[4];
  w1[1] = 0x80000000;
  w1[2] = 0;
  w1[3] = 0;
  w2[0] = 0;
  w2[1] = 0;
  w2[2] = 0;
  w2[3] = 0;
  w3[0] = 0;
  w3[1] = 0;
  w3[2] = 0;
  w3[3] = (64 + 20) * 8;

  digest[0] = opad[0];
  digest[1] = opad[1];
  digest[2] = opad[2];
  digest[3] = opad[3];
  digest[4] = opad[4];

  sha1_transform_vector (w0, w1, w2, w3, digest);
}

KERNEL_FQ KERNEL_FA void m18600_init (KERN_ATTR_TMPS_ESALT (odf11_tmp_t, odf11_t))
{
  /**
   * base
   */

  const u64 gid = get_global_id (0);

  if (gid >= GID_CNT) return;

  sha1_ctx_t sha1_ctx;

  sha1_init (&sha1_ctx);

  sha1_update_global_swap (&sha1_ctx, pws[gid].i, pws[gid].pw_len);

  sha1_final (&sha1_ctx);

  // hmac key = hashed passphrase
  u32 k0[4];
  u32 k1[4];
  u32 k2[4];
  u32 k3[4];

  k0[0] = sha1_ctx.h[0];
  k0[1] = sha1_ctx.h[1];
  k0[2] = sha1_ctx.h[2];
  k0[3] = sha1_ctx.h[3];
  k1[0] = sha1_ctx.h[4];
  k1[1] = 0;
  k1[2] = 0;
  k1[3] = 0;
  k2[0] = 0;
  k2[1] = 0;
  k2[2] = 0;
  k2[3] = 0;
  k3[0] = 0;
  k3[1] = 0;
  k3[2] = 0;
  k3[3] = 0;

  // hmac message = salt
  u32 m0[4];
  u32 m1[4];
  u32 m2[4];
  u32 m3[4];

  m0[0] = salt_bufs[DIGESTS_OFFSET_HOST].salt_buf[0];
  m0[1] = salt_bufs[DIGESTS_OFFSET_HOST].salt_buf[1];
  m0[2] = salt_bufs[DIGESTS_OFFSET_HOST].salt_buf[2];
  m0[3] = salt_bufs[DIGESTS_OFFSET_HOST].salt_buf[3];
  m1[0] = 0;
  m1[1] = 0;
  m1[2] = 0;
  m1[3] = 0;
  m2[0] = 0;
  m2[1] = 0;
  m2[2] = 0;
  m2[3] = 0;
  m3[0] = 0;
  m3[1] = 0;
  m3[2] = 0;
  m3[3] = 0;

  sha1_hmac_ctx_t sha1_hmac_ctx;

  sha1_hmac_init_64 (&sha1_hmac_ctx, k0, k1, k2, k3);

  tmps[gid].ipad[0]  = sha1_hmac_ctx.ipad.h[0];
  tmps[gid].ipad[1]  = sha1_hmac_ctx.ipad.h[1];
  tmps[gid].ipad[2]  = sha1_hmac_ctx.ipad.h[2];
  tmps[gid].ipad[3]  = sha1_hmac_ctx.ipad.h[3];
  tmps[gid].ipad[4]  = sha1_hmac_ctx.ipad.h[4];

  tmps[gid].opad[0]  = sha1_hmac_ctx.opad.h[0];
  tmps[gid].opad[1]  = sha1_hmac_ctx.opad.h[1];
  tmps[gid].opad[2]  = sha1_hmac_ctx.opad.h[2];
  tmps[gid].opad[3]  = sha1_hmac_ctx.opad.h[3];
  tmps[gid].opad[4]  = sha1_hmac_ctx.opad.h[4];

  // first pbkdf iteration
  m1[0] = 1;

  sha1_hmac_ctx_t sha1_hmac_ctx_loop = sha1_hmac_ctx;

  sha1_hmac_update_64 (&sha1_hmac_ctx_loop, m0, m1, m2, m3, 20);

  sha1_hmac_final (&sha1_hmac_ctx_loop);

  tmps[gid].dgst[0] = sha1_hmac_ctx_loop.opad.h[0];
  tmps[gid].dgst[1] = sha1_hmac_ctx_loop.opad.h[1];
  tmps[gid].dgst[2] = sha1_hmac_ctx_loop.opad.h[2];
  tmps[gid].dgst[3] = sha1_hmac_ctx_loop.opad.h[3];
  tmps[gid].dgst[4] = sha1_hmac_ctx_loop.opad.h[4];

  tmps[gid].out[0] = tmps[gid].dgst[0];
  tmps[gid].out[1] = tmps[gid].dgst[1];
  tmps[gid].out[2] = tmps[gid].dgst[2];
  tmps[gid].out[3] = tmps[gid].dgst[3];
  tmps[gid].out[4] = tmps[gid].dgst[4];
}

KERNEL_FQ KERNEL_FA void m18600_loop (KERN_ATTR_TMPS_ESALT (odf11_tmp_t, odf11_t))
{
  const u64 gid = get_global_id (0);

  if ((gid * VECT_SIZE) >= GID_CNT) return;

  u32x ipad[5];
  u32x opad[5];

  ipad[0] = packv (tmps, ipad, gid, 0);
  ipad[1] = packv (tmps, ipad, gid, 1);
  ipad[2] = packv (tmps, ipad, gid, 2);
  ipad[3] = packv (tmps, ipad, gid, 3);
  ipad[4] = packv (tmps, ipad, gid, 4);

  opad[0] = packv (tmps, opad, gid, 0);
  opad[1] = packv (tmps, opad, gid, 1);
  opad[2] = packv (tmps, opad, gid, 2);
  opad[3] = packv (tmps, opad, gid, 3);
  opad[4] = packv (tmps, opad, gid, 4);

  u32x dgst[5];
  u32x out[5];

  dgst[0] = packv (tmps, dgst, gid, 0);
  dgst[1] = packv (tmps, dgst, gid, 1);
  dgst[2] = packv (tmps, dgst, gid, 2);
  dgst[3] = packv (tmps, dgst, gid, 3);
  dgst[4] = packv (tmps, dgst, gid, 4);

  out[0] = packv (tmps, out, gid, 0);
  out[1] = packv (tmps, out, gid, 1);
  out[2] = packv (tmps, out, gid, 2);
  out[3] = packv (tmps, out, gid, 3);
  out[4] = packv (tmps, out, gid, 4);

  for (u32 j = 0; j < LOOP_CNT; j++)
  {
    u32x w0[4];
    u32x w1[4];
    u32x w2[4];
    u32x w3[4];

    w0[0] = dgst[0];
    w0[1] = dgst[1];
    w0[2] = dgst[2];
    w0[3] = dgst[3];
    w1[0] = dgst[4];
    w1[1] = 0x80000000;
    w1[2] = 0;
    w1[3] = 0;
    w2[0] = 0;
    w2[1] = 0;
    w2[2] = 0;
    w2[3] = 0;
    w3[0] = 0;
    w3[1] = 0;
    w3[2] = 0;
    w3[3] = (64 + 20) * 8;

    hmac_sha1_run_V (w0, w1, w2, w3, ipad, opad, dgst);

    out[0] ^= dgst[0];
    out[1] ^= dgst[1];
    out[2] ^= dgst[2];
    out[3] ^= dgst[3];
    out[4] ^= dgst[4];

    unpackv (tmps, dgst, gid, 0, dgst[0]);
    unpackv (tmps, dgst, gid, 1, dgst[1]);
    unpackv (tmps, dgst, gid, 2, dgst[2]);
    unpackv (tmps, dgst, gid, 3, dgst[3]);
    unpackv (tmps, dgst, gid, 4, dgst[4]);

    unpackv (tmps, out, gid, 0, out[0]);
    unpackv (tmps, out, gid, 1, out[1]);
    unpackv (tmps, out, gid, 2, out[2]);
    unpackv (tmps, out, gid, 3, out[3]);
    unpackv (tmps, out, gid, 4, out[4]);
  }
}

KERNEL_FQ KERNEL_FA FIXED_THREAD_COUNT(FIXED_LOCAL_SIZE_COMP) void m18600_comp (KERN_ATTR_TMPS_ESALT (odf11_tmp_t, odf11_t))
{
  const u64 gid = get_global_id (0);
  const u64 lid = get_local_id (0);
  const u64 lsz = get_local_size (0);

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  u32 ukey[4];

  ukey[0] = tmps[gid].out[0];
  ukey[1] = tmps[gid].out[1];
  ukey[2] = tmps[gid].out[2];
  ukey[3] = tmps[gid].out[3];

  /**
   * blowfish setkey
   */

  #ifdef DYNAMIC_LOCAL
  // from host
  #else
  LOCAL_VK u32 S0_all[FIXED_LOCAL_SIZE_COMP][256];
  LOCAL_VK u32 S1_all[FIXED_LOCAL_SIZE_COMP][256];
  LOCAL_VK u32 S2_all[FIXED_LOCAL_SIZE_COMP][256];
  LOCAL_VK u32 S3_all[FIXED_LOCAL_SIZE_COMP][256];
  #endif

  #ifdef BCRYPT_AVOID_BANK_CONFLICTS
  LOCAL_AS u32 *S0 = S + (FIXED_LOCAL_SIZE_COMP * 256 * 0);
  LOCAL_AS u32 *S1 = S + (FIXED_LOCAL_SIZE_COMP * 256 * 1);
  LOCAL_AS u32 *S2 = S + (FIXED_LOCAL_SIZE_COMP * 256 * 2);
  LOCAL_AS u32 *S3 = S + (FIXED_LOCAL_SIZE_COMP * 256 * 3);
  #else
  LOCAL_AS u32 *S0 = S0_all[lid];
  LOCAL_AS u32 *S1 = S1_all[lid];
  LOCAL_AS u32 *S2 = S2_all[lid];
  LOCAL_AS u32 *S3 = S3_all[lid];
  #endif

  u32 P[18];

  blowfish_set_key (ukey, 4, P, S0, S1, S2, S3);

  GLOBAL_AS const odf11_t *es = &esalt_bufs[DIGESTS_OFFSET_HOST];

  u32 iv[2];

  iv[0] = es->iv[0];
  iv[1] = es->iv[1];

  u32 pt[256];

  for (int i = 0, j = 0; i < es->encrypted_len; i += 8, j += 2)
  {
    u32 ct[2];

    ct[0] = es->encrypted_data[j + 0];
    ct[1] = es->encrypted_data[j + 1];

    BF_ENCRYPT (iv[0], iv[1]);

    pt[j + 0] = ct[0] ^ iv[0];
    pt[j + 1] = ct[1] ^ iv[1];

    iv[0] = ct[0];
    iv[1] = ct[1];
  }

  const int full64 = es->encrypted_len / 64;

  const int encrypted_len64 = full64 * 64;

  sha1_ctx_t sha1_ctx;

  sha1_init (&sha1_ctx);

  sha1_update (&sha1_ctx, pt, encrypted_len64);

  const int remaining64 = es->encrypted_len - encrypted_len64;

  if (remaining64)
  {
    PRIVATE_AS u32 *pt_remaining = pt + (encrypted_len64 / 4);

    truncate_block_16x4_be_S (pt_remaining + 0, pt_remaining + 4, pt_remaining + 8, pt_remaining + 12, remaining64);

    sha1_update (&sha1_ctx, pt_remaining, remaining64);
  }

  sha1_final (&sha1_ctx);

  const u32 r0 = sha1_ctx.h[0];
  const u32 r1 = sha1_ctx.h[1];
  const u32 r2 = sha1_ctx.h[2];
  const u32 r3 = sha1_ctx.h[3];

  #define il_pos 0

  #ifdef KERNEL_STATIC
  #include COMPARE_M
  #endif
}
