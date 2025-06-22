#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H
#include <stddef.h>

extern const size_t MEMORY_LIMIT_BYTES;

// Estrutura para armazenar o tamanho da alocação junto com o ponteiro
typedef struct {
    size_t size;
    // Pode adicionar mais campos se quiser
} MemHeader;

void* allocate_memory(size_t size);
void free_memory(void* ptr);
size_t get_memory_usage();
void check_available_memory(size_t size);
void* reallocate_memory(void* ptr, size_t new_size);

#endif