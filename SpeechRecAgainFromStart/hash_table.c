#include "hash_table.h"

#include <stdlib.h>
#include <string.h>
/*
 * HACK!!  Initial hash table size is restricted by this set of primes.  (Of course,
 * collision resolution by chaining will accommodate more entries indefinitely, but
 * efficiency will drop.)
 */
const int32 prime[] = {
    101, 211, 307, 401, 503, 601, 701, 809, 907,
    1009, 1201, 1601, 2003, 2411, 3001, 4001, 5003, 6007, 7001, 8009,
    9001,
    10007, 12007, 16001, 20011, 24001, 30011, 40009, 50021, 60013,
    70001, 80021, 90001,
    100003, 120011, 160001, 200003, 240007, 300007, 400009, 500009,
    600011, 700001, 800011, 900001,
    -1
};


/**
 * This function returns a very large prime. 
 */
static int32 prime_size(int32 size) {
    int32 i;

    for (i = 0; (prime[i] > 0) && (prime[i] < size); i++);
    if (prime[i] <= 0) {
        //E_WARN("Very large hash table requested (%d entries)\n", size); //TODO
        --i;
    }
    return (prime[i]);
}

hash_table_t* hash_table_new(int32 size, int32 casearg) {
    hash_table_t *h;

    h = (hash_table_t *) calloc(1, sizeof(hash_table_t)); //TODO
    h->size = prime_size(size + (size >> 1));
    h->nocase = (casearg == HASH_CASE_NO);
    h->table = (hash_entry_t *) calloc(h->size, sizeof(hash_entry_t)); //TODO
    /* The above calloc clears h->table[*].key and .next to NULL, i.e. an empty table */

    return h;
}

/*
 * Compute hash value for given key string.
 * Somewhat tuned for English text word strings.
 */
static uint32 key2hash(hash_table_t* h, const char *key) {

    register const char* cp;

    /** ARCHAN 20050712: 
	[1236322] libutil\str2words special character bgu
	HACK Apply suggested hack of fixing the hash table such that
	it can work with extended ascii code . This is a hack because
	the best way to solve it is to make sure all character
	representation is unsigned character in the first place. (or
	better unicode.)
    **/

    /*register char c; */
    register unsigned char c;
    register int32 s;
    register uint32 hash;

    hash = 0;
    s = 0;

    if (h->nocase) {
        for (cp = key; *cp; cp++) {
            c = *cp;
            c = UPPER_CASE(c);
            hash += c << s;
            s += 5;
            if (s >= 25)
                s -= 24;
        }
    }
    else {
        for (cp = key; *cp; cp++) {
            hash += (*cp) << s;
            s += 5;
            if (s >= 25)
                s -= 24;
        }
    }

    return (hash % h->size);
}

static int32 keycmp_case(hash_entry_t * entry, const char *key) {
    char c1, c2;
    int32 i;
    const char *str;

    str = entry->key;
    for (i = 0; i < entry->len; i++) {
        c1 = *(str++);
        c2 = *(key++);
        if (c1 != c2)
            return (c1 - c2);
    }

    return 0;
}

static int32 keycmp_nocase(hash_entry_t * entry, const char *key) {
    char c1, c2;
    int32 i;
    const char *str;

    str = entry->key;
    for (i = 0; i < entry->len; i++) {
        c1 = *(str++);
        c1 = UPPER_CASE(c1);
        c2 = *(key++);
        c2 = UPPER_CASE(c2);
        if (c1 != c2)
            return (c1 - c2);
    }

    return 0;
}

/*
 * Lookup entry with hash-value hash in table h for given key
 * Return value: hash_entry_t for key
 */
static hash_entry_t* lookup(hash_table_t* h, uint32 hash, const char* key, size_t len) {
    hash_entry_t *entry;

    entry = &(h->table[hash]);
    if (entry->key == NULL)
        return NULL;

    if (h->nocase) {
        while (entry && ((entry->len != len)
                         || (keycmp_nocase(entry, key) != 0)))
            entry = entry->next;
    }
    else {
        while (entry && ((entry->len != len)
                         || (keycmp_case(entry, key) != 0)))
            entry = entry->next;
    }

    return entry;
}

static void* enter(hash_table_t * h, uint32 hash, const char *key, size_t len, void *val, int32 replace) {
    hash_entry_t *cur, *neww;

    if ((cur = lookup(h, hash, key, len)) != NULL) {
        void* oldval;
        /* Key already exists. */
        oldval = cur->val;
        if (replace) {
            /* Replace the pointer if replacement is requested,
             * because this might be a different instance of the same
             * string (this verges on magic, sorry) */
            cur->key = key;
            cur->val = val;
        }
        return oldval;
    }

    cur = &(h->table[hash]);
    if (cur->key == NULL) {
        /* Empty slot at hashed location; add this entry */
        cur->key = key;
        cur->len = len;
        cur->val = val;

        /* Added by ARCHAN at 20050515. This allows deletion could work. */
        cur->next = NULL;

    }
    else {
        /* Key collision; create new entry and link to hashed location */
        neww = (hash_entry_t *) calloc(1, sizeof(hash_entry_t)); //TODO
        neww->key = key;
        neww->len = len;
        neww->val = val;
        neww->next = cur->next;
        cur->next = neww;
    }
    ++h->inuse;

    return val;
}
	
void* hash_table_enter(hash_table_t * h, const char *key, void *val) {
    uint32 hash;
    size_t len;

    hash = key2hash(h, key);
    len = strlen(key);
    return (enter(h, hash, key, len, val, 0));
}

llist_t hash_table_tolist(hash_table_t * h, int32 * count) {
    llist_t g;
    hash_entry_t *e;
    int32 i, j;

    g = NULL;

    j = 0;
    for (i = 0; i < h->size; i++) {
        e = &(h->table[i]);

        if (e->key != NULL) {
            g = llist_add_ptr(g, (void *) e);
            j++;

            for (e = e->next; e; e = e->next) {
                g = llist_add_ptr(g, (void *) e);
                j++;
            }
        }
    }

    if (count)
        *count = j;

    return g;
}

void hash_table_free(hash_table_t * h) {
    hash_entry_t *e, *e2;
    int32 i;

    if (h == NULL)
        return;

    /* Free additional entries created for key collision cases */
    for (i = 0; i < h->size; i++) {
        for (e = h->table[i].next; e; e = e2) {
            e2 = e->next;
            free((void *) e); //TODO
        }
    }

    free((void *) h->table); //TODO
    free((void *) h); //TODO
}

int32 hash_table_lookup(hash_table_t * h, const char *key, void ** val) {
    hash_entry_t *entry;
    uint32 hash;
    size_t len;

    hash = key2hash(h, key);
    len = strlen(key);

    entry = lookup(h, hash, key, len);
    if (entry) {
        if (val)
            *val = entry->val;
        return 0;
    }
    else
        return -1;
}

void* hash_table_replace(hash_table_t * h, const char *key, void *val) {
    uint32 hash;
    size_t len;

    hash = key2hash(h, key);
    len = strlen(key);
    return (enter(h, hash, key, len, val, 1));
}