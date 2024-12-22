/*
** XIMAZ PROJECTS, 2024
** dict_resize.c
** File description:
** Exposes a function to resize a dict and recompute all the key hashes.
*/

#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "murmurhash1.h"

/**
 * @brief This function is reponsible for allocating a new linked list buckets
 * array.
 *
 * If it fails, it deallocates all the successfully allocated linked list along
 * with the buckets array itself, and it returns a `NULL` pointer.
 *
 * @param new_size The number of buckets to allocate.
 * @return The allocated buckets array on success, `NULL` pointer on error.
 */
static
bucket_t **compute_new_buckets(uint64_t new_size)
{
    bucket_t **new_buckets = calloc(new_size, sizeof(bucket_t *));

    if (NULL == new_buckets)
        return NULL;
    if (-1 == dict_buckets_ctor(new_buckets, new_size)) {
        free(new_buckets);
        return NULL;
    }
    return new_buckets;
}

/**
 * @brief This function iterates over a non-empty bucket linked list.
 * It looks for all it's entries, re-hash the keys and tries to place the entry
 * to a new bucket.
 *
 * If the new bucket was not allocated, the function will crash. The buckets
 * array must have been allocated using the dict_buckets_ctor function, which
 * itself allocates all the subsequent linked list, so that the new_bucket is
 * never a `NULL` pointer.
 *
 * @param bucket The bucket from which to scan for entries.
 * @param new_buckets The linked list buckets array receiving the entries.
 * @param new_size The linked list buckets array size.
 */
static
void dict_rehash_bucket(const bucket_t *bucket, bucket_t **new_buckets,
    uint64_t new_size)
{
    uint32_t key_hash = 0;
    bucket_t **new_bucket = NULL;

    while (NULL != bucket && NULL != bucket->key) {
        key_hash = murmurhash1(bucket->key, strlen(bucket->key), HASH_SEED);
        new_bucket = &(new_buckets[DICT_BUCKET_IDX(key_hash, new_size)]);
        dict_bucket_insert(new_bucket, bucket->key, bucket->value);
        bucket = bucket->next;
    }
}

/**
 * @link https://github.com/python/cpython/blob/main/Python/hashtable.c#L108
 */
static
uint64_t round_size(uint64_t new_size)
{
    uint64_t i = 1;

    if (new_size < DICT_MIN_SIZE)
        return DICT_MIN_SIZE;
    while (i < new_size)
        i <<= 1;
    return i;
}

int dict_resize(dict_t *dict)
{
    uint64_t index = 0;
    uint64_t new_size = round_size(dict->size * DICT_RESIZE_FACTOR);
    bucket_t **new_buckets = compute_new_buckets(new_size);

    if (NULL == new_buckets)
        return -1;
    for (; index < dict->size; ++index)
        dict_rehash_bucket(dict->buckets[index], new_buckets, new_size);
    dict_buckets_dtor(dict->buckets, dict->size, NULL);
    free(dict->buckets);
    dict->buckets = new_buckets;
    dict->size = new_size;
    return 0;
}
