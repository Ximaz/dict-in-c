/*
** XIMAZ PROJECTS, 2024
** dict_buckets_dtor.c
** File description:
** Exposes a function used to deallocate a buckets linked list array.
*/

#include <stdlib.h>
#include "dict.h"

/**
 * @brief Deallocate a bucket and free it's content using `free_pair`.
 *
 * @note If `free_pair` is a `NULL` pointer, it means that we don't have to
 * free neither keys nor values. We branch depending on whether `free_pair` is
 * a `NULL` pointer so that we save branching on iteration.
 *
 * @param bucket The bucket to free.
 * @param free_pair The optional function to free keys and values.
 */
static
void dict_bucket_dtor(bucket_t *bucket, free_pair_t free_pair)
{
    bucket_t *next = NULL;

    if (NULL != free_pair)
        while (NULL != bucket) {
            next = bucket->next;
            free_pair(bucket->key, bucket->value);
            free(bucket);
            bucket = next;
        }
    else
        while (NULL != bucket) {
            next = bucket->next;
            free(bucket);
            bucket = next;
        }
}

void dict_buckets_dtor(bucket_t **buckets, uint64_t size,
    free_pair_t free_pair)
{
    uint64_t index = 0;

    for (; index < size; ++index)
        dict_bucket_dtor(buckets[index], free_pair);
}
