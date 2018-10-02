#include "config_line.h"

#include "prim_type.h"

struct config_line_s {
    int refcount;
  //  hash_table_t *ht;
    char **f_argv;
    uint32 f_argc;
};