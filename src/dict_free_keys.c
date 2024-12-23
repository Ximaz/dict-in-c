/*
** XIMAZ PROJECTS, 2024
** dict_free_keys.c
** File description:
** Exposes a function to free the allocate memory from dict_get_keys function.
*/

#include <stdlib.h>
#include "dict.h"

void dict_free_keys(dict_keys_t *dict_keys)
{
    free(dict_keys->keys);
    free(dict_keys);
}
