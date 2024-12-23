/*
** XIMAZ PROJECTS, 2024
** dict.h
** File description:
** Methods and Interfaces for dict data structure.
*/

#ifndef __DICT_H_
#define __DICT_H_

#include <stdint.h>

/**
 * @internal
 * @brief The default number of buckets to be allocated.
 */
#define DICT_MIN_SIZE (2 << 3)

/**
 * @internal
 * @brief The factor limit to be reach before allocating new buckets.
 */
#define DICT_HIGH 0.5

/**
 * @internal
 * @brief The factor limit to be reach before deallocating buckets.
 */
#define DICT_LOW 0.1

/**
 * @internal
 * @brief Once multiplied by the number of entries, returns the new dict size.
 */
#define DICT_RESIZE_FACTOR 2.0 / (DICT_HIGH + DICT_LOW)

/**
 * @internal
 * @brief Returns whether the dict must grow to accept new entries.
 *
 * @param D The dict to evaluate.
 */
#define DICT_MUST_GROW(D) ((float) (D)->items / (float) (D)->size) > DICT_HIGH

/**
 * @internal
 * @brief Returns whether the dict must shirnk to save some memory upon entry
 * removal.
 *
 * @param D The dict to evaluate.
 */
#define DICT_MUST_SHRINK(D) ((float) (D)->items / (float) (D)->size) < DICT_LOW

/**
 * @internal
 * @brief The hash seed to use. Mostly for security, but it has to be the same
 * all across the project.
 */
#define HASH_SEED 0

/**
 * @internal
 * @brief Returns the index to the bucket in which to store the entry.
 *
 * @param H The key hash.
 * @param S The size of the buckets array.
 */
#define DICT_BUCKET_IDX(H, S) (H) % (S)

/**
 * @brief Such function prototype represents the function to use to release the
 * memory allcoated to an entry value. If the value was not allocated (e.g. the
 * value is an integer), then you may use a NULL pointer instead to indicate
 * that the value has not to be free.
 *
 * @note There may be an upgrade in the future where either key or value may
 * be `NULL` pointers when calling. For instace, if the programmer tries to
 * insert different values with the same key, during the second insertion, the
 * value already in the dict will have to be deallocated, but not the key
 * itself. Now that I think about it, I wonder what would happen if one sets an
 * allocated key and then tries to insert the key with another allocated key
 * containing the same value, or worse, a stack-allocated key, which can't be
 * free then, as it would cause a bad free. The solution may be to return an
 * error when one tries to insert a key which already is set in the dict. They
 * will be in charge of removing it first, then inserting it with it's new
 * value.
 */
typedef void (*free_pair_t)(char *key, void *value);

/**
 * @brief The bucket type is a linked list node which contains both the key and
 * the value to store.
 */
typedef struct s_bucket {
    /** The key used to refer to the value. */
    char *key;

    /** The value to store, refered at via the key. */
    void *value;

    /** Pointer to the next entry. */
    struct s_bucket *next;
} bucket_t;

/**
 * @internal
 * @file src/dict_buckets_ctor.c
 * @brief Allocates new buckets linked list inside the array.
 *
 * If it failed to allocate one bucket, it free all the previously allocated
 * ones before returning -1.
 *
 * If existing buckets are already allocated inside the array, they will not
 * be free by this function. The programmer will have to free them first, even
 * though the purpose of this function is to allocate buckets of a new buckets
 * array that has just been allocated.
 *
 * If the buckets array is a `NULL` pointer, the function will crash.
 *
 * @param buckets The pre-allocated buckets array.
 * @param size The number of buckets to allocate inside the buckets array.
 * @return 0 on success, -1 on error.
 */
int dict_buckets_ctor(bucket_t **buckets, uint64_t size);

/**
 * @internal
 * @file src/dict_buckets_dtor.c
 * @brief Deallocates buckets linked list from the array.
 *
 * This function will not free the buckets array, just it's elements.
 *
 * If the buckets array is a `NULL` pointer, the function will crash.
 *
 * @param buckets The pre-allocated buckets array.
 * @param size The nuber of buckets to deallocate from the buckets array.
 * @param free_pair The function to use to free pair, may be `NULL`.
 */
void dict_buckets_dtor(bucket_t **buckets, uint64_t size,
    free_pair_t free_pair);

/**
 * @internal
 * @file src/dict_bucket_insert.c
 * @brief Inserts an entry into a dict bucket.
 *
 * If it failed to allocate the linked list bucket node, the bucket is left
 * unchanged and the function returns -1.
 *
 * If a `NULL` pointer is passed for bucket, the function will crash.
 *
 * @param bucket The pointer to the allocated bucket.
 * @param key The key of the pair.
 * @param value The value of the pair.
 * @return 0 on success, -1 on error.
 */
int dict_bucket_insert(bucket_t **bucket, char *key, void *value);

/**
 * @internal
 * @file src/dict_buckets_debug.c
 * @brief This function prints the content of each linked list bucket from the
 * buckets array.
 *
 * The debugging content will be flushed to the STDERR file descriptor (2).
 *
 * If `NULL` pointer is passed, or if the size is greater than the actual size
 * of the array, this function will crash.
 *
 * @param buckets The buckets to debug.
 * @param size The number of bucket inside the buckets array.
 */
void dict_buckets_debug(bucket_t *const *buckets, uint64_t size);

/**
 * @brief This structure represents the state of a dict (hashmap) object. Upon
 * insertion, the string keys are hashed using Murmurhash1 algorithm. They are
 * then stored inside a linked list (bucket), inside an array (buckets).
 *
 * Upon key insertion, if the number of items is greater than half the size,
 * the buckets array is enlarged and all the key hashes are re-computed.
 *
 * Upon key deletion, if the number of items is lower than the DICT_MIN_SIZE
 * value, the buckets array is reduced and all the key hashes are re-comupted.
 *
 * If a key is inserted twice, it's entry is overwritten to store the new value
 * associated to that key. If the entry value was already allocated, the memory
 * will not be released correctly, unless the programmer does it before using
 * the same key twice (or more).
 */
typedef struct s_dict {
    /** Total number of entries. */
    uint64_t items;

    /** Number of allocated buckets. */
    uint64_t size;

    /** Array of buckets linked list. */
    bucket_t **buckets;
} dict_t;

/**
 * @file src/dict_ctor.c
 * @brief Allocates a new dict.
 *
 * If it failed, returns `NULL`.
 *
 * @return The allocated dict.
 */
dict_t *dict_ctor(void);

/**
 * @file src/dict_dtor.c
 * @brief Deallocates the dict.
 *
 * The `free_pair` function will be in charge to free the key of the pair, if
 * not constant, and the value, if not constant. If both are constant, you can
 * pass a `NULL` pointer as nothing has to be free'd.
 *
 * If a `NULL` pointer is passed, or if the dict has already been deallocated,
 * the function will crash.
 *
 * @param dict The dict's pointer to deallocate.
 * @param free_pair The function to use to free pair, may be `NULL`.
 */
void dict_dtor(dict_t *dict, free_pair_t free_pair);

/**
 * @file src/dict_insert.c
 * @brief Insert an entry into the dict.
 *
 * The function allocates a new entry. If it was able to allocate AND insert
 * the entry, the function returns 0. If either one failed, it will return -1
 * and the dict is left unchanged.
 *
 * If a `NULL` pointer is passed, or if the dict has been deallocated, the
 * function will crash.
 *
 * @param dict The dict in which to insert the entry.
 * @param key The key to refer to the value.
 * @param key_length The length of the key.
 * @param value The value refered at via the key.
 * @return 0 on success, -1 on error.
 */
int dict_insert(dict_t *dict, char *key, uint64_t key_length, void *value);

/**
 * @internal
 * @file src/dict_resize.c
 * @brief Resizes the dict.
 *
 * This process implies that all the key hashes must be recomputed. Depending
 * on both the size of the dict and the length of the keys, the runtime may be
 * slow. We will used `strlen` instead of storing the length of the key as an
 * entry member for the following reasons :
 *
 * 1. upon creation, there is a chance that the programmer already knows the
 *   length of the key, so that we don't have to use the `strlen` function. If
 *   they don't know the length of the key, they are responsible for using such
 *   function;
 *
 * 2. the resize opeartion is complitly abstract to the programmer;
 *
 * 3. dict object are pretty heavy in memory, it could be nice to save 8 bytes
 *   per key, knowning CPU can compute pretty fast, and that the `strlen`
 *   function is probably optimized;
 *
 * If the dict could not be resized, it's unchanged and -1 is returned.
 *
 * If a `NULL` pointer is passed, or if the dict has been deallocated, the
 * function will crash.
 *
 * @param dict The dict to resize.
 * @return 0 on success, -1 on error.
 */
int dict_resize(dict_t *dict);

/**
 * @brief This structure represents the keys of the dict. It will be computed
 * each time the dict_keys() function is called. It will not be used by other
 * function to operate on the dict, as the internal dict state is known by all
 * functions.
 */
typedef struct s_dict_keys {
    /** Number of keys. Same value as the 'items' dict_t member. */
    uint64_t size;

    /**
     * The keys in a string array of length 'size'.
     *
     * @attention DO NOT TRY TO FREE THE MEMORY OF ANY ITEM FROM THAT ARRAY.
     */
    const char **keys;
} dict_keys_t;

/**
 * @file src/dict_get_keys.c
 * @brief Returns the list of the keys of the dict.
 *
 * If an error occured during the dict_keys_t object allocation, `NULL` pointer
 * is returned.
 *
 * @attention DO NOT TRY TO FREE ANY KEY INSIDE THE `keys` MEMBER BY YOURSELF.
 * The `keys` array member contains references to the dict keys, not a copy. If
 * you free one of them, it may break the whole dict object.
 *
 * The `dict_keys_t` object will have to be free using the `dict_free_keys`
 * function.
 *
 * If a `NULL` pointer is passed, or if the dict has already been deallocated,
 * the function will crash.
 *
 * @note The list of keys is not ordered. The keys are taken from each bucket
 * from 0 to the size of the dict, and each of the bucket is iterated over from
 * head to tail (linked list). Knowing that the insertion depends on a hash, we
 * don't know the order in which the keys will appear in the list.
 *
 * @param dict The dict from which to get the keys.
 * @return The list of keys.
 */
dict_keys_t *dict_get_keys(const dict_t *dict);

/**
 * @file src/dict_free_keys.c
 * @brief Release memory from `dict_get_keys` allocations.
 *
 * If a `NULL` pointer is passed, or if the dict keys have already been
 * deallocated, the function will crash.
 *
 * @param dict_keys The keys to free.
 */
void dict_free_keys(dict_keys_t *dict_keys);

/**
 * @brief This structure represents the values of the dict. It will be computed
 * each time the dict_values() function is called. It will not be used by other
 * function to operate on the dict, as the internal dict state is known by all
 * functions.
 */
typedef struct s_dict_values {
    /** Number of values. Same value as the 'items' dict_t member. */
    uint64_t size;

    /**
     * The values in a string array of length 'size'.
     *
     * @attention DO NOT TRY TO FREE THE MEMORY OF ANY ITEM FROM THAT ARRAY.
     */
    const void **values;
} dict_values_t;

/**
 * @file src/dict_get_values.c
 * @brief Returns the list of the values of the dict.
 *
 * If an error occured during the dict_values_t object allocation, `NULL`
 * pointer is returned.
 *
 * @attention DO NOT TRY TO FREE ANY KEY INSIDE THE `values` MEMBER BY
 * YOURSELF. The `values` array member contains references to the dict values,
 * not a copy. If you free one of them, it may break the whole dict object.
 *
 * The `dict_values_t` object will have to be free using the `dict_free_values`
 * function.
 *
 * If a `NULL` pointer is passed, or if the dict has already been deallocated,
 * the function will crash.
 *
 * @note The list of values is not ordered. The values are taken from each
 * bucket from 0 to the size of the dict, and each of the bucket is iterated
 * over from head to tail (linked list). Knowing that the insertion depends on
 * a hash, we don't know the order in which the values will appear in the list.
 *
 * @param dict The dict from which to get the values.
 * @return The list of values.
 */
dict_values_t *dict_get_values(const dict_t *dict);

/**
 * @file src/dict_free_values.c
 * @brief Release memory from `dict_get_values` allocations.
 *
 * If a `NULL` pointer is passed, or if the dict values have already been
 * deallocated, the function will crash.
 *
 * @param dict_values The values to free.
 */
void dict_free_values(dict_values_t *dict_keys);


#endif /* !__DICT_H_ */
