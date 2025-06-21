#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H
#include <stddef.h>

extern const size_t MEMORY_LIMIT_BYTES;

void* allocate_memory(size_t size);
void free_memory(void* ptr);
void get_memory_usage(size_t* used, size_t* total);
void check_available_memory(size_t size);

#endif