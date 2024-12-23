/*
** XIMAZ PROJECTS, 2024
** tests_dict_delete.c
** File description:
** Unit tests for the dict delete function.
*/

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "dict.h"

void fake_delete_key(__attribute__((unused)) char *key,
    __attribute__((unused)) void *value)
{}

Test(dict_delete, delete_from_unknown_position)
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
        "KEY6",
        "KEY7",
        "KEY8",
        "KEY4",
        "KEY9",
    };

    char *EXPECTED[] = {
        "KEY1",
        "KEY6",
        "KEY7",
        "KEY4",
        "KEY9",
    };

    for (; index < 6; ++index)
        cr_expect(eq(int, 0, dict_insert(dict, KEYS[index], 4,
            (void *) my_value)));
    dict_buckets_debug(dict->buckets, dict->size);
    dict_delete(dict, "KEY8", 4, fake_delete_key);
    dict_buckets_debug(dict->buckets, dict->size);
    dict_keys = dict_get_keys(dict);
    dict_dtor(dict, fake_delete_key);

    cr_expect(ne(ptr, NULL, dict_keys));
    cr_expect(eq(int, 5, dict_keys->size));
    /**
     * NOTE: Here we compare addresses, as the values are referenced, not
     * copied. So we make sure it's the same address than the dummy one.
     */
    for (index = 0; index < dict_keys->size; ++index)
        cr_expect(eq(ptr, (void *) EXPECTED[index],
                (void *) dict_keys->keys[index]));
    dict_free_keys(dict_keys);
}

Test(dict_delete, delete_from_head)
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
        "KEY6",
        "KEY7",
        "KEY8",
        "KEY4",
        "KEY9",
    };

    char *EXPECTED[] = {
        "KEY1",
        "KEY6",
        "KEY7",
        "KEY8",
        "KEY9",
    };

    for (; index < 6; ++index)
        cr_expect(eq(int, 0, dict_insert(dict, KEYS[index], 4,
            (void *) my_value)));

    dict_delete(dict, "KEY4", 4, fake_delete_key);
    dict_keys = dict_get_keys(dict);
    dict_dtor(dict, fake_delete_key);

    cr_expect(ne(ptr, NULL, dict_keys));
    cr_expect(eq(int, 5, dict_keys->size));
    /**
     * NOTE: Here we compare addresses, as the values are referenced, not
     * copied. So we make sure it's the same address than the dummy one.
     */
    for (index = 0; index < dict_keys->size; ++index)
        cr_expect(eq(ptr, (void *) EXPECTED[index],
            (void *) dict_keys->keys[index]));
    dict_free_keys(dict_keys);
}
