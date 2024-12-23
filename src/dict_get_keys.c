/*
** XIMAZ PROJECTS, 2024
** dict_get_keys.c
** File description:
** Exposes a function to get the list of keys of a dict.
*/

#include <assert.h>
#include <stdlib.h>
#include "dict.h"

static
void populate_keys(const dict_t *dict, dict_keys_t *keys)
{
    uint64_t index = 0;
    const bucket_t *bucket = NULL;

    for (; index < dict->size; ++index) {
        bucket = dict->buckets[index];
        while (NULL != bucket->key) {
            keys->keys[keys->size++] = bucket->key;
            bucket = bucket->next;
        }
    }
    assert(keys->size == dict->items);
}

dict_keys_t *dict_get_keys(const dict_t *dict)
{
    dict_keys_t *keys = (dict_keys_t *) calloc(1, sizeof(dict_keys_t));

    if (NULL == keys)
        return NULL;
    keys->keys = (const char **) calloc(dict->items, sizeof(char *));
    if (NULL == keys->keys) {
        free(keys);
        return NULL;
    }
    populate_keys(dict, keys);
    return keys;
}
