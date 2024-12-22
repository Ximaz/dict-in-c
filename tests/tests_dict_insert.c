/*
** XIMAZ PROJECTS, 2024
** tests_dict_insert.c
** File description:
** Unit tests for the dict insert function.
*/

#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "dict.h"

Test(dict_insert, passing)
{
    dict_t *dict = dict_ctor();
    char *my_value = "Hello, World !";

    cr_expect(eq(int, 0, dict_insert(dict, "KEY", 3, (void *) my_value)));
    dict_dtor(dict);
}
