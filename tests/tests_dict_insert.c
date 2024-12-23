/*
** XIMAZ PROJECTS, 2024
** tests_dict_insert.c
** File description:
** Unit tests for the dict insert function.
*/

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "dict.h"

Test(dict_insert, passing)
{
    dict_t *dict = dict_ctor();
    char *my_value = "Hello, World !";

    cr_expect(eq(int, 0, dict_insert(dict, "KEY0", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY1", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY2", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY3", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY4", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY5", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY6", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY7", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY8", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY9", 4, (void *) my_value)));
    cr_expect(eq(int, -1, dict_insert(dict, "KEY9", 4, (void *) my_value)));
    dict_dtor(dict, NULL);
}
