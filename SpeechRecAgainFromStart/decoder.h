#pragma once

#include "config.h"

/* Decoder object. */
struct decoder_s {
	int refcounter; /* Reference count. */
};

/* Decoder: speech recognizer object. */
typedef struct decoder_s decoder_t;

/* Initialize the decoder from a configuration object.
 
   @note The decoder retains ownership of the pointer
   <code>config</code>, so if you are not going to use it
   elsewere, you can free it.
  
   @param <code>config</code> a command-line structure, as created by
   cmd_ln_parse_r() or cmd_ln_parse_file_r(). */
decoder_t* dc_init(config_t* config);

/* Reinitialize the decoder with updated configuration.
 
   This function allows you to switch the acoustic model, dictionary,
   or other configuration without creating an entirely new decoding
   object.
  
   @note The decoder retains ownership of the pointer
   <code>config</code>, so you must not attempt to free it manually.
   If you wish to reuse it elsewhere, call cmd_ln_retain() on it.
  
   @param dc Decoder.
   @param <code>config</code> An optional new configuration to use.  If this is
                 NULL, the previous configuration will be reloaded,
                 with any changes applied.
   @return 0 for success, <0 for failure. */
int dc_reinit(decoder_t *dc, config_t* config);