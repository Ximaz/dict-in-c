/*
** XIMAZ PROJECTS, 2024
** tests_dict_vlaues.c
** File description:
** Unit tests for the dict values allocation and deallocation.
*/

#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "dict.h"

Test(dict_values, passing)
{
    uint64_t index = 0;
    dict_t *dict = dict_ctor();
    dict_values_t *dict_values = NULL;
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

    dict_values = dict_get_values(dict);
    dict_dtor(dict, NULL);

    cr_expect(ne(ptr, NULL, dict_values));
    cr_expect(eq(int, 10, dict_values->size));
    /**
     * NOTE: Here we compare addresses, as the values are referenced, not
     * copied. So we make sure it's the same address than the dummy one.
     */
    for (; index < dict_values->size; ++index)
        cr_expect(eq(ptr, (void *) my_value,
            (void *) dict_values->values[index]));
    dict_free_values(dict_values);
}
