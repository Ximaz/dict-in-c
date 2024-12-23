/*
** XIMAZ PROJECTS, 2024
** dict_bucket_delete.c
** File description:
** Exposes a function to delete an entry from a dict bucket.
*/

#include <stdlib.h>
#include "dict.h"

/**
 * @brief Deletes the node of a bucket once it has been identified by the key.
 *
 * If no node matched the key, -1 is returned as an error and the bucket is
 * unchanged.
 *
 * @note The key is not marked as const as it may get free'd by the `free_pair`
 * function, but bear in mind the current function will not modify the key.
 *
 * @param node The bucket's front node from which to delete the entry.
 * @param key The key used to match the entry to delete.
 * @param free_pair The function used to release the key and value memory.
 * @return 0 on success, -1 on error.
 */
inline static
int dict_bucket_delete_until(bucket_t *node, char *key, free_pair_t free_pair)
{
    bucket_t *prev = NULL;

    while (NULL != node->key) {
        if (!DICT_KEY_MATCH(node->key, key)) {
            prev = node;
            node = node->next;
            continue;
        }
        prev->next = node->next;
        if (NULL != free_pair)
            free_pair(node->key, node->value);
        free(node);
        return 0;
    }
    return -1;
}

int dict_bucket_delete(bucket_t **bucket, char *key, free_pair_t free_pair)
{
    bucket_t *node = *bucket;

    if (!DICT_KEY_MATCH(node->key, key))
        return dict_bucket_delete_until(node, key, free_pair);
    *bucket = node->next;
    if (NULL != free_pair)
        free_pair(node->key, node->value);
    free(node);
    return 0;
}
