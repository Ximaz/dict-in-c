/*
** XIMAZ PROJECTS, 2024
** dict_ctor.c
** File description:
** Exposes the dict object constructor.
*/

#include <stdlib.h>
#include "dict.h"

/* TODO: The way linked list buckets are created is wrong. */
dict_t *dict_ctor(void)
{
    dict_t *dict = (dict_t *) calloc(1, sizeof(dict_t));

    if (NULL == dict)
        return NULL;
    dict->buckets = (bucket_t *) calloc(DICT_MIN_SIZE, sizeof(bucket_t));
    if (NULL == dict->buckets) {
        free(dict);
        return NULL;
    }
    dict->items = 0;
    dict->size = DICT_MIN_SIZE;
    return dict;
}
