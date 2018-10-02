#include "config_line.h"

/**
 * PocketSphinx speech recognizer object.
 */
typedef struct decoder_s decoder_t;

/**
 * Initialize the decoder from a configuration object.
 *
 * @note The decoder retains ownership of the pointer
 * <code>config</code>, so if you are not going to use it
 * elsewere, you can free it.
 *
 * @param config a command-line structure, as created by
 * cmd_ln_parse_r() or cmd_ln_parse_file_r().
 */
decoder_t* dc_init(config_line_t* config);