/**
 * The hash table structures.
 * Each hash table is identified by a hash_table_t structure.  hash_table_t->table is
 * pre-allocated for a user-controlled max size, and is initially empty.  As new
 * entries are created (using hash_enter()), the empty entries get filled.  If multiple
 * keys hash to the same entry, new entries are allocated and linked together in a
 * linear list.
 */

#pragma once

#include "prim_type.h"
#include "llist.h"

#include <string.h>

typedef struct hash_entry_s {
	const char* key;		/** Key string, NULL if this is an empty slot.
					    NOTE that the key must not be changed once the entry
					    has been made. */
	size_t len;			/** Key-length; the key string does not have to be a C-style NULL
					    terminated string; it can have arbitrary binary bytes */
	void* val;			/** Value associated with above key */
	struct hash_entry_s* next;	/** For collision resolution */
} hash_entry_t;

typedef struct hash_table_s {
	hash_entry_t* table;	/**Primary hash table, excluding entries that collide */
	int32 size;		/** Primary hash table size, (is a prime#); NOTE: This is the
				    number of primary entries ALLOCATED, NOT the number of valid
				    entries in the table */
	int32 inuse;		/** Number of valid entries in the table. */
	int32 nocase;		/** Whether case insensitive for key comparisons */
} hash_table_t;

/**
 * Allocate a new hash table for a given expected size.
 *
 * @note Case sensitivity of hash keys applies to 7-bit ASCII
 * characters only, and is not locale-dependent.
 *
 * @return handle to allocated hash table.
 */
hash_table_t* hash_table_new(int32 size,	/**< In: Expected number of entries in the table */
                              int32 casearg  	/**< In: Whether case insensitive for key
                                                   comparisons. When 1, case is insentitive,
                                                   0, case is sensitive. */
    );

#define HASH_CASE_YES		0
#define HASH_CASE_NO		1

/** 
 * Return upper case form for c 
 */
#define UPPER_CASE(c)	((((c) >= 'a') && ((c) <= 'z')) ? (c-32) : c)

/**
 * Return lower case form for c 
 */
#define LOWER_CASE(c)	((((c) >= 'A') && ((c) <= 'Z')) ? (c+32) : c)

/**
 * Try to add a new entry with given key and associated value to hash table h.  If key doesn't
 * already exist in hash table, the addition is successful, and the return value is val.  But
 * if key already exists, return its existing associated value.  (The hash table is unchanged;
 * it is up to the caller to resolve the conflict.)
 */
void* hash_table_enter(hash_table_t *h, /**< In: Handle of hash table in which to create entry */
                       const char *key, /**< In: C-style NULL-terminated key string
                                           for the new entry */
                       void *val	  /**< In: Value to be associated with above key */
    );

/**
 * Build a linked-list of valid hash_entry_t pointers from the given hash table.  Return the list.
 */
llist_t hash_table_tolist(hash_table_t *h,	/**< In: Hash table from which list is to be generated */
                          int32 *count		/**< Out: Number of entries in the list.
                                                   If this is NULL, no count will be returned. */

	);

/**
 * Free the specified hash table; the caller is responsible for freeing the key strings
 * pointed to by the table entries.
 */
void hash_table_free(hash_table_t *h /**< In: Handle of hash table to free */
    );