/*
** XIMAZ PROJECTS, 2024
** dict_buckets_dtor.c
** File description:
** Exposes a function used to deallocate a buckets linked list array.
*/

#include <stdlib.h>
#include "dict.h"

void dict_buckets_dtor(bucket_t **buckets, uint64_t size)
{
    uint64_t index = 0;

    for (; index < size; ++index)
        free(buckets[index]);
}
