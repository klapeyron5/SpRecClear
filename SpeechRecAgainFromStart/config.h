#pragma once

#include "prim_type.h"
#include "init_args.h"
#include "hash_table.h"

struct config_s {
    int refcount;
    hash_table_t *ht;
    char **f_argv;
    uint32 f_argc;
};

/* Configuration type to hold parameters for decoder */
typedef struct config_s config_t;

/**
 * Parse a list of strings into argumetns.
 *
 * Parse the given list of arguments (name-value pairs) according to
 * the given definitions.  Argument values can be retrieved in future
 * using cmd_ln_access().  argv[0] is assumed to be the program name
 * and skipped.  Any unknown argument name causes a fatal error.  The
 * routine also prints the prevailing argument values (to stderr)
 * after parsing.
 *
 * @note It is currently assumed that the strings in argv are
 *       allocated statically, or at least that they will be valid as
 *       long as the cmd_ln_t returned from this function.
 *       Unpredictable behaviour will result if they are freed or
 *       otherwise become invalidated.
 *
 * @return A cmd_ln_t containing the results of command line parsing,
 *         or NULL on failure.
 **/
config_t* config_parse_args(config_t *inout_config, /**< In/Out: Previous command-line to update,
                                                     or NULL to create a new one. */
                         arg_t const *defn,	/**< In: Array of argument name definitions */
                         int32 argc,		/**< In: Number of actual arguments */
                         char *argv[],		/**< In: Actual arguments */
                         int32 strict           /**< In: Fail on duplicate or unknown
                                                   arguments, or no arguments? */
    );

/**
 * Release a command-line argument set and all associated strings.
 *
 * @return new reference count (0 if freed completely)
 */
int config_free_r(config_t* config);