#include "config.h"

#include "llist.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct cmd_ln_val_s {
    anytype_t val;
    int type;
} cmd_ln_val_t;

config_t* config_parse_args(config_t *inout_config, /**< In/Out: Previous command-line to update,
                                                     or NULL to create a new one. */
                         arg_t const *defn,	/**< In: Array of argument name definitions */
                         int32 argc,		/**< In: Number of actual arguments */
                         char *argv[],		/**< In: Actual arguments */
                         int32 strict           /**< In: Fail on duplicate or unknown
                                                   arguments, or no arguments? */
    ) {
	config_t* config;
    hash_table_t* hashtbl = NULL;
    int32 i, j, n, argstart;

	/* Construct config object */
    if (inout_config == NULL) {
        config = (config_t*)calloc(1, sizeof(*config)); //TODO
        config->refcount = 1;
    }
    else
        config = inout_config;
	
    /* Build a hash table for argument definitions */
    hashtbl = hash_table_new(50, 0);
	if (defn) {
        for (n = 0; defn[n].name; n++) {
            void *v;

            v = hash_table_enter(hashtbl, defn[n].name, (void *)&defn[n]);
            if (strict && (v != &defn[n])) {
              //  E_ERROR("Duplicate argument name in definition: %s\n", defn[n].name); //TODO
                goto error; //TODO
            }
        }
    }
    else {
        /* No definitions. */
        n = 0;
    }
	
  error:
    if (hashtbl)
        hash_table_free(hashtbl);
    if (inout_config == NULL)
        config_free_r(config);
    //E_ERROR("Failed to parse arguments list\n"); //TODO
    return NULL;
}

void cmd_ln_val_free(cmd_ln_val_t* val) {
    int i;
    if (val->type & ARG_STRING_LIST) {
        char ** array = (char **)val->val.ptr;
        if (array) {
            for (i = 0; array[i] != NULL; i++) {
                free(array[i]); //TODO
            }
            free(array); //TODO
        }
    }
    if (val->type & ARG_STRING)
        free(val->val.ptr); //TODO
    free(val); //TODO
}

int config_free_r(config_t* config) {
    if (config == NULL)
        return 0;
    if (config->refcount > 0)
        return config->refcount;

    if (config->ht) {
        llist_t entries;
        lnode_t *ln;
        int32 n;

        entries = hash_table_tolist(config->ht, &n);
        for (ln = entries; ln; ln = ln->next) {
			hash_entry_t *e = (hash_entry_t *)ln->data.ptr;
            cmd_ln_val_free((cmd_ln_val_t *)e->val);
        }
        llist_free(entries);
        hash_table_free(config->ht);
        config->ht = NULL;
    }

    if (config->f_argv) {
        int32 i;
        for (i = 0; i < config->f_argc; ++i) {
            free(config->f_argv[i]); //TODO
        }
        free(config->f_argv); //TODO
        config->f_argv = NULL;
        config->f_argc = 0;
    }
    free(config); //TODO
    return 0;
}