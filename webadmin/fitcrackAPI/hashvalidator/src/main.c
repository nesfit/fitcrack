/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "types.h"
#include "user_options.h"
#include "memory.h"
#include "hashcat.h"






int main (int argc, char **argv)
{

  hashcat_ctx_t *hashcat_ctx = (hashcat_ctx_t *) hcmalloc (sizeof (hashcat_ctx_t));
  const int rc_hashcat_init = hashcat_init (hashcat_ctx, NULL);


  const int rc_options_init = user_options_init (hashcat_ctx);
  const int rc_options_getopt = user_options_getopt (hashcat_ctx, argc, argv);
  const int rc_options_sanity = user_options_sanity (hashcat_ctx);

  
  
  const int rc_session_init = hashcat_session_init (hashcat_ctx, NULL, NULL, 0, NULL, 0);
  int rc_final = hashcat_session_execute (hashcat_ctx);

   

  free (hashcat_ctx);
  return rc_final;
}
