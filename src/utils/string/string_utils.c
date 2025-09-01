#include <string.h>

#include "string_utils.h"
#include "memory/memory_controller.h"

char* string_copy(const char* src) {
    size_t len = strlen(src);

    char* dest = allocate_memory(len + 1);

    if(dest == NULL) return NULL;

    strcpy(dest, src);
}
