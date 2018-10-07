#include "config.h"
#include "fe.h"

/**
 * Acoustic model structure.
 *
 * This object encapsulates all stages of acoustic processing, from
 * raw audio input to acoustic score output.  The reason for grouping
 * all of these modules together is that they all have to "agree" in
 * their parameterizations, and the configuration of the acoustic and
 * dynamic feature computation is completely dependent on the
 * parameters used to build the original acoustic model (which should
 * by now always be specified in a feat.params file).
 *
 * Because there is not a one-to-one correspondence from blocks of
 * input audio or frames of input features to frames of acoustic
 * scores (due to dynamic feature calculation), results may not be
 * immediately available after input, and the output results will not
 * correspond to the last piece of data input.
 *
 * TODO: In addition, this structure serves the purpose of queueing
 * frames of features (and potentially also scores in the future) for
 * asynchronous passes of recognition operating in parallel.
 */
struct acmod_s {
    /* Global objects, not retained. */
    config_t *config;          /**< Configuration. */

    /* Feature computation: */
    fe_t *fe;                  /**< Acoustic feature computation (waveform->cepstral). */
  //  feat_t *fcb;               /**< Dynamic feature computation (cepstral->features). */

    /* Model parameters: */
 //   bin_mdef_t *mdef;          /**< Model definition. */
 //   tmat_t *tmat;              /**< Transition matrices. */
 //   ps_mgau_t *mgau;           /**< Model parameters. */
 //   ps_mllr_t *mllr;           /**< Speaker transformation. */

    /* Senone scoring: */
    int16 *senone_scores;      /**< GMM scores for current frame. */
 //   bitvec_t *senone_active_vec; /**< Active GMMs in current frame. */
    uint8 *senone_active;      /**< Array of deltas to active GMMs. */
    int senscr_frame;          /**< Frame index for senone_scores. */
    int n_senone_active;       /**< Number of active GMMs. */
    int log_zero;              /**< Zero log-probability value. */

    /* Utterance processing: */
    mfcc_t **mfc_buf;   /**< Temporary buffer of acoustic features. */
    mfcc_t ***feat_buf; /**< Temporary buffer of dynamic features. */
 //   FILE *rawfh;        /**< File for writing raw audio data. */
 //   FILE *mfcfh;        /**< File for writing acoustic feature data. */
//    FILE *senfh;        /**< File for writing senone score data. */
 //   FILE *insenfh;	/**< Input senone score file. */
    long *framepos;     /**< File positions of recent frames in senone file. */

    /* Rawdata collected during decoding */
    int16 *rawdata;
    int32 rawdata_size;
    int32 rawdata_pos;

    /* A whole bunch of flags and counters: */
    uint8 state;        /**< State of utterance processing. */
    uint8 compallsen;   /**< Compute all senones? */
    uint8 grow_feat;    /**< Whether to grow feat_buf. */
    uint8 insen_swap;   /**< Whether to swap input senone score. */

 //   frame_idx_t utt_start_frame; /**< Index of the utterance start in the stream, all timings are relative to that. */

 //   frame_idx_t output_frame; /**< Index of next frame of dynamic features. */
 //   frame_idx_t n_mfc_alloc;  /**< Number of frames allocated in mfc_buf */
 //   frame_idx_t n_mfc_frame;  /**< Number of frames active in mfc_buf */
 //   frame_idx_t mfc_outidx;   /**< Start of active frames in mfc_buf */
 //   frame_idx_t n_feat_alloc; /**< Number of frames allocated in feat_buf */
 //   frame_idx_t n_feat_frame; /**< Number of frames active in feat_buf */
 //   frame_idx_t feat_outidx;  /**< Start of active frames in feat_buf */
};
typedef struct acmod_s acmod_t;