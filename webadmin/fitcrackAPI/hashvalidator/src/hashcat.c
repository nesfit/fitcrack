/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#include "common.h"

// basic tools

#include "types.h"
#include "memory.h"



// features

#include "hashcat.h"
#include "hashes.h"
#include "interface.h"
#include "user_options.h"




static int outer_loop (hashcat_ctx_t *hashcat_ctx)
{

  const int rc_hashconfig = hashconfig_init (hashcat_ctx);

  if (rc_hashconfig == -1)
  {
    return -1;
  }


  hashes_init_filename (hashcat_ctx);
  const int rc_hashes_init_stage1 = hashes_init_stage1 (hashcat_ctx);
  return rc_hashes_init_stage1;
}



int hashcat_init (hashcat_ctx_t *hashcat_ctx, void (*event) (const u32, struct hashcat_ctx *, const void *, const size_t))
{


  //hashcat_ctx->bitmap_ctx         = (bitmap_ctx_t *)          hcmalloc (sizeof (bitmap_ctx_t));
  //hashcat_ctx->combinator_ctx     = (combinator_ctx_t *)      hcmalloc (sizeof (combinator_ctx_t));
  //hashcat_ctx->cpt_ctx            = (cpt_ctx_t *)             hcmalloc (sizeof (cpt_ctx_t));
  //hashcat_ctx->debugfile_ctx      = (debugfile_ctx_t *)       hcmalloc (sizeof (debugfile_ctx_t));
  //hashcat_ctx->dictstat_ctx       = (dictstat_ctx_t *)        hcmalloc (sizeof (dictstat_ctx_t));
  //hashcat_ctx->event_ctx          = (event_ctx_t *)           hcmalloc (sizeof (event_ctx_t));
  hashcat_ctx->folder_config      = (folder_config_t *)       hcmalloc (sizeof (folder_config_t));
  //hashcat_ctx->hashcat_user       = (hashcat_user_t *)        hcmalloc (sizeof (hashcat_user_t));
  hashcat_ctx->hashconfig         = (hashconfig_t *)          hcmalloc (sizeof (hashconfig_t));
  hashcat_ctx->hashes             = (hashes_t *)              hcmalloc (sizeof (hashes_t));
  //hashcat_ctx->hwmon_ctx          = (hwmon_ctx_t *)           hcmalloc (sizeof (hwmon_ctx_t));
  //hashcat_ctx->induct_ctx         = (induct_ctx_t *)          hcmalloc (sizeof (induct_ctx_t));
  //hashcat_ctx->logfile_ctx        = (logfile_ctx_t *)         hcmalloc (sizeof (logfile_ctx_t));
  //hashcat_ctx->loopback_ctx       = (loopback_ctx_t *)        hcmalloc (sizeof (loopback_ctx_t));
  //hashcat_ctx->mask_ctx           = (mask_ctx_t *)            hcmalloc (sizeof (mask_ctx_t));
  //hashcat_ctx->opencl_ctx         = (opencl_ctx_t *)          hcmalloc (sizeof (opencl_ctx_t));
  //hashcat_ctx->outcheck_ctx       = (outcheck_ctx_t *)        hcmalloc (sizeof (outcheck_ctx_t));
  //hashcat_ctx->outfile_ctx        = (outfile_ctx_t *)         hcmalloc (sizeof (outfile_ctx_t));
  //hashcat_ctx->pidfile_ctx        = (pidfile_ctx_t *)         hcmalloc (sizeof (pidfile_ctx_t));
  //hashcat_ctx->potfile_ctx        = (potfile_ctx_t *)         hcmalloc (sizeof (potfile_ctx_t));
  //hashcat_ctx->restore_ctx        = (restore_ctx_t *)         hcmalloc (sizeof (restore_ctx_t));
  //hashcat_ctx->status_ctx         = (status_ctx_t *)          hcmalloc (sizeof (status_ctx_t));
  //hashcat_ctx->straight_ctx       = (straight_ctx_t *)        hcmalloc (sizeof (straight_ctx_t));
  //hashcat_ctx->tuning_db          = (tuning_db_t *)           hcmalloc (sizeof (tuning_db_t));
  hashcat_ctx->user_options_extra = (user_options_extra_t *)  hcmalloc (sizeof (user_options_extra_t));
  hashcat_ctx->user_options       = (user_options_t *)        hcmalloc (sizeof (user_options_t));


  return 0;
}

int hashcat_session_init (hashcat_ctx_t *hashcat_ctx, const char *install_folder, const char *shared_folder, int argc, char **argv, const int comptime)
{
  user_options_extra_init (hashcat_ctx);
  return 0;
}

int hashcat_session_execute (hashcat_ctx_t *hashcat_ctx)
{
    int rc_final = outer_loop (hashcat_ctx);
    return rc_final;
}

int hashcat_get_status (hashcat_ctx_t *hashcat_ctx, hashcat_status_t *hashcat_status)
{

  return 0;
}
