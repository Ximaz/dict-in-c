/*
** XIMAZ PROJECTS, 2024
** dict_insert.c
** File description:
** Exposes a function to insert an entry into a dict.
*/

#include <stdlib.h>
#include "dict.h"

int dict_bucket_insert(bucket_t **bucket, entry_t *entry)
{
    bucket_t *node = (bucket_t *) calloc(1, sizeof(bucket_t));

    if (NULL == node)
        return -1;
    node->entry = entry;
    node->next = *bucket;
    *bucket = node;
    return 0;
}
