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
 * @internal
 * @brief This structure represents a key / value pair.
 */
typedef struct s_entry {
    /** The key used to refer to the value. */
    const char *key;

    /** The value to store, refered at via the key. */
    void *value;
} entry_t;

/**
 * @internal
 * @file src/entry_ctor.c
 * @brief Allocates a new entry. If it failed, returns `NULL`.
 *
 * @param key The key used to refer to the value.
 * @param value The value to store, refered at via the key.
 * @return The allocated entry.
 */
entry_t *entry_ctor(const char *key, void *value);

/**
 * @internal
 * @file src/entry_dtor.c
 * @brief Deallocates the entry.
 *
 * The deallocator will only free the entry itself. It will not free neither
 * the key nor the value. The programmer is in charge of entry's members
 * deallocation.
 *
 * If the entry has already been deallocated, the function will crash.
 *
 * @param entry The entry's pointer to deallocate.
 */
void entry_dtor(entry_t *entry);

typedef struct s_bucket {
    /** Entry to store. */
    entry_t *entry;

    /** Pointer to the next entry. */
    struct s_bucket *next;
} bucket_t;

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
 * @brief This structure represents the keys of the dict. It will be computed
 * each time the dict_keys() function is called. It will not be used by other
 * function to operate on the dict, as the internal dict state is known by all
 * functions.
 */
typedef struct s_dict_keys {
    /** Number of keys. Same value as the 'items' dict_t member. */
    uint64_t size;

    /** The keys in a string array of length 'size'.  */
    const char **keys;
} dict_keys_t;

/**
 * @brief This structure represents the values of the dict. It will be computed
 * each time the dict_values() function is called. It will not be used by other
 * function to operate on the dict, as the internal dict state is known by all
 * functions.
 */
typedef struct s_dict_values {
    /** Number of values. Same value as the 'items' dict_t member. */
    uint64_t size;

    /** The values in a string array of length 'size'.  */
    const void **values;
} dict_values_t;

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
 * @brief Such function prototype represents the function to use to release the
 * memory allcoated to an entry value. If the value was not allocated (e.g. the
 * value is an integer), then you may use a NULL pointer instead to indicate
 * that the value has not to be free.
 */
typedef void (*free_value_t)(void *value);

/**
 * @internal
 * @file src/dict_buckets_dtor.c
 * @brief Deallocates buckets linked list from the array.
 *
 * This function will not free the buckets array, just it's elements.
 *
 * If the buckets array is a `NULL` pointer, the function will crash.
 *
 * If `free_entries` equals 0 but the `free_value` is not `NULL`, the function
 * will not free entries, thus `free_value` will not be called. If you want to
 * free the entries value, make sure to set `free_entries` to 1.
 *
 * @param buckets The pre-allocated buckets array.
 * @param size The nuber of buckets to deallocate from the buckets array.
 * @param free_entries Whether to free the entries inside each bucket. (1 | 0).
 * @param free_value The function to use to free entries value, may be `NULL`.
 */
void dict_buckets_dtor(bucket_t **buckets, uint64_t size, int free_entries,
    free_value_t free_value);

/**
 * @file src/dict_dtor.c
 * @brief Deallocates the dict.
 *
 * The deallocator will only free the buckets array and the dict. It will not
 * free the entries. The programmer is in charge of entries deallocation.
 *
 * If a `NULL` pointer is passed, or if the dict has already been deallocated,
 * the function will crash.
 *
 * @param dict The dict's pointer to deallocate.
 * @param free_value The function to use to free entries value, may be `NULL`.
 */
void dict_dtor(dict_t *dict, free_value_t free_value);

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
 * @param entry The entry to insert.
 * @return 0 on success, -1 on error.
 */
int dict_bucket_insert(bucket_t **bucket, entry_t *entry);

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
int dict_insert(dict_t *dict, const char *key, uint64_t key_length,
    void *value);

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

#endif /* !__DICT_H_ */
