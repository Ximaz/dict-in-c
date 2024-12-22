/*
** XIMAZ PROJECTS, 2024
** murmurhash3.h
** File description:
** The MurmurHash1 algorithm, simplified for my use case.
** Credits : https://github.com/aappleby/smhasher
*/

#ifndef __MURMURHASH1_H_
#define __MURMURHASH1_H_

#include <stdint.h>

#define R 16
#define M 0xc6a4a793
#define C sizeof(uint32_t)

/**
 * @brief Computes the padding of the hash when it's length is not % uint32_t.
 *
 * @param data The remaining data to be processed.
 * @param length The remaning length of the data to be processed.
 * @param h The computed hash so far.
 * @return The new hash value.
 */
uint32_t compute_padding(const uint8_t *data, uint64_t length, uint32_t h);

/**
 * @brief Hashes the key into an unsigned 32 bits.
 *
 * @param key The key to hash.
 * @param length The length of the key.
 * @param seed The seed to use for the hash. Should be randomized once.
 * @return The hash result.
 */
uint32_t hash(const void *key, uint64_t length, uint32_t seed);

#endif /* !__MURMURHASH1_H_ */
