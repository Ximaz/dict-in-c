/*
** XIMAZ PROJECTS, 2024
** dict_insert.c
** File description:
** Exposes a function to insert an entry into a dict.
*/

#include <stdlib.h>
#include "dict.h"
#include "murmurhash1.h"

int dict_insert(dict_t *dict, char *key, uint64_t key_length, void *value)
{
    uint32_t key_hash = 0;
    bucket_t **bucket_addr = NULL;

    if (DICT_MUST_GROW(dict) && -1 == dict_resize(dict))
        return -1;
    key_hash = murmurhash1(key, key_length, HASH_SEED);
    bucket_addr = &(dict->buckets[DICT_BUCKET_IDX(key_hash, dict->size)]);
    if (-1 == dict_bucket_insert(bucket_addr, key, value))
        return -1;
    ++dict->items;
    return 0;
}
