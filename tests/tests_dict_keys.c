/*
** XIMAZ PROJECTS, 2024
** tests_dict_keys.c
** File description:
** Unit tests for the dict keys allocation and deallocation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "dict.h"

Test(dict_keys, passing)
{
    uint64_t index = 0;
    dict_t *dict = dict_ctor();
    dict_keys_t *dict_keys = NULL;
    const void *my_value = "HI MOM!";
    /** The order here is special as we don't know the order of keys after they
     * are inserted. So we make the KEYS match the order at the beginning so
     * that we can make the end for-loop test work just fine as it will match
     * the order.
     */
    char *KEYS[] = {
        "KEY1",
        "KEY4",
        "KEY6",
        "KEY7",
        "KEY8",
        "KEY9",
        "KEY5",
        "KEY3",
        "KEY0",
        "KEY2",
    };

    for (; index < 10; ++index)
        cr_expect(eq(int, 0, dict_insert(dict, KEYS[index], 4,
            (void *) my_value)));
    dict_keys = dict_get_keys(dict);
    dict_dtor(dict, NULL);

    cr_expect(ne(ptr, NULL, dict_keys));
    cr_expect(eq(int, 10, dict_keys->size));
    /**
     * NOTE: Here we compare addresses, as the values are referenced, not
     * copied. So we make sure it's the same address than the dummy one.
     */
    for (index = 0; index < dict_keys->size; ++index)
        cr_expect(eq(ptr, (void *) KEYS[index],
            (void *) dict_keys->keys[index]));
    dict_free_keys(dict_keys);
}
