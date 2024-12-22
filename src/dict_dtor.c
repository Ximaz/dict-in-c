/*
** XIMAZ PROJECTS, 2024
** dict_ctor.c
** File description:
** Exposes the dict object destructor.
*/

#include <stdlib.h>
#include "dict.h"

void dict_dtor(dict_t *dict)
{
    free(dict->buckets);
    free(dict);
}
