/*
** XIMAZ PROJECTS, 2024
** dict_buckets_dtor.c
** File description:
** Exposes a function used to deallocate a buckets linked list array.
*/

#include <stdlib.h>
#include "dict.h"

static
void dict_bucket_dtor(bucket_t *bucket)
{
    bucket_t *next = NULL;

    while (NULL != bucket) {
        next = bucket->next;
        free(bucket);
        bucket = next;
    }
}

static
void dict_bucket_dtor_with_entries(bucket_t *bucket, free_value_t free_value)
{
    bucket_t *next = NULL;

    while (NULL != bucket) {
        next = bucket->next;
        if (NULL != free_value)
            free_value(bucket->entry);
        free(bucket);
        bucket = next;
    }
}

void dict_buckets_dtor(bucket_t **buckets, uint64_t size, int free_entry,
    free_value_t free_value)
{
    uint64_t index = 0;

    for (; index < size; ++index) {
        if (1 == free_entry)
            dict_bucket_dtor_with_entries(buckets[index], free_value);
        else
            dict_bucket_dtor(buckets[index]);
    }
}
