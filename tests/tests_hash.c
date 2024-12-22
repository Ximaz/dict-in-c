/*
** XIMAZ PROJECTS, 2024
** tests_hash.c
** File description:
** Unit tests for the MurmurHash1 function.
*/

#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "murmurhash1.h"

#define KEY_VALUE "Hello, World !!"
#define KEY_LENGTH 15
#define SEED 0
#define EXPECTED 1637579608

Test(hash, passing_with_expected_value)
{
    const char *key = KEY_VALUE;

    uint32_t hash_value = hash((const void *) key, KEY_LENGTH, SEED);

    cr_expect(eq(int, hash_value, EXPECTED));
}

Test(hash, passing_with_heap)
{
    const char *key1 = KEY_VALUE;
    char *key2 = malloc(sizeof(char) * KEY_LENGTH + 1);

    memset(key2, 0, KEY_LENGTH + 1);
    strcpy(key2, key1);
    uint32_t hash1 = hash((const void *) key1, KEY_LENGTH, SEED);
    uint32_t hash2 = hash((const void *) key2, KEY_LENGTH, SEED);

    cr_expect(eq(int, hash1, hash2));
    free(key2);
}

Test(hash, passing_with_stack)
{
    const char *key1 = KEY_VALUE;
    const char *key2 = KEY_VALUE;

    uint32_t hash1 = hash((const void *) key1, KEY_LENGTH, SEED);
    uint32_t hash2 = hash((const void *) key2, KEY_LENGTH, SEED);

    cr_expect(eq(int, hash1, hash2));
}

Test(hash, passing_with_same_pointer)
{
    const char *key = KEY_VALUE;

    uint32_t hash1 = hash((const void *) key, KEY_LENGTH, SEED);
    uint32_t hash2 = hash((const void *) key, KEY_LENGTH, SEED);

    cr_expect(eq(int, hash1, hash2));
}
