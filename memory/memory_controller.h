#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H
#include <stddef.h>

extern const size_t MEMORY_LIMIT_BYTES;

void* allocate_memory(size_t size);
void free_memory(void* ptr);
size_t get_memory_usage();
void check_available_memory(size_t size);
void* reallocate_memory(void* ptr, size_t new_size);

#endif