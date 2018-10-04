#include "initializer.h"

#include "decoder.h"
#include "config.h"

#include <stdlib.h>

decoder_t* init_dc() {
	config_t* config;
	
	int argcc = 1;
	char *argvv[] = {"-infile", "../../../clapeyron_myrobot_histeward_16bitFrom8bitFrom16bitFromSource.wav"};

	config = config_parse_args(NULL, cont_args_def, argcc, argvv, TRUE);

	config = calloc(1,sizeof(config));

	return dc_init(config);
}