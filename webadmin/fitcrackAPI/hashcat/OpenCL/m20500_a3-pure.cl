/*

PKZIP Kernels for Hashcat (c) 2018, European Union

PKZIP Kernels for Hashcat has been developed by the Joint Research Centre of the European Commission.
It is released as open source software under the MIT License.

PKZIP Kernels for Hashcat makes use of a primary external components, which continue to be subject
to the terms and conditions stipulated in the respective licences they have been released under. These
external components include, but are not necessarily limited to, the following:

-----

1. Hashcat: MIT License

Copyright (c) 2015-2018 Jens Steube

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

-----

The European Union disclaims all liability related to or arising out of the use made by third parties of
any external components and dependencies which may be included with PKZIP Kernels for Hashcat.

-----

The MIT License

Copyright (c) 2018, EUROPEAN UNION

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without
limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Author:              Sein Coray
Related publication: https://scitepress.org/PublicationsDetail.aspx?ID=KLPzPqStp5g=

*/

  /* for fixed length 3
  for (u32 pos = pw_len - 1; pos >= 3; pos--)
  {
    const u32 t = hc_bfe_S (pws[gid].i[pos / 4], (pos & 3) * 8, 8);

    inv_update_key012 (prep0, prep1, prep2, t, l_icrc32tab);
  }

  inv_update_key012 (prep0, prep1, prep2, 0, l_icrc32tab);

  prep2 = INVCRC32 (prep2, MSB (prep1), l_icrc32tab);
  prep1 = (prep1 - 1) * INVCONST;

    key0 = key0 ^ w0c;

    key1 = key1 - (key0 & 0xff);
    key0 = INVCRC32 (key0, w0b, l_icrc32tab);

    inv_update_key012 (key0, key1, key2, w0a, l_icrc32tab);
  */

#include M2S(INCLUDE_PATH/inc_vendor.h)
#include M2S(INCLUDE_PATH/inc_types.h)
#include M2S(INCLUDE_PATH/inc_platform.cl)
#include M2S(INCLUDE_PATH/inc_common.cl)
#include M2S(INCLUDE_PATH/inc_simd.cl)
#include M2S(INCLUDE_PATH/inc_checksum_crc.cl)

#define MSB(x)          ((x) >> 24)
#define CRC32(x,c,t)    (((x) >> 8) ^ (t)[((x) ^ (c)) & 0xff])
#define CONST           0x08088405
#define INVCRC32(x,c,t) (((x) << 8) ^ (t)[(x) >> 24] ^ ((c) & 0xff))
#define INVCONST        0xd94fa8cd
#define KEY0INIT        0x12345678
#define KEY1INIT        0x23456789
#define KEY2INIT        0x34567890

#define inv_update_key012(k0,k1,k2,c,t)         \
  (k2) = INVCRC32 ((k2), MSB (k1), (t));        \
  (k1) = ((k1) - 1) * INVCONST - ((k0) & 0xff); \
  (k0) = INVCRC32 ((k0), (c), (t));

#define update_key012(k0,k1,k2,c,t)           \
{                                             \
  (k0) = CRC32 ((k0), c, (t));                \
  (k1) = ((k1) + ((k0) & 0xff)) * CONST + 1;  \
  (k2) = CRC32 ((k2), MSB (k1), (t));         \
}

KERNEL_FQ KERNEL_FA void m20500_sxx (KERN_ATTR_VECTOR ())
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

  LOCAL_VK u32 l_icrc32tab[256];

  for (u64 i = lid; i < 256; i += lsz)
  {
    l_icrc32tab[i] = icrc32tab[i];
  }

  SYNC_THREADS ();

  if (gid >= GID_CNT) return;

  /**
   * digest
   */

  const u32 search[4] =
  {
    KEY0INIT, // static initial values
    KEY1INIT, // should remain unchanged
    KEY2INIT,
    0
  };

  /**
   * base
   */

  const u32 pw_len = pws[gid].pw_len;

  /**
   * reverse
   */

  u32 prep0 = digests_buf[DIGESTS_OFFSET_HOST].digest_buf[0];
  u32 prep1 = digests_buf[DIGESTS_OFFSET_HOST].digest_buf[1];
  u32 prep2 = digests_buf[DIGESTS_OFFSET_HOST].digest_buf[2];

  for (int pos = pw_len - 1; pos >= 4; pos--)
  {
    const u32 t = hc_bfe_S (pws[gid].i[pos / 4], (pos & 3) * 8, 8);

    inv_update_key012 (prep0, prep1, prep2, t, l_icrc32tab);
  }

  if (pw_len >= 4)
  {
    inv_update_key012 (prep0, prep1, prep2, 0, l_icrc32tab);

    prep2 = INVCRC32 (prep2, MSB (prep1), l_icrc32tab);
    prep1 = (prep1 - 1) * INVCONST;
  }

  /**
   * loop
   */

  u32 w0l = pws[gid].i[0];

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos += VECT_SIZE)
  {
    const u32x w0r = words_buf_r[il_pos / VECT_SIZE];

    const u32x w0 = w0l | w0r;

    u32x key0 = prep0;
    u32x key1 = prep1;
    u32x key2 = prep2;

    const u32x w0a = unpack_v8a_from_v32_S (w0);
    const u32x w0b = unpack_v8b_from_v32_S (w0);
    const u32x w0c = unpack_v8c_from_v32_S (w0);
    const u32x w0d = unpack_v8d_from_v32_S (w0);

    if (pw_len >= 4) key0 = key0 ^ w0d;

    if (pw_len >= 3)
    {
      key1 = key1 - (key0 & 0xff);
      key0 = INVCRC32 (key0, w0c, l_icrc32tab);
    }

    if (pw_len >= 2)
    {
      inv_update_key012 (key0, key1, key2, w0b, l_icrc32tab);
    }

    if (pw_len >= 1)
    {
      inv_update_key012 (key0, key1, key2, w0a, l_icrc32tab);
    }

    const u32x r0 = key0;
    const u32x r1 = key1;
    const u32x r2 = key2;
    const u32x r3 = 0;

    COMPARE_S_SIMD (r0, r1, r2, r3);
  }
}

KERNEL_FQ KERNEL_FA void m20500_mxx (KERN_ATTR_VECTOR ())
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

  LOCAL_VK u32 l_crc32tab[256];

  for (u64 i = lid; i < 256; i += lsz)
  {
    l_crc32tab[i] = crc32tab[i];
  }

  SYNC_THREADS ();

  if (gid >= GID_CNT) return;

  /**
   * base
   */

  const u32 pw_len = pws[gid].pw_len;

  u32x w[64] = { 0 };

  for (int i = 0, idx = 0; i < pw_len; i += 4, idx += 1)
  {
    w[idx] = pws[gid].i[idx];
  }

  /**
   * loop
   */

  u32 w0l =  w[0];

  for (u32 il_pos = 0; il_pos < IL_CNT; il_pos += VECT_SIZE)
  {
    const u32x w0r = words_buf_r[il_pos / VECT_SIZE];

    const u32x w0 = w0l | w0r;

    u32x key0 = KEY0INIT;
    u32x key1 = KEY1INIT;
    u32x key2 = KEY2INIT;

    if (pw_len >=  1) update_key012 (key0, key1, key2, unpack_v8a_from_v32_S (w0), l_crc32tab);
    if (pw_len >=  2) update_key012 (key0, key1, key2, unpack_v8b_from_v32_S (w0), l_crc32tab);
    if (pw_len >=  3) update_key012 (key0, key1, key2, unpack_v8c_from_v32_S (w0), l_crc32tab);
    if (pw_len >=  4) update_key012 (key0, key1, key2, unpack_v8d_from_v32_S (w0), l_crc32tab);

    for (u32 i = 4, j = 1; i < pw_len; i += 4, j += 1)
    {
      if (pw_len >= (i + 1)) update_key012 (key0, key1, key2, unpack_v8a_from_v32_S (w[j]), l_crc32tab);
      if (pw_len >= (i + 2)) update_key012 (key0, key1, key2, unpack_v8b_from_v32_S (w[j]), l_crc32tab);
      if (pw_len >= (i + 3)) update_key012 (key0, key1, key2, unpack_v8c_from_v32_S (w[j]), l_crc32tab);
      if (pw_len >= (i + 4)) update_key012 (key0, key1, key2, unpack_v8d_from_v32_S (w[j]), l_crc32tab);
    }

    const u32x r0 = key0;
    const u32x r1 = key1;
    const u32x r2 = key2;
    const u32x r3 = 0;

    COMPARE_M_SIMD (r0, r1, r2, r3);
  }
}

#undef inv_update_key012
#undef INVCONST
#undef INVCRC32
#undef MSB
#undef KEY0INIT
#undef KEY1INIT
#undef KEY2INIT
