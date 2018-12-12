/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#include "common.h"
#include "types.h"
#include "memory.h"
#include "event.h"
#include "filehandling.h"
#include "hlfmt.h"                       
#include "interface.h"
#include "terminal.h"
#include "shared.h"
#include "hashValidator.h" 

int sort_by_digest_p0p1 (const void *v1, const void *v2, void *v3)
{  
  const u32 *d1 = (const u32 *) v1; 
  const u32 *d2 = (const u32 *) v2;

  hashconfig_t *hashconfig = (hashconfig_t *) v3;

  const u32 dgst_pos0 = hashconfig->dgst_pos0;
  const u32 dgst_pos1 = hashconfig->dgst_pos1;
  const u32 dgst_pos2 = hashconfig->dgst_pos2;
  const u32 dgst_pos3 = hashconfig->dgst_pos3;

  if (d1[dgst_pos3] > d2[dgst_pos3]) return  1;
  if (d1[dgst_pos3] < d2[dgst_pos3]) return -1;
  if (d1[dgst_pos2] > d2[dgst_pos2]) return  1;
  if (d1[dgst_pos2] < d2[dgst_pos2]) return -1;
  if (d1[dgst_pos1] > d2[dgst_pos1]) return  1;
  if (d1[dgst_pos1] < d2[dgst_pos1]) return -1;
  if (d1[dgst_pos0] > d2[dgst_pos0]) return  1;
  if (d1[dgst_pos0] < d2[dgst_pos0]) return -1;

  return 0;
}

int sort_by_salt (const void *v1, const void *v2)
{
  const salt_t *s1 = (const salt_t *) v1;
  const salt_t *s2 = (const salt_t *) v2;

  const int res1 = (int) s1->salt_len - (int) s2->salt_len;

  if (res1 != 0) return (res1);

  const int res2 = (int) s1->salt_iter - (int) s2->salt_iter;

  if (res2 != 0) return (res2);

  for (int n = 0; n < 64; n++)
  {
    if (s1->salt_buf[n] > s2->salt_buf[n]) return  1;
    if (s1->salt_buf[n] < s2->salt_buf[n]) return -1;
  }

  for (int n = 0; n < 64; n++)
  {
    if (s1->salt_buf_pc[n] > s2->salt_buf_pc[n]) return  1;
    if (s1->salt_buf_pc[n] < s2->salt_buf_pc[n]) return -1;
  }

  return 0;
}

int sort_by_hash (const void *v1, const void *v2, void *v3)
{
  const hash_t *h1 = (const hash_t *) v1;
  const hash_t *h2 = (const hash_t *) v2;

  hashconfig_t *hashconfig = (hashconfig_t *) v3;

  if (hashconfig->is_salted == true)
  {
    const salt_t *s1 = h1->salt;
    const salt_t *s2 = h2->salt;

    int res = sort_by_salt (s1, s2);

    if (res != 0) return (res);
  }

  const void *d1 = h1->digest;
  const void *d2 = h2->digest;

  return sort_by_digest_p0p1 (d1, d2, v3);
}

int sort_by_hash_no_salt (const void *v1, const void *v2, void *v3)
{
  const hash_t *h1 = (const hash_t *) v1;
  const hash_t *h2 = (const hash_t *) v2;

  const void *d1 = h1->digest;
  const void *d2 = h2->digest;

  return sort_by_digest_p0p1 (d1, d2, v3);
}


void hashes_init_filename (hashcat_ctx_t *hashcat_ctx)
{
  hashconfig_t         *hashconfig         = hashcat_ctx->hashconfig;
  hashes_t             *hashes             = hashcat_ctx->hashes;
  user_options_t       *user_options       = hashcat_ctx->user_options;
  user_options_extra_t *user_options_extra = hashcat_ctx->user_options_extra;

  char *hash_or_file = user_options_extra->hc_hash;

  /**
   * load hashes, part I: find input mode, count hashes
   */

  if ((user_options->benchmark == false) && (user_options->stdout_flag == false) && (user_options->keyspace == false))
  {
    if (hashconfig->opts_type & OPTS_TYPE_BINARY_HASHFILE)
    {
      hashes->hashlist_mode = HL_MODE_ARG;

      hashes->hashfile = hash_or_file;

      hc_asprintf (&hashes->hashfile_hcdmp, "%s.hcdmp", hashes->hashfile);
    }
    else
    {
      hashes->hashlist_mode = (hc_path_exist (hash_or_file) == true) ? HL_MODE_FILE : HL_MODE_ARG;

      if (hashes->hashlist_mode == HL_MODE_FILE)
      {
        hashes->hashfile = hash_or_file;

        hc_asprintf (&hashes->hashfile_hcdmp, "%s.hcdmp", hashes->hashfile);
      }
    }
  }
  else
  {
    hashes->hashlist_mode = HL_MODE_ARG;
  }
}

int hashes_init_stage1 (hashcat_ctx_t *hashcat_ctx)
{
  hashconfig_t          *hashconfig         = hashcat_ctx->hashconfig;
  hashes_t              *hashes             = hashcat_ctx->hashes;
  user_options_t        *user_options       = hashcat_ctx->user_options;
  user_options_extra_t  *user_options_extra = hashcat_ctx->user_options_extra;

  /**
   * load hashes, part I: find input mode, count hashes
   */

  const char *hashfile      = hashes->hashfile;
  const u32   hashlist_mode = hashes->hashlist_mode;

  u32 hashlist_format = HLFMT_HASHCAT;

  u64 hashes_avail = 0;

  if ((user_options->benchmark == false) && (user_options->stdout_flag == false) && (user_options->keyspace == false))
  {
    if (hashlist_mode == HL_MODE_ARG)
    {
      if ((hashconfig->hash_mode == 2500) || (hashconfig->hash_mode == 2501))
      {
        struct stat st;

        if (stat (hashes->hashfile, &st) == -1)
        {
          event_log_error (hashcat_ctx, "%s: %s", hashes->hashfile, strerror (errno));

          return -1;
        }

        // 392 = old hccap_t size

        if ((st.st_size % 392) == 0)
        {
          const int rc = check_old_hccap (hashes->hashfile);

          if (rc == 1)
          {
            event_log_error (hashcat_ctx, "%s: Old hccap format detected! You need to update: https://hashcat.net/q/hccapx", hashes->hashfile);

            return -1;
          }
        }

        hashes_avail = st.st_size / sizeof (hccapx_t);
      }
      else if (hashconfig->hash_mode == 14600)
      {
        struct stat st;

        if (stat (hashes->hashfile, &st) == -1)
        {
          event_log_error (hashcat_ctx, "%s: %s", hashes->hashfile, strerror (errno));

          return -1;
        }

        hashes_avail = LUKS_NUMKEYS;
      }
      else
      {
        hashes_avail = 1;
      }
    }
    else if (hashlist_mode == HL_MODE_FILE)
    {
      FILE *fp = NULL;

      if ((fp = fopen (hashfile, "rb")) == NULL)
      {
        event_log_error (hashcat_ctx, "%s: %s", hashfile, strerror (errno));

        return -1;
      }

      //EVENT_DATA (EVENT_HASHLIST_COUNT_LINES_PRE, hashfile, strlen (hashfile));

      hashes_avail = count_lines (fp);

      //EVENT_DATA (EVENT_HASHLIST_COUNT_LINES_POST, hashfile, strlen (hashfile));

      rewind (fp);

      if (hashes_avail == 0)
      {
        event_log_error (hashcat_ctx, "hashfile is empty or corrupt.");

        fclose (fp);

        return -1;
      }

      hashlist_format = hlfmt_detect (hashcat_ctx, fp, 100); // 100 = max numbers to "scan". could be hashes_avail, too

      if ((user_options->remove == true) && (hashlist_format != HLFMT_HASHCAT))
      {
        event_log_error (hashcat_ctx, "Use of --remove is not supported in native hashfile-format mode.");

        fclose (fp);

        return -1;
      }

      fclose (fp);
    }
  }
  else
  {
    hashes_avail = 1;
  }

  if (hashconfig->hash_mode == 3000) hashes_avail *= 2;

  hashes->hashlist_format = hashlist_format;

  /**
   * load hashes, part II: allocate required memory, set pointers
   */

  hash_t *hashes_buf     = (hash_t *) hccalloc (hashes_avail, sizeof (hash_t));
  void   *digests_buf    =            hccalloc (hashes_avail, hashconfig->dgst_size);
  salt_t *salts_buf      = NULL;
  void   *esalts_buf     = NULL;
  void   *hook_salts_buf = NULL;

  if ((user_options->username == true) || (hashconfig->opts_type & OPTS_TYPE_HASH_COPY) || (hashconfig->opts_type & OPTS_TYPE_HASH_SPLIT))
  {
    u32 hash_pos;

    for (hash_pos = 0; hash_pos < hashes_avail; hash_pos++)
    {
      hashinfo_t *hash_info = (hashinfo_t *) hcmalloc (sizeof (hashinfo_t));

      hashes_buf[hash_pos].hash_info = hash_info;

      if (user_options->username == true)
      {
        hash_info->user = (user_t *) hcmalloc (sizeof (user_t));
      }

      if (hashconfig->opts_type & OPTS_TYPE_HASH_COPY)
      {
        if (user_options->benchmark == false)
        {
          hash_info->orighash = (char *) hcmalloc (256);
        }
      }

      if (hashconfig->opts_type & OPTS_TYPE_HASH_SPLIT)
      {
        hash_info->split = (split_t *) hcmalloc (sizeof (split_t));
      }
    }
  }

  if (hashconfig->is_salted == true)
  {
    salts_buf = (salt_t *) hccalloc (hashes_avail, sizeof (salt_t));

    if (hashconfig->esalt_size > 0)
    {
      esalts_buf = hccalloc (hashes_avail, hashconfig->esalt_size);
    }

    if (hashconfig->hook_salt_size > 0)
    {
      hook_salts_buf = (seven_zip_hook_salt_t *) hccalloc (hashes_avail, hashconfig->hook_salt_size);
    }
  }
  else
  {
    salts_buf = (salt_t *) hccalloc (1, sizeof (salt_t));
  }

  for (u32 hash_pos = 0; hash_pos < hashes_avail; hash_pos++)
  {
    hashes_buf[hash_pos].digest = ((char *) digests_buf) + (hash_pos * hashconfig->dgst_size);

    if (hashconfig->is_salted == true)
    {
      hashes_buf[hash_pos].salt = &salts_buf[hash_pos];

      if (hashconfig->esalt_size > 0)
      {
        hashes_buf[hash_pos].esalt = ((char *) esalts_buf) + (hash_pos * hashconfig->esalt_size);
      }

      if (hashconfig->hook_salt_size > 0)
      {
        hashes_buf[hash_pos].hook_salt = ((char *) hook_salts_buf) + (hash_pos * hashconfig->hook_salt_size);
      }
    }
    else
    {
      hashes_buf[hash_pos].salt = &salts_buf[0];
    }
  }

  hashes->hashes_buf     = hashes_buf;
  hashes->digests_buf    = digests_buf;
  hashes->salts_buf      = salts_buf;
  hashes->esalts_buf     = esalts_buf;
  hashes->hook_salts_buf = hook_salts_buf;

  /**
   * load hashes, part III: parse hashes or generate them if benchmark
   */

  u32 hashes_cnt = 0;

  if (user_options->benchmark == true)
  {
    hashconfig_benchmark_defaults (hashcat_ctx, hashes_buf[0].salt, hashes_buf[0].esalt, hashes_buf[0].hook_salt);

    hashes->hashfile = "-";

    hashes_cnt = 1;
  }
  else if (user_options->example_hashes == true)
  {
  }
  else if (user_options->keyspace == true)
  {
  }
  else if (user_options->stdout_flag == true)
  {
  }
  else if (user_options->opencl_info == true)
  {
  }
  else
  {
    if (hashes_avail == 0)
    {
      // ???
    }
    else if (hashlist_mode == HL_MODE_ARG)
    {
      char *input_buf = user_options_extra->hc_hash;

      size_t input_len = strlen (input_buf);

      char  *hash_buf = NULL;
      size_t hash_len = 0;

      hlfmt_hash (hashcat_ctx, hashlist_format, input_buf, input_len, &hash_buf, &hash_len);

      bool hash_fmt_error = 0;

      if (hash_len < 1)     hash_fmt_error = 1;
      if (hash_buf == NULL) hash_fmt_error = 1;

      if (hash_fmt_error)
      {
        event_log_warning (hashcat_ctx, "Failed to parse hashes using the '%s' format.", strhlfmt (hashlist_format));
      }
      else
      {
        if (hashconfig->opts_type & OPTS_TYPE_HASH_COPY)
        {
          hashinfo_t *hash_info_tmp = hashes_buf[hashes_cnt].hash_info;

          hash_info_tmp->orighash = hcstrdup (hash_buf);
        }

        if (hashconfig->is_salted == true)
        {
          memset (hashes_buf[0].salt, 0, sizeof (salt_t));
        }

        if (hashconfig->esalt_size > 0)
        {
          memset (hashes_buf[0].esalt, 0, hashconfig->esalt_size);
        }

        if (hashconfig->hook_salt_size > 0)
        {
          memset (hashes_buf[0].hook_salt, 0, hashconfig->hook_salt_size);
        }

        int parser_status = PARSER_OK;

        if ((hashconfig->hash_mode == 2500) || (hashconfig->hash_mode == 2501))
        {
          hashes->hashlist_mode = HL_MODE_FILE;

          FILE *fp = fopen (hash_buf, "rb");

          if (fp == NULL)
          {
            event_log_error (hashcat_ctx, "%s: %s", hash_buf, strerror (errno));

            return -1;
          }

          char *in = (char *) hcmalloc (sizeof (hccapx_t));

          while (!feof (fp))
          {
            const size_t nread = hc_fread (in, sizeof (hccapx_t), 1, fp);

            if (nread == 0) break;

            if (hashes_avail == hashes_cnt)
            {
              event_log_warning (hashcat_ctx, "Hashfile '%s': File changed during runtime. Skipping new data.", hash_buf);

              break;
            }

            if (hashconfig->is_salted == true)
            {
              memset (hashes_buf[hashes_cnt].salt, 0, sizeof (salt_t));
            }

            if (hashconfig->esalt_size > 0)
            {
              memset (hashes_buf[hashes_cnt].esalt, 0, hashconfig->esalt_size);

              if ((user_options->hash_mode == 2500) || (user_options->hash_mode == 2501))
              {
                wpa_t *wpa = (wpa_t *) hashes_buf[hashes_cnt].esalt;

                if (user_options->hccapx_message_pair_chgd == true)
                {
                  wpa->message_pair_chgd = (int) user_options->hccapx_message_pair_chgd;
                  wpa->message_pair      = (u8)  user_options->hccapx_message_pair;
                }

                wpa->nonce_error_corrections = user_options->nonce_error_corrections;
              }
            }

            if (hashconfig->hook_salt_size > 0)
            {
              memset (hashes_buf[hashes_cnt].hook_salt, 0, hashconfig->hook_salt_size);
            }

            parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) in, sizeof (hccapx_t), &hashes_buf[hashes_cnt], hashconfig), hash_buf);

            if (parser_status != PARSER_OK)
            {
              event_log_warning (hashcat_ctx, "Hashfile '%s': %s", hash_buf, strparser (parser_status));

              continue;
            }

            hashes_cnt++;
          }

          hcfree (in);

          fclose (fp);
        }
        else if (hashconfig->hash_mode == 3000)
        {
          if (hash_len == 32)
          {
            parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) hash_buf, 16, &hashes_buf[hashes_cnt], hashconfig), hash_buf);

            if (parser_status == PARSER_OK)
            {
              hashes_buf[hashes_cnt].hash_info->split->split_group  = 0;
              hashes_buf[hashes_cnt].hash_info->split->split_origin = SPLIT_ORIGIN_LEFT;

              hashes_cnt++;
            }
            else
            {
              event_log_warning (hashcat_ctx, "Hash '%s': %s", input_buf, strparser (parser_status));
            }

            parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) hash_buf + 16, 16, &hashes_buf[hashes_cnt], hashconfig), hash_buf);

            if (parser_status == PARSER_OK)
            {
              hashes_buf[hashes_cnt].hash_info->split->split_group  = 0;
              hashes_buf[hashes_cnt].hash_info->split->split_origin = SPLIT_ORIGIN_RIGHT;

              hashes_cnt++;
            }
            else
            {
              event_log_warning (hashcat_ctx, "Hash '%s': %s", input_buf, strparser (parser_status));
            }
          }
          else
          {
            parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) hash_buf, (u32) hash_len, &hashes_buf[hashes_cnt], hashconfig), hash_buf);

            if (parser_status == PARSER_OK)
            {
              hashes_buf[hashes_cnt].hash_info->split->split_group  = 0;
              hashes_buf[hashes_cnt].hash_info->split->split_origin = SPLIT_ORIGIN_NONE;

              hashes_cnt++;
            }
            else
            {
              event_log_warning (hashcat_ctx, "Hash '%s': %s", input_buf, strparser (parser_status));
            }
          }
        }
        else if (hashconfig->hash_mode == 14600)
        {
          hashes->hashlist_mode = HL_MODE_FILE;

          for (int keyslot_idx = 0; keyslot_idx < LUKS_NUMKEYS; keyslot_idx++)
          {
            parser_status = luks_parse_hash ((u8 *) hash_buf, (u32) hash_len, &hashes_buf[hashes_cnt], hashconfig, keyslot_idx);

            if (parser_status != PARSER_OK)
            {
              if (parser_status != PARSER_LUKS_KEY_DISABLED)
              {
                event_log_warning (hashcat_ctx, "Hashfile '%s': %s", hash_buf, strparser (parser_status));
              }

              continue;
            }

            hashes_cnt++;
          }
        }
        else
        {
          parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) hash_buf, (u32) hash_len, &hashes_buf[hashes_cnt], hashconfig), hash_buf);

          if (parser_status == PARSER_OK)
          {
            hashes_cnt++;
          }
          else
          {
            event_log_warning (hashcat_ctx, "Hash '%s': %s", input_buf, strparser (parser_status));
          }
        }
      }
    }
    else if (hashlist_mode == HL_MODE_FILE)
    {
      FILE *fp;

      if ((fp = fopen (hashfile, "rb")) == NULL)
      {
        event_log_error (hashcat_ctx, "%s: %s", hashfile, strerror (errno));

        return -1;
      }

      u32 line_num = 0;

      char *line_buf = (char *) hcmalloc (HCBUFSIZ_LARGE);

      time_t prev = 0;
      time_t now  = 0;

      while (!feof (fp))
      {
        line_num++;

        const size_t line_len = fgetl (fp, line_buf);

        if (line_len == 0) continue;

        if (hashes_avail == hashes_cnt)
        {
          event_log_warning (hashcat_ctx, "Hashfile '%s' on line %u: File changed during runtime. Skipping new data.", hashes->hashfile, line_num);

          break;
        }

        char  *hash_buf = NULL;
        size_t hash_len = 0;

        hlfmt_hash (hashcat_ctx, hashlist_format, line_buf, line_len, &hash_buf, &hash_len);

        bool hash_fmt_error = 0;

        if (hash_len < 1)     hash_fmt_error = 1;
        if (hash_buf == NULL) hash_fmt_error = 1;

        if (hash_fmt_error)
        {
          event_log_warning (hashcat_ctx, "Failed to parse hashes using the '%s' format.", strhlfmt (hashlist_format));

          continue;
        }

        if (user_options->username == true)
        {
          char  *user_buf = NULL;
          size_t user_len = 0;

          hlfmt_user (hashcat_ctx, hashlist_format, line_buf, line_len, &user_buf, &user_len);

          // special case:
          // both hash_t need to have the username info if the pwdump format is used (i.e. we have 2 hashes for 3000, both with same user)

          u32 hashes_per_user = 1;

          if (hashconfig->hash_mode == 3000) // the following conditions should be true if (hashlist_format == HLFMT_PWDUMP)
          {
            if (hash_len == 32)
            {
              hashes_per_user = 2;
            }
          }

          for (u32 i = 0; i < hashes_per_user; i++)
          {
            user_t **user = &hashes_buf[hashes_cnt + i].hash_info->user;

            *user = (user_t *) hcmalloc (sizeof (user_t));

            user_t *user_ptr = *user;

            if (user_buf != NULL)
            {
              user_ptr->user_name = hcstrdup (user_buf);
            }
            else
            {
              user_ptr->user_name = hcstrdup ("");
            }

            user_ptr->user_len = (u32) user_len;
          }
        }

        if (hashconfig->opts_type & OPTS_TYPE_HASH_COPY)
        {
          hashinfo_t *hash_info_tmp = hashes_buf[hashes_cnt].hash_info;

          hash_info_tmp->orighash = hcstrdup (hash_buf);
        }

        if (hashconfig->is_salted == true)
        {
          memset (hashes_buf[hashes_cnt].salt, 0, sizeof (salt_t));
        }

        if (hashconfig->esalt_size > 0)
        {
          memset (hashes_buf[hashes_cnt].esalt, 0, hashconfig->esalt_size);
        }

        if (hashconfig->hook_salt_size > 0)
        {
          memset (hashes_buf[hashes_cnt].hook_salt, 0, hashconfig->hook_salt_size);
        }

        if (hashconfig->hash_mode == 3000)
        {
          if (hash_len == 32)
          {
            int parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) hash_buf, 16, &hashes_buf[hashes_cnt], hashconfig), hash_buf);

            if (parser_status < PARSER_GLOBAL_ZERO)
            {
              char *tmp_line_buf;

              hc_asprintf (&tmp_line_buf, "%s", line_buf);

              compress_terminal_line_length (tmp_line_buf, 38, 32);

              event_log_warning (hashcat_ctx, "Hashfile '%s' on line %u (%s): %s", hashes->hashfile, line_num, tmp_line_buf, strparser (parser_status));

              hcfree (tmp_line_buf);

              continue;
            }

            hashes_buf[hashes_cnt].hash_info->split->split_group  = line_num;
            hashes_buf[hashes_cnt].hash_info->split->split_origin = SPLIT_ORIGIN_LEFT;

            hashes_cnt++;

            parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) hash_buf + 16, 16, &hashes_buf[hashes_cnt], hashconfig), hash_buf);

            if (parser_status < PARSER_GLOBAL_ZERO)
            {
              char *tmp_line_buf;

              hc_asprintf (&tmp_line_buf, "%s", line_buf);

              compress_terminal_line_length (tmp_line_buf, 38, 32);

              event_log_warning (hashcat_ctx, "Hashfile '%s' on line %u (%s): %s", hashes->hashfile, line_num, tmp_line_buf, strparser (parser_status));

              hcfree (tmp_line_buf);

              continue;
            }

            hashes_buf[hashes_cnt].hash_info->split->split_group  = line_num;
            hashes_buf[hashes_cnt].hash_info->split->split_origin = SPLIT_ORIGIN_RIGHT;

            hashes_cnt++;
          }
          else
          {
            int parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) hash_buf, (u32) hash_len, &hashes_buf[hashes_cnt], hashconfig), hash_buf);

            if (parser_status < PARSER_GLOBAL_ZERO)
            {
              char *tmp_line_buf;

              hc_asprintf (&tmp_line_buf, "%s", line_buf);

              compress_terminal_line_length (tmp_line_buf, 38, 32);

              event_log_warning (hashcat_ctx, "Hashfile '%s' on line %u (%s): %s", hashes->hashfile, line_num, tmp_line_buf, strparser (parser_status));

              hcfree (tmp_line_buf);

              continue;
            }

            hashes_buf[hashes_cnt].hash_info->split->split_group  = line_num;
            hashes_buf[hashes_cnt].hash_info->split->split_origin = SPLIT_ORIGIN_NONE;

            hashes_cnt++;
          }
        }
        else
        {
          int parser_status = fitcrackHashValidator(hashconfig->parse_func ((u8 *) hash_buf, (u32) hash_len, &hashes_buf[hashes_cnt], hashconfig), hash_buf);

          if (parser_status < PARSER_GLOBAL_ZERO)
          {
            char *tmp_line_buf;

            hc_asprintf (&tmp_line_buf, "%s", line_buf);

            compress_terminal_line_length (tmp_line_buf, 38, 32);

            event_log_warning (hashcat_ctx, "Hashfile '%s' on line %u (%s): %s", hashes->hashfile, line_num, tmp_line_buf, strparser (parser_status));

            hcfree (tmp_line_buf);

            continue;
          }

          hashes_cnt++;
        }

        time (&now);

        if ((now - prev) == 0) continue;

        time (&prev);

        hashlist_parse_t hashlist_parse;

        hashlist_parse.hashes_cnt   = hashes_cnt;
        hashlist_parse.hashes_avail = hashes_avail;

        //EVENT_DATA (EVENT_HASHLIST_PARSE_HASH, &hashlist_parse, sizeof (hashlist_parse_t));
      }

      hashlist_parse_t hashlist_parse;

      hashlist_parse.hashes_cnt   = hashes_cnt;
      hashlist_parse.hashes_avail = hashes_avail;

      //EVENT_DATA (EVENT_HASHLIST_PARSE_HASH, &hashlist_parse, sizeof (hashlist_parse_t));

      hcfree (line_buf);

      fclose (fp);
    }
  }

  hashes->hashes_cnt = hashes_cnt;

  if (hashconfig->hash_mode == 3000)
  {
    // update split split_neighbor after sorting
    // see https://github.com/hashcat/hashcat/issues/1034 for good examples for testing

    for (u32 i = 0; i < hashes_cnt; i++)
    {
      split_t *split1 = hashes_buf[i].hash_info->split;

      if (split1->split_origin != SPLIT_ORIGIN_LEFT) continue;

      for (u32 j = 0; j < hashes_cnt; j++)
      {
        split_t *split2 = hashes_buf[j].hash_info->split;

        if (split2->split_origin != SPLIT_ORIGIN_RIGHT) continue;

        if (split1->split_group != split2->split_group) continue;

        split1->split_neighbor = j;
        split2->split_neighbor = i;

        break;
      }
    }
  }

  return 0;
}
