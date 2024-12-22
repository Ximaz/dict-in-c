/*
** XIMAZ PROJECTS, 2024
** murmurhash1.c
** File description:
** The MurmurHash1 algorithm, simplified for my use case.
** Credits : https://github.com/aappleby/smhasher
*/

#include <stdint.h>
#include "murmurhash1.h"

inline
uint32_t compute_padding(const uint8_t *data, uint64_t length, uint32_t h)
{
    if (3 == length)
        h += data[2] << 16;
    if (2 <= length)
        h += data[1] << 8;
    if (1 <= length) {
        h += data[0];
        h *= M;
        h ^= h >> R;
    }
    return h;
}

inline
uint32_t murmurhash1(const void *key, uint64_t length, uint32_t seed)
{
    uint32_t h = seed ^ (length * M);
    const uint8_t *data = (const uint8_t *) key;

    while (C <= length) {
        h += *(uint32_t *) data;
        h *= M;
        h ^= h >> R;
        data += C;
        length -= C;
    }
    h = compute_padding(data, length, h);
    h *= M;
    h ^= h >> 10;
    h *= M;
    h ^= h >> 17;
    return h;
}
