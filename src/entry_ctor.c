/*
** XIMAZ PROJECTS, 2024
** entry_ctor.c
** File description:
** Exposes the entry object constructor.
*/

#include <stdlib.h>
#include "dict.h"

entry_t *entry_ctor(const char *key, void *value)
{
    entry_t *entry = (entry_t *) calloc(1, sizeof(entry_t));

    if (NULL != entry) {
        entry->key = key;
        entry->value = value;
    }
    return entry;
}
