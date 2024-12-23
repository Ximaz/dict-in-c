/*
** XIMAZ PROJECTS, 2024
** dict_delete.c
** File description:
** Exposes a function used to delete a pair from a dict using the key.
*/

#include <string.h>
#include "dict.h"
#include "murmurhash1.h"

int dict_delete(dict_t *dict, char *key, uint64_t key_length,
    free_pair_t free_pair)
{
    uint32_t key_hash = 0;
    bucket_t **bucket_addr = NULL;

    if (DICT_MUST_SHRINK(dict) && -1 == dict_resize(dict))
        return -1;
    key_hash = murmurhash1(key, key_length, HASH_SEED);
    bucket_addr = &(dict->buckets[DICT_BUCKET_IDX(key_hash, dict->size)]);
    if (-1 == dict_bucket_delete(bucket_addr, key, free_pair))
        return -1;
    --dict->items;
    return 0;
}
