/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#ifndef INC_CHECKSUM_CRC_H
#define INC_CHECKSUM_CRC_H

DECLSPEC u32x crc32              (PRIVATE_AS   const u32x *w, const u32 pw_len, const u32 iv);
DECLSPEC u32x crc32_opti         (PRIVATE_AS   const u32x *w, const u32 pw_len, const u32 iv);
DECLSPEC u32x crc32_global       (GLOBAL_AS    const u32x *w, const u32 pw_len, const u32 iv);

DECLSPEC u32x crc32c             (PRIVATE_AS   const u32x *w, const u32 pw_len, const u32 iv);
DECLSPEC u32x crc32c_opti        (PRIVATE_AS   const u32x *w, const u32 pw_len, const u32 iv);
DECLSPEC u32x crc32c_global      (GLOBAL_AS    const u32x *w, const u32 pw_len, const u32 iv);

DECLSPEC u64  crc64j             (PRIVATE_AS const u32 *w, const u32 pw_len, const u64 iv, SHM_TYPE u64 *crc64table);
DECLSPEC u64  crc64j_opti        (PRIVATE_AS const u32 *w, const u32 pw_len, const u64 iv, SHM_TYPE u64 *crc64table);
DECLSPEC u64  crc64j_global      (GLOBAL_AS  const u32 *w, const u32 pw_len, const u64 iv, SHM_TYPE u64 *crc64table);

DECLSPEC u32  round_crc32_16_S   (const u32 crc32, PRIVATE_AS const u32 *buf, const u32 len, LOCAL_AS u32 *crc32table);

#endif // INC_CHECKSUM_CRC_H
