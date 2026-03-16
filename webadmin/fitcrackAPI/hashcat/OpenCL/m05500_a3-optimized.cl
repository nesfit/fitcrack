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
#include M2S(INCLUDE_PATH/inc_hash_md4.cl)
#include M2S(INCLUDE_PATH/inc_cipher_des.cl)
#endif

typedef struct netntlm
{
  u32 user_len;
  u32 domain_len;
  u32 srvchall_len;
  u32 clichall_len;

  u32 userdomain_buf[64];
  u32 chall_buf[256];

} netntlm_t;

DECLSPEC void transform_netntlmv1_key (const u32x w0, const u32x w1, PRIVATE_AS u32x *out)
{
  u32x t[8];

  t[0] = (w0 >>  0) & 0xff;
  t[1] = (w0 >>  8) & 0xff;
  t[2] = (w0 >> 16) & 0xff;
  t[3] = (w0 >> 24) & 0xff;
  t[4] = (w1 >>  0) & 0xff;
  t[5] = (w1 >>  8) & 0xff;
  t[6] = (w1 >> 16) & 0xff;
  t[7] = (w1 >> 24) & 0xff;

  u32x k[8];

  k[0] =               (t[0] >> 0);
  k[1] = (t[0] << 7) | (t[1] >> 1);
  k[2] = (t[1] << 6) | (t[2] >> 2);
  k[3] = (t[2] << 5) | (t[3] >> 3);
  k[4] = (t[3] << 4) | (t[4] >> 4);
  k[5] = (t[4] << 3) | (t[5] >> 5);
  k[6] = (t[5] << 2) | (t[6] >> 6);
  k[7] = (t[6] << 1);

  out[0] = ((k[0] & 0xff) <<  0)
         | ((k[1] & 0xff) <<  8)
         | ((k[2] & 0xff) << 16)
         | ((k[3] & 0xff) << 24);

  out[1] = ((k[4] & 0xff) <<  0)
         | ((k[5] & 0xff) <<  8)
         | ((k[6] & 0xff) << 16)
         | ((k[7] & 0xff) << 24);
}

DECLSPEC void m05500m (SHM_TYPE u32 (*s_SPtrans)[64], SHM_TYPE u32 (*s_skb)[64], PRIVATE_AS u32 *w, const u32 pw_len, KERN_ATTR_FUNC_VECTOR ())
{
  /**
   * modifiers are taken from args
   */

  /**
   * salt
   */

  const u32 s0 = salt_bufs[SALT_POS_HOST].salt_buf[0];
  const u32 s1 = salt_bufs[SALT_POS_HOST].salt_buf[1];
  const u32 s2 = salt_bufs[SALT_POS_HOST].salt_buf[2];

  /**
   * loop
   */

  u32 w0l = w[0];

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos += VECT_SIZE)
  {
    const u32x w0r = words_buf_r[il_pos / VECT_SIZE];

    const u32x w0 = w0l | w0r;

    u32x w0_t[4];
    u32x w1_t[4];
    u32x w2_t[4];
    u32x w3_t[4];

    w0_t[0] = w0;
    w0_t[1] = w[ 1];
    w0_t[2] = w[ 2];
    w0_t[3] = w[ 3];
    w1_t[0] = w[ 4];
    w1_t[1] = w[ 5];
    w1_t[2] = w[ 6];
    w1_t[3] = w[ 7];
    w2_t[0] = w[ 8];
    w2_t[1] = w[ 9];
    w2_t[2] = w[10];
    w2_t[3] = w[11];
    w3_t[0] = w[12];
    w3_t[1] = w[13];
    w3_t[2] = w[14];
    w3_t[3] = w[15];

    u32x a = MD4M_A;
    u32x b = MD4M_B;
    u32x c = MD4M_C;
    u32x d = MD4M_D;

    MD4_STEP (MD4_Fo, a, b, c, d, w0_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w0_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w0_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w0_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w1_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w1_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w1_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w1_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w2_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w2_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w2_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w2_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w3_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w3_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w3_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w3_t[3], MD4C00, MD4S03);

    MD4_STEP (MD4_Go, a, b, c, d, w0_t[0], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[0], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[0], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[0], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[1], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[1], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[1], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[1], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[2], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[2], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[2], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[2], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[3], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[3], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[3], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[3], MD4C01, MD4S13);

    MD4_STEP (MD4_H , a, b, c, d, w0_t[0], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[0], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[0], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[0], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[2], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[2], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[2], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[2], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[1], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[1], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[1], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[1], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[3], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[3], MD4C02, MD4S21);

    if (MATCHES_NONE_VS (((d + make_u32x (MD4M_D)) >> 16), s2)) continue;

    MD4_STEP (MD4_H , c, d, a, b, w1_t[3], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[3], MD4C02, MD4S23);

    a += make_u32x (MD4M_A);
    b += make_u32x (MD4M_B);
    c += make_u32x (MD4M_C);
    d += make_u32x (MD4M_D);

    /**
     * DES1
     */

    u32x key[2];

    transform_netntlmv1_key (a, b, key);

    u32x Kc[16];
    u32x Kd[16];

    _des_crypt_keysetup_lm_vect (key[0], key[1], Kc, Kd, s_skb);

    u32x data[2];

    data[0] = s0;
    data[1] = s1;

    u32x iv1[2];

    _des_crypt_encrypt_lm_vect (iv1, data, Kc, Kd, s_SPtrans);

    /**
     * DES2
     */

    transform_netntlmv1_key (((b >> 24) | (c << 8)), ((c >> 24) | (d << 8)), key);

    _des_crypt_keysetup_lm_vect (key[0], key[1], Kc, Kd, s_skb);

    u32x iv2[2];

    _des_crypt_encrypt_lm_vect (iv2, data, Kc, Kd, s_SPtrans);

    /**
     * compare
     */

    COMPARE_M_SIMD (iv1[0], iv1[1], iv2[0], iv2[1]);
  }
}

DECLSPEC void m05500s (SHM_TYPE u32 (*s_SPtrans)[64], SHM_TYPE u32 (*s_skb)[64], PRIVATE_AS u32 *w, const u32 pw_len, KERN_ATTR_FUNC_VECTOR ())
{
  /**
   * modifiers are taken from args
   */

  /**
   * salt
   */

  const u32 s0 = salt_bufs[SALT_POS_HOST].salt_buf[0];
  const u32 s1 = salt_bufs[SALT_POS_HOST].salt_buf[1];
  const u32 s2 = salt_bufs[SALT_POS_HOST].salt_buf[2];

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
   * loop
   */

  u32 w0l = w[0];

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos += VECT_SIZE)
  {
    const u32x w0r = words_buf_r[il_pos / VECT_SIZE];

    const u32x w0 = w0l | w0r;

    u32x w0_t[4];
    u32x w1_t[4];
    u32x w2_t[4];
    u32x w3_t[4];

    w0_t[0] = w0;
    w0_t[1] = w[ 1];
    w0_t[2] = w[ 2];
    w0_t[3] = w[ 3];
    w1_t[0] = w[ 4];
    w1_t[1] = w[ 5];
    w1_t[2] = w[ 6];
    w1_t[3] = w[ 7];
    w2_t[0] = w[ 8];
    w2_t[1] = w[ 9];
    w2_t[2] = w[10];
    w2_t[3] = w[11];
    w3_t[0] = w[12];
    w3_t[1] = w[13];
    w3_t[2] = w[14];
    w3_t[3] = w[15];

    u32x a = MD4M_A;
    u32x b = MD4M_B;
    u32x c = MD4M_C;
    u32x d = MD4M_D;

    MD4_STEP (MD4_Fo, a, b, c, d, w0_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w0_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w0_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w0_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w1_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w1_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w1_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w1_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w2_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w2_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w2_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w2_t[3], MD4C00, MD4S03);
    MD4_STEP (MD4_Fo, a, b, c, d, w3_t[0], MD4C00, MD4S00);
    MD4_STEP (MD4_Fo, d, a, b, c, w3_t[1], MD4C00, MD4S01);
    MD4_STEP (MD4_Fo, c, d, a, b, w3_t[2], MD4C00, MD4S02);
    MD4_STEP (MD4_Fo, b, c, d, a, w3_t[3], MD4C00, MD4S03);

    MD4_STEP (MD4_Go, a, b, c, d, w0_t[0], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[0], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[0], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[0], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[1], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[1], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[1], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[1], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[2], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[2], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[2], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[2], MD4C01, MD4S13);
    MD4_STEP (MD4_Go, a, b, c, d, w0_t[3], MD4C01, MD4S10);
    MD4_STEP (MD4_Go, d, a, b, c, w1_t[3], MD4C01, MD4S11);
    MD4_STEP (MD4_Go, c, d, a, b, w2_t[3], MD4C01, MD4S12);
    MD4_STEP (MD4_Go, b, c, d, a, w3_t[3], MD4C01, MD4S13);

    MD4_STEP (MD4_H , a, b, c, d, w0_t[0], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[0], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[0], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[0], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[2], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[2], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[2], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[2], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[1], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[1], MD4C02, MD4S21);
    MD4_STEP (MD4_H , c, d, a, b, w1_t[1], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[1], MD4C02, MD4S23);
    MD4_STEP (MD4_H , a, b, c, d, w0_t[3], MD4C02, MD4S20);
    MD4_STEP (MD4_H , d, a, b, c, w2_t[3], MD4C02, MD4S21);

    if (MATCHES_NONE_VS (((d + make_u32x (MD4M_D)) >> 16), s2)) continue;

    MD4_STEP (MD4_H , c, d, a, b, w1_t[3], MD4C02, MD4S22);
    MD4_STEP (MD4_H , b, c, d, a, w3_t[3], MD4C02, MD4S23);

    a += make_u32x (MD4M_A);
    b += make_u32x (MD4M_B);
    c += make_u32x (MD4M_C);
    d += make_u32x (MD4M_D);

    /**
     * DES1
     */

    u32x key[2];

    transform_netntlmv1_key (a, b, key);

    u32x Kc[16];
    u32x Kd[16];

    _des_crypt_keysetup_lm_vect (key[0], key[1], Kc, Kd, s_skb);

    u32x data[2];

    data[0] = s0;
    data[1] = s1;

    u32x iv1[2];

    _des_crypt_encrypt_lm_vect (iv1, data, Kc, Kd, s_SPtrans);

    /**
     * DES2
     */

    /*
    transform_netntlmv1_key (((b >> 24) | (c << 8)), ((c >> 24) | (d << 8)), key);

    _des_crypt_keysetup_lm_vect (key[0], key[1], Kc, Kd, s_skb);

    u32x iv2[2];

    _des_crypt_encrypt_lm_vect (iv2, data, Kc, Kd, s_SPtrans);
    */

    u32x iv2[2];

    iv2[0] = search[2];
    iv2[1] = search[3];

    /**
     * compare
     */

    COMPARE_S_SIMD (iv1[0], iv1[1], iv2[0], iv2[1]);
  }
}

KERNEL_FQ KERNEL_FA void m05500_m04 (KERN_ATTR_VECTOR ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);
  const u64 lsz = get_local_size (0);

  /**
   * sbox, kbox
   */

  #ifdef REAL_SHM

  LOCAL_VK u32 s_SPtrans[8][64];
  LOCAL_VK u32 s_skb[8][64];

  for (u32 i = lid; i < 64; i += lsz)
  {
    s_SPtrans[0][i] = c_SPtrans[0][i];
    s_SPtrans[1][i] = c_SPtrans[1][i];
    s_SPtrans[2][i] = c_SPtrans[2][i];
    s_SPtrans[3][i] = c_SPtrans[3][i];
    s_SPtrans[4][i] = c_SPtrans[4][i];
    s_SPtrans[5][i] = c_SPtrans[5][i];
    s_SPtrans[6][i] = c_SPtrans[6][i];
    s_SPtrans[7][i] = c_SPtrans[7][i];

    s_skb[0][i] = c_skb[0][i];
    s_skb[1][i] = c_skb[1][i];
    s_skb[2][i] = c_skb[2][i];
    s_skb[3][i] = c_skb[3][i];
    s_skb[4][i] = c_skb[4][i];
    s_skb[5][i] = c_skb[5][i];
    s_skb[6][i] = c_skb[6][i];
    s_skb[7][i] = c_skb[7][i];
  }

  SYNC_THREADS ();

  #else

  CONSTANT_AS u32a (*s_SPtrans)[64] = c_SPtrans;
  CONSTANT_AS u32a (*s_skb)[64]     = c_skb;

  #endif

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = 0;
  w[ 5] = 0;
  w[ 6] = 0;
  w[ 7] = 0;
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len & 63;

  /**
   * main
   */

  m05500m (s_SPtrans, s_skb, w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_extra0_buf, d_extra1_buf, d_extra2_buf, d_extra3_buf, kernel_param, gid, lid, lsz);
}

KERNEL_FQ KERNEL_FA void m05500_m08 (KERN_ATTR_VECTOR ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);
  const u64 lsz = get_local_size (0);

  /**
   * sbox, kbox
   */

  #ifdef REAL_SHM

  LOCAL_VK u32 s_SPtrans[8][64];
  LOCAL_VK u32 s_skb[8][64];

  for (u32 i = lid; i < 64; i += lsz)
  {
    s_SPtrans[0][i] = c_SPtrans[0][i];
    s_SPtrans[1][i] = c_SPtrans[1][i];
    s_SPtrans[2][i] = c_SPtrans[2][i];
    s_SPtrans[3][i] = c_SPtrans[3][i];
    s_SPtrans[4][i] = c_SPtrans[4][i];
    s_SPtrans[5][i] = c_SPtrans[5][i];
    s_SPtrans[6][i] = c_SPtrans[6][i];
    s_SPtrans[7][i] = c_SPtrans[7][i];

    s_skb[0][i] = c_skb[0][i];
    s_skb[1][i] = c_skb[1][i];
    s_skb[2][i] = c_skb[2][i];
    s_skb[3][i] = c_skb[3][i];
    s_skb[4][i] = c_skb[4][i];
    s_skb[5][i] = c_skb[5][i];
    s_skb[6][i] = c_skb[6][i];
    s_skb[7][i] = c_skb[7][i];
  }

  SYNC_THREADS ();

  #else

  CONSTANT_AS u32a (*s_SPtrans)[64] = c_SPtrans;
  CONSTANT_AS u32a (*s_skb)[64]     = c_skb;

  #endif

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = pws[gid].i[ 4];
  w[ 5] = pws[gid].i[ 5];
  w[ 6] = pws[gid].i[ 6];
  w[ 7] = pws[gid].i[ 7];
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len & 63;

  /**
   * main
   */

  m05500m (s_SPtrans, s_skb, w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_extra0_buf, d_extra1_buf, d_extra2_buf, d_extra3_buf, kernel_param, gid, lid, lsz);
}

KERNEL_FQ KERNEL_FA void m05500_m16 (KERN_ATTR_VECTOR ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);
  const u64 lsz = get_local_size (0);

  /**
   * sbox, kbox
   */

  #ifdef REAL_SHM

  LOCAL_VK u32 s_SPtrans[8][64];
  LOCAL_VK u32 s_skb[8][64];

  for (u32 i = lid; i < 64; i += lsz)
  {
    s_SPtrans[0][i] = c_SPtrans[0][i];
    s_SPtrans[1][i] = c_SPtrans[1][i];
    s_SPtrans[2][i] = c_SPtrans[2][i];
    s_SPtrans[3][i] = c_SPtrans[3][i];
    s_SPtrans[4][i] = c_SPtrans[4][i];
    s_SPtrans[5][i] = c_SPtrans[5][i];
    s_SPtrans[6][i] = c_SPtrans[6][i];
    s_SPtrans[7][i] = c_SPtrans[7][i];

    s_skb[0][i] = c_skb[0][i];
    s_skb[1][i] = c_skb[1][i];
    s_skb[2][i] = c_skb[2][i];
    s_skb[3][i] = c_skb[3][i];
    s_skb[4][i] = c_skb[4][i];
    s_skb[5][i] = c_skb[5][i];
    s_skb[6][i] = c_skb[6][i];
    s_skb[7][i] = c_skb[7][i];
  }

  SYNC_THREADS ();

  #else

  CONSTANT_AS u32a (*s_SPtrans)[64] = c_SPtrans;
  CONSTANT_AS u32a (*s_skb)[64]     = c_skb;

  #endif

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = pws[gid].i[ 4];
  w[ 5] = pws[gid].i[ 5];
  w[ 6] = pws[gid].i[ 6];
  w[ 7] = pws[gid].i[ 7];
  w[ 8] = pws[gid].i[ 8];
  w[ 9] = pws[gid].i[ 9];
  w[10] = pws[gid].i[10];
  w[11] = pws[gid].i[11];
  w[12] = pws[gid].i[12];
  w[13] = pws[gid].i[13];
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len & 63;

  /**
   * main
   */

  m05500m (s_SPtrans, s_skb, w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_extra0_buf, d_extra1_buf, d_extra2_buf, d_extra3_buf, kernel_param, gid, lid, lsz);
}

KERNEL_FQ KERNEL_FA void m05500_s04 (KERN_ATTR_VECTOR ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);
  const u64 lsz = get_local_size (0);

  /**
   * sbox, kbox
   */

  #ifdef REAL_SHM

  LOCAL_VK u32 s_SPtrans[8][64];
  LOCAL_VK u32 s_skb[8][64];

  for (u32 i = lid; i < 64; i += lsz)
  {
    s_SPtrans[0][i] = c_SPtrans[0][i];
    s_SPtrans[1][i] = c_SPtrans[1][i];
    s_SPtrans[2][i] = c_SPtrans[2][i];
    s_SPtrans[3][i] = c_SPtrans[3][i];
    s_SPtrans[4][i] = c_SPtrans[4][i];
    s_SPtrans[5][i] = c_SPtrans[5][i];
    s_SPtrans[6][i] = c_SPtrans[6][i];
    s_SPtrans[7][i] = c_SPtrans[7][i];

    s_skb[0][i] = c_skb[0][i];
    s_skb[1][i] = c_skb[1][i];
    s_skb[2][i] = c_skb[2][i];
    s_skb[3][i] = c_skb[3][i];
    s_skb[4][i] = c_skb[4][i];
    s_skb[5][i] = c_skb[5][i];
    s_skb[6][i] = c_skb[6][i];
    s_skb[7][i] = c_skb[7][i];
  }

  SYNC_THREADS ();

  #else

  CONSTANT_AS u32a (*s_SPtrans)[64] = c_SPtrans;
  CONSTANT_AS u32a (*s_skb)[64]     = c_skb;

  #endif

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = 0;
  w[ 5] = 0;
  w[ 6] = 0;
  w[ 7] = 0;
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len & 63;

  /**
   * main
   */

  m05500s (s_SPtrans, s_skb, w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_extra0_buf, d_extra1_buf, d_extra2_buf, d_extra3_buf, kernel_param, gid, lid, lsz);
}

KERNEL_FQ KERNEL_FA void m05500_s08 (KERN_ATTR_VECTOR ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);
  const u64 lsz = get_local_size (0);

  /**
   * sbox, kbox
   */

  #ifdef REAL_SHM

  LOCAL_VK u32 s_SPtrans[8][64];
  LOCAL_VK u32 s_skb[8][64];

  for (u32 i = lid; i < 64; i += lsz)
  {
    s_SPtrans[0][i] = c_SPtrans[0][i];
    s_SPtrans[1][i] = c_SPtrans[1][i];
    s_SPtrans[2][i] = c_SPtrans[2][i];
    s_SPtrans[3][i] = c_SPtrans[3][i];
    s_SPtrans[4][i] = c_SPtrans[4][i];
    s_SPtrans[5][i] = c_SPtrans[5][i];
    s_SPtrans[6][i] = c_SPtrans[6][i];
    s_SPtrans[7][i] = c_SPtrans[7][i];

    s_skb[0][i] = c_skb[0][i];
    s_skb[1][i] = c_skb[1][i];
    s_skb[2][i] = c_skb[2][i];
    s_skb[3][i] = c_skb[3][i];
    s_skb[4][i] = c_skb[4][i];
    s_skb[5][i] = c_skb[5][i];
    s_skb[6][i] = c_skb[6][i];
    s_skb[7][i] = c_skb[7][i];
  }

  SYNC_THREADS ();

  #else

  CONSTANT_AS u32a (*s_SPtrans)[64] = c_SPtrans;
  CONSTANT_AS u32a (*s_skb)[64]     = c_skb;

  #endif

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = pws[gid].i[ 4];
  w[ 5] = pws[gid].i[ 5];
  w[ 6] = pws[gid].i[ 6];
  w[ 7] = pws[gid].i[ 7];
  w[ 8] = 0;
  w[ 9] = 0;
  w[10] = 0;
  w[11] = 0;
  w[12] = 0;
  w[13] = 0;
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len & 63;

  /**
   * main
   */

  m05500s (s_SPtrans, s_skb, w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_extra0_buf, d_extra1_buf, d_extra2_buf, d_extra3_buf, kernel_param, gid, lid, lsz);
}

KERNEL_FQ KERNEL_FA void m05500_s16 (KERN_ATTR_VECTOR ())
{
  /**
   * modifier
   */

  const u64 lid = get_local_id (0);
  const u64 gid = get_global_id (0);
  const u64 lsz = get_local_size (0);

  /**
   * sbox, kbox
   */

  #ifdef REAL_SHM

  LOCAL_VK u32 s_SPtrans[8][64];
  LOCAL_VK u32 s_skb[8][64];

  for (u32 i = lid; i < 64; i += lsz)
  {
    s_SPtrans[0][i] = c_SPtrans[0][i];
    s_SPtrans[1][i] = c_SPtrans[1][i];
    s_SPtrans[2][i] = c_SPtrans[2][i];
    s_SPtrans[3][i] = c_SPtrans[3][i];
    s_SPtrans[4][i] = c_SPtrans[4][i];
    s_SPtrans[5][i] = c_SPtrans[5][i];
    s_SPtrans[6][i] = c_SPtrans[6][i];
    s_SPtrans[7][i] = c_SPtrans[7][i];

    s_skb[0][i] = c_skb[0][i];
    s_skb[1][i] = c_skb[1][i];
    s_skb[2][i] = c_skb[2][i];
    s_skb[3][i] = c_skb[3][i];
    s_skb[4][i] = c_skb[4][i];
    s_skb[5][i] = c_skb[5][i];
    s_skb[6][i] = c_skb[6][i];
    s_skb[7][i] = c_skb[7][i];
  }

  SYNC_THREADS ();

  #else

  CONSTANT_AS u32a (*s_SPtrans)[64] = c_SPtrans;
  CONSTANT_AS u32a (*s_skb)[64]     = c_skb;

  #endif

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  u32 w[16];

  w[ 0] = pws[gid].i[ 0];
  w[ 1] = pws[gid].i[ 1];
  w[ 2] = pws[gid].i[ 2];
  w[ 3] = pws[gid].i[ 3];
  w[ 4] = pws[gid].i[ 4];
  w[ 5] = pws[gid].i[ 5];
  w[ 6] = pws[gid].i[ 6];
  w[ 7] = pws[gid].i[ 7];
  w[ 8] = pws[gid].i[ 8];
  w[ 9] = pws[gid].i[ 9];
  w[10] = pws[gid].i[10];
  w[11] = pws[gid].i[11];
  w[12] = pws[gid].i[12];
  w[13] = pws[gid].i[13];
  w[14] = pws[gid].i[14];
  w[15] = 0;

  const u32 pw_len = pws[gid].pw_len & 63;

  /**
   * main
   */

  m05500s (s_SPtrans, s_skb, w, pw_len, pws, rules_buf, combs_buf, words_buf_r, tmps, hooks, bitmaps_buf_s1_a, bitmaps_buf_s1_b, bitmaps_buf_s1_c, bitmaps_buf_s1_d, bitmaps_buf_s2_a, bitmaps_buf_s2_b, bitmaps_buf_s2_c, bitmaps_buf_s2_d, plains_buf, digests_buf, hashes_shown, salt_bufs, esalt_bufs, d_return_buf, d_extra0_buf, d_extra1_buf, d_extra2_buf, d_extra3_buf, kernel_param, gid, lid, lsz);
}
