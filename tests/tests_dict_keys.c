/*
** XIMAZ PROJECTS, 2024
** tests_dict_ctor.c
** File description:
** Unit tests for the dict keys allocation and deallocation.
*/

#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "dict.h"

Test(dict_keys, passing)
{
    dict_t *dict = dict_ctor();
    dict_keys_t *dict_keys = NULL;
    const void *my_value = "HI MOM!";

    cr_expect(eq(int, 0, dict_insert(dict, "KEY", 3, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY1", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY2", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY3", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY4", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY5", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY6", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY7", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY8", 4, (void *) my_value)));
    cr_expect(eq(int, 0, dict_insert(dict, "KEY9", 4, (void *) my_value)));

    dict_keys = dict_get_keys(dict);
    dict_dtor(dict, NULL);

    cr_expect(ne(ptr, NULL, dict_keys));
    cr_expect(eq(int, 10, dict_keys->size));
    dict_free_keys(dict_keys);
}
