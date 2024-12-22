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

typedef struct s_entry {
    /** The key used to refer to the value. */
    const char *key;

    /** The value to store, refered at via the key. */
    void *value;
} entry_t;

/**
 * @brief Allocates a new entry. If it failed, returns `NULL`.
 *
 * @param key The key used to refer to the value.
 * @param value The value to store, refered at via the key.
 * @return The allocated entry.
 */
entry_t *entry_ctor(const char *key, void *value);

/**
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
    const entry_t *entry;

    /** Pointer to the next entry. */
    struct s_bucket *next;
} bucket_t;

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
 * If it failed, returns `NULL`.
 *
 * @return The allocated dict.
 */
dict_t *dict_ctor(void);

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
 * If the buckets array is a `NULL` pointer, the function will crash.
 *
 * @param buckets The pre-allocated buckets array.
 * @param size The number of buckets to allocate inside the buckets array.
 * @return 0 on success, -1 on error.
 */
int dict_buckets_ctor(bucket_t **buckets, uint64_t size);

/**
 * @brief Deallocates buckets linked list from the array.
 *
 * This function will not free the buckets array, just it's elements.
 *
 * If the buckets array is a `NULL` pointer, the function will crash.
 *
 * @param buckets The pre-allocated buckets array.
 * @param size The nuber of buckets to deallocate from the buckets array.
 */
void dict_buckets_dtor(bucket_t **buckets, uint64_t size);

/**
 * @brief Deallocates the dict.
 *
 * The deallocator will only free the buckets array and the dict. It will not
 * free the entries. The programmer is in charge of entries deallocation.
 *
 * If a `NULL` pointer is passed, or if the dict has already been deallocated,
 * the function will crash.
 *
 * @param dict The dict's pointer to deallocate.
 */
void dict_dtor(dict_t *dict);

/**
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
int dict_bucket_insert(bucket_t **bucket, const entry_t *entry);

/**
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
