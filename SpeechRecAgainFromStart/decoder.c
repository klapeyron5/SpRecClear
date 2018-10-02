#include "decoder.h"
#include "decoder_internal.h"

#include <stdlib.h>

decoder_t* dc_init(config_line_t* config) {
	decoder_t* dc;
	dc = calloc(1, sizeof(*dc));

	return dc;
}