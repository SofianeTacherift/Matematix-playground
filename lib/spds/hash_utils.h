
#ifndef HASH_UTILS_H

#define HASH_UTILS_H

#include <stdbool.h>
#include <stddef.h>

static bool needs_resize(const size_t size, const size_t capacity){
    if ( (( size >> ((sizeof(size_t) * 8) -2) ) & 3L) == 0L) {
        const bool no_capacity_overflow = ( ((capacity >> (sizeof(size_t) * 8) -1) ) & 1L ) == 0;
        return no_capacity_overflow && size>= ((3*capacity) >> 2);
    }
    return false;
}


#endif