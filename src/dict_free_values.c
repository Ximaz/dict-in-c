/*
** XIMAZ PROJECTS, 2024
** dict_free_values.c
** File description:
** Exposes a function to free the allocate memory from dict_get_values
** function.
*/

#include <stdlib.h>
#include "dict.h"

void dict_free_values(dict_values_t *dict_values)
{
    free(dict_values->values);
    free(dict_values);
}
