/*
** XIMAZ PROJECTS, 2024
** dict_bucket_has_key.c
** File description:
** Exposes a function ussed to tell whether a key is present in a bucket.
*/

#include <string.h>
#include "dict.h"

int dict_bucket_has_key(const bucket_t *bucket, const char *key)
{
    while (NULL != bucket->key) {
        if (bucket->key == key || 0 == strcmp(bucket->key, key))
            return 1;
        bucket = bucket->next;
    }
    return 0;
}
