#include "hash.h"

/**
 * @brief Computes the djb2 hash for a given null-terminated string.
 *
 * This function implements the djb2 hash algorithm, which is a simple and fast
 * hash function created by Dan Bernstein. It is commonly used for hashing strings.
 *
 * @param str Pointer to the null-terminated input string to hash.
 * @return The computed hash value as an unsigned long.
 */
unsigned long _djb2_impl(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        // hash = hash * 33 + c
        hash = ((hash << 5) + hash) + c; 
    }

    return hash;
}

/**
 * @brief  Global constant instance of the HASH_API structure.
 *
 * This object provides the interface and implementation for hash-related
 * operations. It is intended to be used as the main entry point for
 * hashing functionality within the application.
 *
 * @note  The actual contents and functions provided by HASH_API should be
 *        documented in the corresponding header file.
 */
const HASH_API HASH = {
    .djb2 = _djb2_impl
};