#include "config.h"
#include "llist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable : 4996)

typedef struct cmd_ln_val_s {
    anytype_t val;
    int type;
} cmd_ln_val_t;

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

static size_t strnappend(char **dest, size_t *dest_allocation,
       const char *source, size_t n) {
    size_t source_len, required_allocation;

    if (dest == NULL || dest_allocation == NULL)
        return -1;
    if (*dest == NULL && *dest_allocation != 0)
        return -1;
    if (source == NULL)
        return *dest_allocation;

    source_len = strlen(source);
    if (n && n < source_len)
        source_len = n;

    required_allocation = (*dest ? strlen(*dest) : 0) + source_len + 1;
    if (*dest_allocation < required_allocation) {
        if (*dest_allocation == 0) {
            *dest = (char *)calloc(required_allocation * 2, 1); //TODO
        } else {
            *dest = (char *)realloc(*dest, required_allocation * 2); //TODO
        }
        *dest_allocation = required_allocation * 2;
    }

    strncat(*dest, source, source_len);

    return *dest_allocation;
}

static size_t strappend(char **dest, size_t *dest_allocation,
       const char *source) {
    return strnappend(dest, dest_allocation, source, 0);
}

static char* arg_resolve_env(const char *str) {
    char *resolved_str = NULL;
    char env_name[100];
    const char *env_val;
    size_t alloced = 0;
    const char *i = str, *j;

    /* calculate required resolved_str size */
    do {
        j = strstr(i, "$(");
        if (j != NULL) {
            if (j != i) {
                strnappend(&resolved_str, &alloced, i, j - i);
                i = j;
            }
            j = strchr(i + 2, ')');
            if (j != NULL) {
                if (j - (i + 2) < 100) {
                    strncpy(env_name, i + 2, j - (i + 2));
                    env_name[j - (i + 2)] = '\0';
                    #if !defined(_WIN32_WCE)
                    env_val = getenv(env_name);
                    if (env_val)
                        strappend(&resolved_str, &alloced, env_val);
                    #else
                    env_val = 0;
                    #endif
                }
                i = j + 1;
            } else {
                /* unclosed, copy and skip */
                j = i + 2;
                strnappend(&resolved_str, &alloced, i, j - i);
                i = j;
            }
        } else {
            strappend(&resolved_str, &alloced, i);
        }
    } while(j != NULL);

    return resolved_str;
}

static cmd_ln_val_t* cmd_ln_val_init(int t, const char *str) {
    cmd_ln_val_t *v;
    anytype_t val;
    char *e_str;

    if (!str) {
        /* For lack of a better default value. */
        memset(&val, 0, sizeof(val));
    }
    else {
        int valid = 1;
        e_str = arg_resolve_env(str);

        switch (t) {
        case ARG_INTEGER:
        case ARG_FLOAT:
        case ARG_BOOLEAN:
        case ARG_STRING:
        default:
        //    E_ERROR("Unknown argument type: %d\n", t); //TODO
            valid = 0;
        }

        free(e_str); //TODO
        if (valid == 0)
            return NULL;
    }

    v = (cmd_ln_val_t *)calloc(1, sizeof(*v)); //TODO
    memcpy(v, &val, sizeof(val));
    v->type = t;

    return v;
}

config_t* config_parse_args(config_t *inout_config, /**< In/Out: Previous command-line to update,
                                                     or NULL to create a new one. */
                         arg_t const *default_names,	/**< In: Array of argument name definitions */
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
	if (default_names) {
        for (n = 0; default_names[n].name; n++) {
            void *v;

            v = hash_table_enter(hashtbl, default_names[n].name, (void *)&default_names[n]);
			printf("%s%s",default_names[n].deflt,"\n");																		////////////////STOPPED HERE///////////////////
            if (strict && (v != &default_names[n])) {
              //  E_ERROR("Duplicate argument name in definition: %s\n", defn[n].name); //TODO
                goto error; //TODO
            }
        }
    }
    else {
        /* No definitions. */
        n = 0;
    }
	/* Allocate memory for argument values */
    if (config->ht == NULL)
        config->ht = hash_table_new(n, 0 /* argument names are case-sensitive */ );

	/* skip argv[0] if it doesn't start with dash */
    argstart = 0;
    if (argc > 0 && argv[0][0] != '-') {
        argstart = 1;
    }
	
    /* Parse command line arguments (name-value pairs) */
    for (j = argstart; j < argc; j += 2) {
        arg_t *argdef;
        cmd_ln_val_t *val;
        void *v;

        if (hash_table_lookup(hashtbl, argv[j], &v) < 0) {
            if (strict) {
             //   E_ERROR("Unknown argument name '%s'\n", argv[j]); //TODO
                goto error;
            }
            else if (default_names == NULL)
                v = NULL;
            else
                continue;
        }
        argdef = (arg_t *)v;

        /* Enter argument value */
        if (j + 1 >= argc) {
        //    E_ERROR("Argument value for '%s' missing\n", argv[j]); //TODO
            goto error;
        }

        if (argdef == NULL)
            val = cmd_ln_val_init(ARG_STRING, argv[j + 1]);
        else {
            if ((val = cmd_ln_val_init(argdef->type, argv[j + 1])) == NULL) {
             //   E_ERROR("Bad argument value for %s: %s\n", argv[j],
                  //      argv[j + 1]); //TODO
                goto error;
            }
        }

        if ((v = hash_table_enter(config->ht, argv[j], (void *)val)) !=
            (void *)val)
        {
            if (strict) {
                cmd_ln_val_free(val);
              //  E_ERROR("Duplicate argument name in arguments: %s\n",
              //          argdef->name); //TODO
                goto error;
            }
            else {
                v = hash_table_replace(config->ht, argv[j], (void *)val);
                cmd_ln_val_free((cmd_ln_val_t *)v);
            }
        }
    }

	/* If we use it from something except pocketsphinx, print current values */
//    if (!cmd_ln_exists_r(config, "-logfn") && err_get_logfp()) {
//	cmd_ln_print_values_r(config, err_get_logfp(), defn);
//    }

    hash_table_free(hashtbl);
    return config;

  error:
    if (hashtbl)
        hash_table_free(hashtbl);
    if (inout_config == NULL)
        config_free_r(config);
    //E_ERROR("Failed to parse arguments list\n"); //TODO
    return NULL;
}
