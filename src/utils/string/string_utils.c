#include <string.h>

#include "string_utils.h"
#include "memory/memory_controller.h"

char* strdup(const char* src) {
    if (!src) return NULL;
    size_t len = strlen(src) + 1; // +1 for the null terminator
    char* new_str = allocate_memory(len);
    if (new_str) {
        memcpy(new_str, src, len);
    }
    return new_str;
}
