#include "decoder.h"
#include "config_line.h"

#include <stdlib.h>

decoder_t* init_dc() {
	config_line_t* config;
	config = calloc(1,sizeof(config));

	return dc_init(config);
}