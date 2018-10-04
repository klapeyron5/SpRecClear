/**
 * \file glist.h
 * \brief Generic linked-lists maintenance.
 *
 * Only insert at the head of the list.  A convenient little
 * linked-list package, but a double-edged sword: the user must keep
 * track of the data type within the linked list elements.  When it
 * was first written, there was no selective deletions except to
 * destroy the entire list.  This is modified in later version. 
 * 
 * 
 * (C++ would be good for this, but that's a double-edged sword as well.)
 */

#pragma once

#include "prim_type.h"

/* A node in a generic list */
typedef struct lnode_s {
	anytype_t data;		/** See prim_type.h */
	struct lnode_s *next;	/** Next node in list */
} lnode_t;
typedef lnode_t *llist_t;	/** Head of a list of lnodes */

/**
 * Create and prepend a new list node, with the given user-defined data, at the HEAD
 * of the given generic list.  Return the new list thus formed.
 * g may be NULL to indicate an initially empty list.
 */
llist_t llist_add_ptr (llist_t g,  /**< a link list */
		       void *ptr   /**< a pointer */
	);

/**
 * Free the given generic list; user-defined data contained within is not
 * automatically freed.  The caller must have done that already.
 */
void llist_free (llist_t g);