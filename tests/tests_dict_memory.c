/*
** XIMAZ PROJECTS, 2024
** tests_dict_ctor.c
** File description:
** Unit tests for the dict constructor and destructor.
*/

#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "dict.h"

Test(dict_ctor_and_dtor, passing)
{
    dict_t *dict = dict_ctor();

    cr_expect(ne(ptr, NULL, dict));
    cr_expect(eq(int, 0, dict->items));
    cr_expect(eq(int, DICT_MIN_SIZE, dict->size));
    cr_expect(ne(ptr, NULL, dict->buckets));
    dict_dtor(dict, NULL);
}
