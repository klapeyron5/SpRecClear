#include "llist.h"

#include <stdlib.h>

llist_t llist_add_ptr(llist_t g, void *ptr) {
    lnode_t *ln;

    ln = (lnode_t *) calloc(1, sizeof(lnode_t)); //TODO
    ln->data.ptr = ptr;
    ln->next = g;
    return ((llist_t) ln);      /* Return the new head of the list */
}

void llist_free(llist_t g) {
    lnode_t *ln;

    while (g) {
        ln = g;
        g = ln->next;
        free((void *) ln); //TODO
    }
}