/*
** XIMAZ PROJECTS, 2024
** dict_buckets_ctor.c
** File description:
** Exposes a function used to allocate a new buckets linked list array.
*/

#include <stdlib.h>
#include "dict.h"

int dict_buckets_ctor(bucket_t **buckets, uint64_t size)
{
    uint64_t index = 0;

    for (; index < size; ++index) {
        buckets[index] = (bucket_t *) calloc(1, sizeof(bucket_t));
        if (NULL == buckets[index]) {
            dict_buckets_dtor(buckets, index, NULL);
            return -1;
        }
    }
    return 0;
}
