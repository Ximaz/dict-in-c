/*
** XIMAZ PROJECTS, 2024
** dict_resize.c
** File description:
** Exposes a function to resize a dict and recompute all the key hashes.
*/

#include <stdlib.h>
#include "dict.h"

int dict_resize(dict_t *dict)
{
    uint64_t new_size = dict->size * DICT_RESIZE_FACTOR;
    bucket_t *new_buckets = calloc(new_size, sizeof(bucket_t));

    if (NULL == new_buckets)
        return -1;
    /* TODO: Logic to retrieve all keys and recompute them. */
    /* TODO: Move the entry. Do not destroy / construct new ones. */
    /* TODO: If everything looks good, delete all linked nodes, not entries */
    return 0;
}
