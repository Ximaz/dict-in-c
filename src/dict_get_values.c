/*
** XIMAZ PROJECTS, 2024
** dict_get_values.c
** File description:
** Exposes a function to get the list of values of a dict.
*/

#include <assert.h>
#include <stdlib.h>
#include "dict.h"

/**
 * @brief This function will extract the values from each bucket of the dict
 * and place their reference to the `values` member of the `values` array.
 *
 * @param dict The dict to get the values from.
 * @param values The values object in which to set the values.
 */
static
void populate_values(const dict_t *dict, dict_values_t *values)
{
    uint64_t index = 0;
    const bucket_t *bucket = NULL;

    for (; index < dict->size; ++index) {
        bucket = dict->buckets[index];
        while (NULL != bucket->key) {
            values->values[values->size++] = bucket->value;
            bucket = bucket->next;
        }
    }
    assert(values->size == dict->items);
}

dict_values_t *dict_get_values(const dict_t *dict)
{
    dict_values_t *values = (dict_values_t *) calloc(1, sizeof(dict_values_t));

    if (NULL == values)
        return NULL;
    values->values = (const void **) calloc(dict->items, sizeof(void *));
    if (NULL == values->values) {
        free(values);
        return NULL;
    }
    populate_values(dict, values);
    return values;
}
