/*
** XIMAZ PROJECTS, 2024
** entry_dtor.c
** File description:
** Exposes the entry object destructor.
*/

#include <stdlib.h>
#include "dict.h"

void entry_dtor(entry_t *entry)
{
    free(entry);
}
