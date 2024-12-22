/*
** XIMAZ PROJECTS, 2024
** dict_ctor.c
** File description:
** Exposes the dict object destructor.
*/

#include <stdlib.h>
#include "dict.h"

void dict_dtor(dict_t *dict, free_value_t free_value)
{
    dict_buckets_dtor(dict->buckets, dict->size, 1, free_value);
    free(dict->buckets);
    free(dict);
}
