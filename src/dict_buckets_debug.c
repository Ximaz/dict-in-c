/*
** XIMAZ PROJECTS, 2024
** dict_buckets_debug.c
** File description:
** Exposes a function used to debug the content of a linked list buckets array.
*/

#include <stdio.h>
#include "dict.h"

static
void print_bucket(const bucket_t *bucket, uint64_t index)
{
    uint64_t node_index = 0;

#ifndef __APPLE__
    fprintf(stderr, "Buckets[%lu] : %p\n", index, (void *) bucket);
#else
    fprintf(stderr, "Buckets[%llu] : %p\n", index, (void *) bucket);
#endif
    while (NULL != bucket) {
#ifndef __APPLE__
        fprintf(stderr, "- [%lu] = %s\n", node_index, bucket->key);
#else
        fprintf(stderr, "- [%llu] = %s\n", node_index, bucket->key);
#endif
        bucket = bucket->next;
        ++node_index;
    }
}

void dict_buckets_debug(bucket_t *const *buckets, uint64_t size)
{
    uint64_t index = 0;

    for (; index < size; ++index)
        print_bucket(buckets[index], index);
}
