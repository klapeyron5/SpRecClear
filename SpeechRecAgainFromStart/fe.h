/*
 * fe.h
 * 
 * Computing MFCC from wave
 *
 */

#include "prim_type.h"
#include "init_args.h"

#ifdef WORDS_BIGENDIAN
#define NATIVE_ENDIAN "big"
#else
#define NATIVE_ENDIAN "little"
#endif

/** Default number of samples per second. */
#define DEFAULT_SAMPLING_RATE 16000
/** Default number of frames per second. */
#define DEFAULT_FRAME_RATE 100
/** Default spacing between frame starts (equal to
 * DEFAULT_SAMPLING_RATE/DEFAULT_FRAME_RATE) */
#define DEFAULT_FRAME_SHIFT 160
/** Default size of each frame (410 samples @ 16000Hz). */
#define DEFAULT_WINDOW_LENGTH 0.025625 
/** Default number of FFT points. */
#define DEFAULT_FFT_SIZE 512
/** Default number of MFCC coefficients in output. */
#define DEFAULT_NUM_CEPSTRA 13
/** Default number of filter bands used to generate MFCCs. */
#define DEFAULT_NUM_FILTERS 40

/** Default prespeech length */
#define DEFAULT_PRE_SPEECH 20
/** Default postspeech length */
#define DEFAULT_POST_SPEECH 50
/** Default postspeech length */
#define DEFAULT_START_SPEECH 10

/** Default lower edge of mel filter bank. */
#define DEFAULT_LOWER_FILT_FREQ 133.33334
/** Default upper edge of mel filter bank. */
#define DEFAULT_UPPER_FILT_FREQ 6855.4976
/** Default pre-emphasis filter coefficient. */
#define DEFAULT_PRE_EMPHASIS_ALPHA 0.97
/** Default type of frequency warping to use for VTLN. */
#define DEFAULT_WARP_TYPE "inverse_linear"
/** Default random number seed to use for dithering. */
#define SEED -1

#define waveform_to_cepstral_command_line_macro() \
  { "-logspec", \
    ARG_BOOLEAN, \
    "no", \
    "Write out logspectral files instead of cepstra" }, \
   \
  { "-smoothspec", \
    ARG_BOOLEAN, \
    "no", \
    "Write out cepstral-smoothed logspectral files" }, \
   \
  { "-transform", \
    ARG_STRING, \
    "legacy", \
    "Which type of transform to use to calculate cepstra (legacy, dct, or htk)" }, \
   \
  { "-alpha", \
    ARG_FLOAT, \
    ARG_STRINGIFY(DEFAULT_PRE_EMPHASIS_ALPHA), \
    "Preemphasis parameter" }, \
   \
  { "-samprate", \
    ARG_FLOAT, \
    ARG_STRINGIFY(DEFAULT_SAMPLING_RATE), \
    "Sampling rate" }, \
   \
  { "-frate", \
    ARG_INTEGER, \
    ARG_STRINGIFY(DEFAULT_FRAME_RATE), \
    "Frame rate" }, \
   \
  { "-wlen", \
    ARG_FLOAT, \
    ARG_STRINGIFY(DEFAULT_WINDOW_LENGTH), \
    "Hamming window length" }, \
   \
  { "-nfft", \
    ARG_INTEGER, \
    ARG_STRINGIFY(DEFAULT_FFT_SIZE), \
    "Size of FFT" }, \
   \
  { "-nfilt", \
    ARG_INTEGER, \
    ARG_STRINGIFY(DEFAULT_NUM_FILTERS), \
    "Number of filter banks" }, \
   \
  { "-lowerf", \
    ARG_FLOAT, \
    ARG_STRINGIFY(DEFAULT_LOWER_FILT_FREQ), \
    "Lower edge of filters" }, \
   \
  { "-upperf", \
    ARG_FLOAT, \
    ARG_STRINGIFY(DEFAULT_UPPER_FILT_FREQ), \
    "Upper edge of filters" }, \
   \
  { "-unit_area", \
    ARG_BOOLEAN, \
    "yes", \
    "Normalize mel filters to unit area" }, \
   \
  { "-round_filters", \
    ARG_BOOLEAN, \
    "yes", \
    "Round mel filter frequencies to DFT points" }, \
   \
  { "-ncep", \
    ARG_INTEGER, \
    ARG_STRINGIFY(DEFAULT_NUM_CEPSTRA), \
    "Number of cep coefficients" }, \
   \
  { "-doublebw", \
    ARG_BOOLEAN, \
    "no", \
    "Use double bandwidth filters (same center freq)" }, \
   \
  { "-lifter", \
    ARG_INTEGER, \
    "0", \
    "Length of sin-curve for liftering, or 0 for no liftering." }, \
   \
  { "-vad_prespeech", \
    ARG_INTEGER, \
    ARG_STRINGIFY(DEFAULT_PRE_SPEECH), \
    "Num of speech frames to keep before silence to speech." }, \
   \
  { "-vad_startspeech", \
    ARG_INTEGER, \
    ARG_STRINGIFY(DEFAULT_START_SPEECH), \
    "Num of speech frames to trigger VAD from silence to speech." }, \
   \
  { "-vad_postspeech", \
    ARG_INTEGER, \
    ARG_STRINGIFY(DEFAULT_POST_SPEECH), \
    "Num of silence frames to keep after from speech to silence." }, \
   \
  { "-vad_threshold", \
    ARG_FLOAT, \
    "2.0", \
    "Threshold for decision between noise and silence frames. Log-ratio between signal level and noise level." }, \
   \
  { "-input_endian", \
    ARG_STRING, \
    NATIVE_ENDIAN, \
    "Endianness of input data, big or little, ignored if NIST or MS Wav" }, \
   \
  { "-warp_type", \
    ARG_STRING, \
    DEFAULT_WARP_TYPE, \
    "Warping function type (or shape)" }, \
   \
  { "-warp_params", \
    ARG_STRING, \
    NULL, \
    "Parameters defining the warping function" }, \
   \
  { "-dither", \
    ARG_BOOLEAN, \
    "no", \
    "Add 1/2-bit noise" }, \
   \
  { "-seed", \
    ARG_INTEGER, \
    ARG_STRINGIFY(SEED), \
    "Seed for random number generator; if less than zero, pick our own" }, \
   \
  { "-remove_dc", \
    ARG_BOOLEAN, \
    "no", \
    "Remove DC offset from each frame" }, \
                                          \
  { "-remove_noise", \
    ARG_BOOLEAN, \
    "yes", \
    "Remove noise with spectral subtraction in mel-energies" }, \
                                                                \
  { "-remove_silence", \
    ARG_BOOLEAN, \
    "yes", \
    "Enables VAD, removes silence frames from processing" }, \
                                                             \
  { "-verbose", \
    ARG_BOOLEAN, \
    "no", \
    "Show input filenames" } \
	
/* Values for the 'logspec' field. */
enum {
	RAW_LOG_SPEC = 1,
	SMOOTH_LOG_SPEC = 2
};