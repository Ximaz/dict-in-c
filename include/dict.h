/*
** XIMAZ PROJECTS, 2024
** dict.h
** File description:
** Methods and Interfaces for dict data structure.
*/

#ifndef __DICT_H_
#define __DICT_H_

#include <stdint.h>
#include <string.h>

/** @cond INTERNAL */

/**
 * @brief The default number of buckets to be allocated.
 */
#define DICT_MIN_SIZE (2 << 3)

/**
 * @brief The factor limit to be reach before allocating new buckets.
 */
#define DICT_HIGH 0.5

/**
 * @brief The factor limit to be reach before deallocating buckets.
 */
#define DICT_LOW 0.1

/**
 * @brief Once multiplied by the number of entries, returns the new dict size.
 */
#define DICT_RESIZE_FACTOR 2.0 / (DICT_HIGH + DICT_LOW)

/**
 * @brief Returns whether the dict must grow to accept new entries.
 *
 * @param D The dict to evaluate.
 */
#define DICT_MUST_GROW(D) ((float) (D)->items / (float) (D)->size) > DICT_HIGH

/**
 * @brief Returns whether the dict must shirnk to save some memory upon entry
 * removal.
 *
 * @param D The dict to evaluate.
 */
#define DICT_MUST_SHRINK(D) ((float) (D)->items / (float) (D)->size) < DICT_LOW

/**
 * @brief The hash seed to use. Mostly for security, but it has to be the same
 * all across the project.
 */
#define HASH_SEED 0

/**
 * @brief Returns the index to the bucket in which to store the entry.
 *
 * @param H The key hash.
 * @param S The size of the buckets array.
 */
#define DICT_BUCKET_IDX(H, S) (H) % (S)

/**
 * @brief Returns whether two keys from a dict are matching.
 *
 * @param K1 The first key to compare.
 * @param K2 The second key to compare.
 */
#define DICT_KEY_MATCH(K1, K2) (0 == strcmp((K1), (K2)))

/** @endcond INTERNAL */

/**
 * @brief Returns the number of items inside the dict. Not to be mixed up with
 * the dict size which represents the numebr of allocated buckets.
 *
 * @param D The dict to get the number of items of.
 */
#define DICT_SIZE(D) (D)->items

/**
 * @brief Such function prototype represents the function to use to release the
 * memory allcoated to an entry value. If the value was not allocated (e.g. the
 * value is an integer), then you may use a `NULL` pointer instead to indicate
 * that the value has not to be free.
 */
typedef void (*free_pair_t)(char *key, void *value);

/** @cond INTERNAL */

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
 * @warning If the buckets array is a `NULL` pointer, the function will crash.
 *
 * @param buckets The pre-allocated buckets array.
 * @param size The number of buckets to allocate inside the buckets array.
 * @return 0 on success, -1 on error.
 */
int dict_buckets_ctor(bucket_t **buckets, uint64_t size);

/**
 * @brief Deallocates buckets linked list from the array.
 *
 * @warning If the buckets array is a `NULL` pointer, the function will crash.
 *
 * @note This function will not free the buckets array, just it's elements.
 *
 * @param buckets The pre-allocated buckets array.
 * @param size The nuber of buckets to deallocate from the buckets array.
 * @param free_pair The function to use to free pair, may be `NULL`.
 */
void dict_buckets_dtor(bucket_t **buckets, uint64_t size,
    free_pair_t free_pair);

/**
 * @brief Returns whether a key is present in the bucket.
 *
 * @warning If a `NULL` pointer is passed for bucket, the function will crash.
 *
 * @note If the key is `NULL` a falsy value may be returned. Make sure it's not
 * before calling the function.
 *
 * @param bucket The bucket in which to look for the key.
 * @param key The key to look for in the bucket.
 * @return 1 if present, 0 if not present.
 */
int dict_bucket_has_key(const bucket_t *bucket, const char *key);

/**
 * @brief Inserts an entry into a dict bucket.
 *
 * @warning If a `NULL` pointer is passed for bucket, the function will crash.
 *
 * @note If it failed to allocate the linked list bucket node, the bucket is
 * left unchanged and the function returns -1.
 *
 * @param bucket The pointer to the allocated bucket.
 * @param key The key of the pair.
 * @param value The value of the pair.
 * @return 0 on success, -1 on error.
 */
int dict_bucket_insert(bucket_t **bucket, char *key, void *value);

/**
 * @brief Deletes an entry from the bucket based on the key.
 *
 * If no node matched the key, -1 is returned as an error and the bucket is
 * unchanged.
 *
 * @warning If a `NULL` pointer is passed for either the bucket or the key, the
 * function will crash.
 *
 * @note The key is not marked as const as it may get free'd by the `free_pair`
 * function, but bear in mind the current function will not modify the key.
 *
 * @note If the `free_pair` function is a `NULL` pointer, it's not called. It
 * may be useful if neither the key nor the value were allocated.
 *
 * @param bucket The bucket from which to remove the entry.
 * @param key The key used to match the entry to be removed.
 * @param free_pair The function called to release the key and value memory.
 * @return 0 on success, -1 on error.
 */
int dict_bucket_delete(bucket_t **bucket, char *key, free_pair_t free_pair);

/**
 * @brief This function prints the content of each linked list bucket from the
 * buckets array.
 *
 * @warning If `NULL` pointer is passed, or if the size is greater than the
 * actual size of the array, this function will crash.
 *
 * @note The debugging content will be flushed to the `stderr` file descriptor.
 *
 * @param buckets The buckets to debug.
 * @param size The number of bucket inside the buckets array.
 */
void dict_buckets_debug(bucket_t *const *buckets, uint64_t size);

/** @endcond INTERNAL */

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
 * @brief Allocates a new dict.
 *
 * @note If it failed, returns a `NULL` pointer.
 *
 * @return The allocated dict.
 */
dict_t *dict_ctor(void);

/**
 * @brief Deallocates the dict.
 *
 * The `free_pair` function will be in charge to free the key of the pair, if
 * not constant, and the value, if not constant. If both are constant, you can
 * pass a `NULL` pointer as nothing has to be free'd.
 *
 * @warning If a `NULL` pointer is passed, or if the dict has already been
 * deallocated, the function will crash.
 *
 * @param dict The dict's pointer to deallocate.
 * @param free_pair The function to use to free pair, may be `NULL`.
 */
void dict_dtor(dict_t *dict, free_pair_t free_pair);

/**
 * @brief Insert an entry into the dict.
 *
 * The function allocates a new entry. If it was able to allocate AND insert
 * the entry, the function returns 0. If either one failed, it will return -1
 * and the dict is left unchanged.
 *
 * @warning If a `NULL` pointer is passed, or if the dict has been deallocated,
 * the function will crash.
 *
 * @note If the key is `NULL` some functions may break. Make sure it's not
 * before calling the function.
 *
 * @param dict The dict in which to insert the entry.
 * @param key The key to refer to the value.
 * @param key_length The length of the key.
 * @param value The value refered at via the key.
 * @return 0 on success, -1 on error.
 */
int dict_insert(dict_t *dict, char *key, uint64_t key_length, void *value);

/**
 * @brief Deletes an entry from the dict.
 *
 * The function tries to delete an entry based on the given key. If the key was
 * not found, the function returns -1 as an error. If the entry is found, it's
 * key and value are given as parameter of the `free_pair` function to let the
 * programmer handle the memory release, if needed.
 *
 * @warning If `dict` or `key` is a `NULL` pointer, the function will crash.
 *
 * @note The key is not marked as const as it may get free'd by the `free_pair`
 * function, but bear in mind the current function will not modify the key.
 *
 * @note If the `free_pair` function is a `NULL` pointer, it's not called. It
 * may be useful if neither the key nor the value were allocated.
 *
 * @param dict The dict from which the pair must be deleted.
 * @param key The key referring to the pair which must be deleted.
 * @param key_length The length of the key. If unknowned, use `strlen(key)`.
 * @param free_pair The function called to release key and value memory.
 * @return 0 on success, -1 on error.
 */
int dict_delete(dict_t *dict, char *key, uint64_t key_length,
    free_pair_t free_pair);

/** @cond INTERNAL */

/**
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
 * @warning If a `NULL` pointer is passed, or if the dict has been deallocated,
 * the function will crash.
 *
 * @note If the dict could not be resized, it's unchanged and -1 is returned.
 *
 * @param dict The dict to resize.
 * @return 0 on success, -1 on error.
 */
int dict_resize(dict_t *dict);

/** @endcond */

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
 * @brief Returns the list of the keys of the dict.
 *
 * If an error occured during the dict_keys_t object allocation, `NULL` pointer
 * is returned.
 *
 * The `dict_keys_t` object will have to be free using the `dict_free_keys`
 * function.
 *
 * @warning If a `NULL` pointer is passed, or if the dict has already been
 * deallocated, the function will crash.
 *
 * @warning DO NOT TRY TO FREE ANY ITEM INSIDE THE `keys` MEMBER BY YOURSELF.
 * The `keys` array member contains references to the keys, not copies. If you
 * free one of them, it may break the whole dict object.
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
 * @brief Returns the list of the values of the dict.
 *
 * If an error occured during the dict_values_t object allocation, `NULL`
 * pointer is returned.
 *
 * The `dict_values_t` object will have to be free using the `dict_free_values`
 * function.
 *
 * @warning If a `NULL` pointer is passed, or if the dict has already been
 * deallocated, the function will crash.
 *
 * @warning DO NOT TRY TO FREE ANY ITEM INSIDE THE `values` MEMBER BY YOURSELF.
 * The `values` array member contains references to the values, not copies. If
 * you free one of them, it may break the whole dict object.
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
 * @brief Release memory from `dict_get_values` allocations.
 *
 * If a `NULL` pointer is passed, or if the dict values have already been
 * deallocated, the function will crash.
 *
 * @param dict_values The values to free.
 */
void dict_free_values(dict_values_t *dict_values);

#endif /* !__DICT_H_ */
