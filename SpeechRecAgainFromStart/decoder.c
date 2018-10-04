#include "decoder.h"

#include <stdlib.h>

decoder_t* dc_init(config_t* config) {
	decoder_t* dc;
	if (!config) {
		//E_ERROR("No configuration specified"); //TODO
		return NULL;
    }

	dc = calloc(1, sizeof(*dc));
	dc->refcounter = 1;


	return dc;
}

int dc_reinit(decoder_t *dc, config_t* config) {

	return 0;
}