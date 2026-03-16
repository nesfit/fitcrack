/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#ifndef INC_HASH_SCRYPT_H
#define INC_HASH_SCRYPT_H

#define GET_SCRYPT_SZ(r,p) (128 * (r) * (p))
#define GET_STATE_SZ(r)    (128 * (r))

// _SZ is true sizes as bytes
#define SCRYPT_SZ  GET_SCRYPT_SZ (SCRYPT_R, SCRYPT_P)
#define STATE_SZ   GET_STATE_SZ  (SCRYPT_R)

// _CNT is size as whatever /X datatype
#define SCRYPT_CNT4  (SCRYPT_SZ / 4)
#define STATE_CNT4   (STATE_SZ  / 4)

// this would be uint4, feels more natural than 16
#define SCRYPT_CNT44 ((SCRYPT_SZ / 4) / 4)
#define STATE_CNT44  ((STATE_SZ  / 4) / 4)

#define SALSA_SZ   64
#define SALSA_CNT4 (SALSA_SZ / 4)
#define SALSA_CNT44 ((SALSA_SZ / 4) / 4)

// should be safe, because in backend.c we use:
//    u64 size_extra_buffer1 = 4096;
//  size_extra_buffer1 += base_chunk_size;
// could be useless, pointers seem to be page aligned
//#define ALIGN_PTR_1k(p) ((GLOBAL_AS hc_uint4_t *) (((u64) (p) + 1023) & ~1023UL))

#if defined IS_INTEL_SDK

typedef struct
{
  u32 x, y, z, w;

} hc_uint4_t;

#else

typedef uint4 hc_uint4_t;

#endif

DECLSPEC hc_uint4_t xor_uint4 (const hc_uint4_t a, const hc_uint4_t b);

typedef struct
{
  #ifndef SCRYPT_TMP_ELEM
  #define SCRYPT_TMP_ELEM 1
  #endif

  u32 in[SCRYPT_TMP_ELEM / 2];
  u32 out[SCRYPT_TMP_ELEM / 2];

} scrypt_tmp_t;

#endif
