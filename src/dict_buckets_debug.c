/*
** XIMAZ PROJECTS, 2024
** dict_buckets_debug.c
** File description:
** Exposes a function used to debug the content of a linked list buckets array.
*/

#include <stdio.h>
#include "dict.h"

void dict_buckets_debug(bucket_t *const *buckets, uint64_t size)
{
    const bucket_t *bucket = NULL;
    uint64_t index = 0;
    uint64_t node_index = 0;

    for (; index < size; ++index) {
        bucket = buckets[index];
        node_index = 0;
        dprintf(2, "Buckets[%llu] : %p\n", index, (void *) bucket);
        while (NULL != bucket && NULL != bucket->entry) {
            dprintf(2, "- [%llu] = %s\n", node_index, bucket->entry->key);
            bucket = bucket->next;
        }
    }
}
