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
#include M2S(INCLUDE_PATH/inc_simd.cl)
#include M2S(INCLUDE_PATH/inc_cipher_des.cl)
#endif

KERNEL_FQ KERNEL_FA void m01500_mxx (KERN_ATTR_RULES ())
{
  /**
   * modifier
   */

  const u64 gid = get_global_id (0);
  const u64 lid = get_local_id (0);
  const u64 lsz = get_local_size (0);

  /**
   * sbox, kbox
   */

  #ifdef REAL_SHM

  LOCAL_VK u32 s_SPtrans[8][64];
  LOCAL_VK u32 s_skb[8][64];

  for (u32 i = lid; i < 64; i += lsz)
  {
    s_SPtrans[0][i] = c_SPtrans_opti[0][i];
    s_SPtrans[1][i] = c_SPtrans_opti[1][i];
    s_SPtrans[2][i] = c_SPtrans_opti[2][i];
    s_SPtrans[3][i] = c_SPtrans_opti[3][i];
    s_SPtrans[4][i] = c_SPtrans_opti[4][i];
    s_SPtrans[5][i] = c_SPtrans_opti[5][i];
    s_SPtrans[6][i] = c_SPtrans_opti[6][i];
    s_SPtrans[7][i] = c_SPtrans_opti[7][i];

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

  CONSTANT_AS u32a (*s_SPtrans)[64] = c_SPtrans_opti;
  CONSTANT_AS u32a (*s_skb)[64]     = c_skb;

  #endif

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  COPY_PW (pws[gid]);

  /**
   * salt
   */

  const u32 mask = salt_bufs[SALT_POS_HOST].salt_buf[0];

  /**
   * main
   */

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos += VECT_SIZE)
  {
    pw_t tmp = PASTE_PW;

    tmp.pw_len = apply_rules (rules_buf[il_pos].cmds, tmp.i, tmp.pw_len);

    u32 data[2];

    data[0] = (tmp.i[0] << 1) & 0xfefefefe;
    data[1] = (tmp.i[1] << 1) & 0xfefefefe;

    u32 Kc[16];
    u32 Kd[16];

    _des_crypt_keysetup_opti (data[0], data[1], Kc, Kd, s_skb);

    u32 iv[2];

    _des_crypt_encrypt_mask (iv, mask, Kc, Kd, s_SPtrans);

    u32 z = 0;

    COMPARE_M_SIMD (iv[0], iv[1], z, z);
  }
}

KERNEL_FQ KERNEL_FA void m01500_sxx (KERN_ATTR_RULES ())
{
  /**
   * modifier
   */

  const u64 gid = get_global_id (0);
  const u64 lid = get_local_id (0);
  const u64 lsz = get_local_size (0);

  /**
   * sbox, kbox
   */

  #ifdef REAL_SHM

  LOCAL_VK u32 s_SPtrans[8][64];
  LOCAL_VK u32 s_skb[8][64];

  for (u32 i = lid; i < 64; i += lsz)
  {
    s_SPtrans[0][i] = c_SPtrans_opti[0][i];
    s_SPtrans[1][i] = c_SPtrans_opti[1][i];
    s_SPtrans[2][i] = c_SPtrans_opti[2][i];
    s_SPtrans[3][i] = c_SPtrans_opti[3][i];
    s_SPtrans[4][i] = c_SPtrans_opti[4][i];
    s_SPtrans[5][i] = c_SPtrans_opti[5][i];
    s_SPtrans[6][i] = c_SPtrans_opti[6][i];
    s_SPtrans[7][i] = c_SPtrans_opti[7][i];

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

  CONSTANT_AS u32a (*s_SPtrans)[64] = c_SPtrans_opti;
  CONSTANT_AS u32a (*s_skb)[64]     = c_skb;

  #endif

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  COPY_PW (pws[gid]);

  /**
   * salt
   */

  const u32 mask = salt_bufs[SALT_POS_HOST].salt_buf[0];

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
   * main
   */

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos += VECT_SIZE)
  {
    pw_t tmp = PASTE_PW;

    tmp.pw_len = apply_rules (rules_buf[il_pos].cmds, tmp.i, tmp.pw_len);

    u32 data[2];

    data[0] = (tmp.i[0] << 1) & 0xfefefefe;
    data[1] = (tmp.i[1] << 1) & 0xfefefefe;

    u32 Kc[16];
    u32 Kd[16];

    _des_crypt_keysetup_opti (data[0], data[1], Kc, Kd, s_skb);

    u32 iv[2];

    _des_crypt_encrypt_mask (iv, mask, Kc, Kd, s_SPtrans);

    u32 z = 0;

    COMPARE_S_SIMD (iv[0], iv[1], z, z);
  }
}
