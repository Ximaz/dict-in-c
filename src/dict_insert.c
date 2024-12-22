/*
** XIMAZ PROJECTS, 2024
** dict_insert.c
** File description:
** Exposes a function to insert an entry into a dict.
*/

#include <stdlib.h>
#include "dict.h"
#include "murmurhash1.h"

/**
 * TODO: The way buckets are implemented right now is not good.
 * This won't work yet.
 */
static int insert_bucket_tail(bucket_t **bucket, entry_t *entry)
{
    bucket_t *next = (bucket_t *) calloc(1, sizeof(bucket_t));

    if (NULL == next)
        return -1;
    if (NULL == (*bucket)->entry) {
        (*bucket)->entry = entry;
        (*bucket)->next = next;
        return 0;
    }
    while (NULL != (*bucket)->next)
        (*bucket) = (*bucket)->next;
    (*bucket)->entry = entry;
    (*bucket)->next = next;
    return 0;
}

int dict_insert(dict_t *dict, const char *key, uint64_t key_length,
    void *value)
{
    uint32_t key_hash = 0;
    bucket_t *bucket = NULL;
    entry_t *entry = NULL;

    if (DICT_MUST_GROW(dict) && -1 == dict_resize(dict))
        return -1;
    entry = entry_ctor(key, value);
    if (NULL == entry)
        return -1;
    key_hash = hash(key, key_length, HASH_SEED);
    bucket = DICT_GRAB_BUCKET(dict, key_hash);
    return insert_bucket_tail(&bucket, entry);
}
