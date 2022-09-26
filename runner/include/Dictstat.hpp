/**
 * Author: David Bolvansky
 * Based on hashcat's dictstat.c
 * https://github.com/hashcat/hashcat/blob/master/src/dictstat.c
 * 
**/

#ifndef DICTSTAT_HPP
#define DICTSTAT_HPP

#include "SHA1.hpp"

#include <cstdint>

#define DICTSTAT_FILENAME "hashcat.dictstat2"
#define DICTSTAT_VERSION (0x6863646963743200 | 0x02)

#if defined(__unix__) || defined(__APPLE__)
#define _POSIX
#elif defined(_WIN32)
#define _WIN 1
#else
#error Your Operating System is not supported or detected
#endif

#if defined(__linux__)
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 5, 48)
#define STAT_NANOSECONDS_ACCESS_TIME st_atim.tv_nsec
#endif
#endif

#if defined(__APPLE__)
#define STAT_NANOSECONDS_ACCESS_TIME st_atimespec.tv_nsec
#endif

#if defined(__linux__)
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 5, 48)
#define STAT_NANOSECONDS_ACCESS_TIME st_atim.tv_nsec
#endif
#endif

class DictStatBuilder {
  FILE *fp;

public:
  DictStatBuilder(const char *outfile = DICTSTAT_FILENAME) {
    fp = fopen(outfile, "wb");
    if (!fp)
      return;

    uint64_t v = DICTSTAT_VERSION;
    uint64_t z = 0;

    v = __builtin_bswap64(v);
    z = __builtin_bswap64(z);

    fwrite(&v, sizeof(uint64_t), 1, fp);
    fwrite(&z, sizeof(uint64_t), 1, fp);
  }

  bool addStatForDict(const char *filename, uint64_t passwords) {
    struct dictstat {
      uint64_t cnt;

      struct stat stat;

      char encoding_from[64];
      char encoding_to[64];

      uint8_t hash_filename[16];
    };

    struct dictstat d {};

    strcpy(d.encoding_from, "utf-8");
    strcpy(d.encoding_to, "utf-8");

    d.cnt = passwords;

    if (stat(filename, &d.stat) == -1)
      return false;

    d.stat.st_mode = 0;
    d.stat.st_nlink = 0;
    d.stat.st_uid = 0;
    d.stat.st_gid = 0;
    d.stat.st_rdev = 0;
    d.stat.st_atime = 0;

#if defined(STAT_NANOSECONDS_ACCESS_TIME)
    d.stat.STAT_NANOSECONDS_ACCESS_TIME = 0;
#endif

#if defined(_POSIX)
    d.stat.st_blksize = 0;
    d.stat.st_blocks = 0;
#endif

    unsigned char buf[20]{};
    sha1::calc(filename, strlen(filename), buf);
    memcpy(d.hash_filename, buf, 16);

    fwrite(&d, sizeof(struct dictstat), 1, fp);
    return true;
  }

  ~DictStatBuilder(void) {
    if (fp)
      fclose(fp);
  }
};

#endif // DICTSTAT_HPP