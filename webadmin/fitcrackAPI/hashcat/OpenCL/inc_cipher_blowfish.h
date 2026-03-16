/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#ifndef INC_CIPHER_BLOWFISH_H
#define INC_CIPHER_BLOWFISH_H

#ifndef FIXED_LOCAL_SIZE
#ifdef FIXED_LOCAL_SIZE_COMP
#define FIXED_LOCAL_SIZE FIXED_LOCAL_SIZE_COMP
#endif
#endif

DECLSPEC void expand_key            (PRIVATE_AS u32 *E, PRIVATE_AS u32 *W, const int len);
DECLSPEC void blowfish_set_key      (PRIVATE_AS u32 *E, PRIVATE_AS u32 E_dim_size, PRIVATE_AS u32 *P, LOCAL_AS u32 *S0, LOCAL_AS u32 *S1, LOCAL_AS u32 *S2, LOCAL_AS u32 *S3);
DECLSPEC void blowfish_set_key_salt (PRIVATE_AS u32 *E, PRIVATE_AS u32 E_dim_size, PRIVATE_AS u32 *salt_buf, PRIVATE_AS u32 *P, LOCAL_AS u32 *S0, LOCAL_AS u32 *S1, LOCAL_AS u32 *S2, LOCAL_AS u32 *S3);
DECLSPEC void blowfish_encrypt      (PRIVATE_AS u32 *P, LOCAL_AS u32 *S0, LOCAL_AS u32 *S1, LOCAL_AS u32 *S2, LOCAL_AS u32 *S3);

#endif // INC_CIPHER_BLOWFISH_H
