/* This file generate configuration string and return through the
 * method init_dc() initialized decoder with this configuration */

#include "decoder.h"
#include "init_args.h"
#include "fe.h"

#include <stdio.h>

/* Initialize the decoder with the configuration string */
decoder_t* init_dc(void);

/* Minimal set of config-line options for the decoder */
#define POCKETSPHINX_OPTIONS \
    waveform_to_cepstral_command_line_macro(), \
    POCKETSPHINX_DEBUG_OPTIONS

/* Options for debugging and logging. */
#define POCKETSPHINX_DEBUG_OPTIONS                      \
    { "-logfn",                                         \
            ARG_STRING,                                 \
            NULL,                                       \
            "File to write log messages in" },          \
    { "-debug",                                         \
            ARG_INTEGER,                                \
            NULL,                                       \
            "Verbosity level for debugging messages" }, \
     { "-mfclogdir",                                    \
             ARG_STRING,                                \
             NULL,                                      \
             "Directory to log feature files to" },     \
    { "-rawlogdir",                                     \
            ARG_STRING,                                 \
            NULL,                                       \
            "Directory to log raw audio files to" },    \
    { "-senlogdir",                                     \
            ARG_STRING,                                 \
            NULL,                                       \
            "Directory to log senone score files to" }

#define CMDLN_EMPTY_OPTION { NULL, 0, NULL, NULL }

static const arg_t cont_args_def[] = {
    POCKETSPHINX_OPTIONS,
    {"-argfile",
     ARG_STRING,
     NULL,
     "Argument file giving extra arguments."},
    {"-adcdev",
     ARG_STRING,
     NULL,
     "Name of audio device to use for input."},
    {"-infile",
     ARG_STRING,
     NULL,
     "Audio file to transcribe."},
    {"-inmic",
     ARG_BOOLEAN,
     "no",
     "Transcribe audio from microphone."},
    {"-time",
     ARG_BOOLEAN,
     "no",
     "Print word times in file transcription."},
    CMDLN_EMPTY_OPTION
};