#include "memory_controller.h"

#include "memory_controller.h"
#include <stdlib.h>
#include <stdio.h>

// Definição e inicialização da constante
const size_t MEMORY_LIMIT_BYTES = 2 * 1024 * 1024; // 2 MB

// Variável estática para controlar memória usada
static size_t memory_used = 0;

// Estrutura para armazenar o tamanho da alocação junto com o ponteiro
typedef struct {
    size_t size;
    // Pode adicionar mais campos se quiser
} MemHeader;

// Aloca memória com controle de uso
void* allocate_memory(size_t size) {
    size_t total_size = size + sizeof(MemHeader);

    if (memory_used + total_size > MEMORY_LIMIT_BYTES) {
        fprintf(stderr, "Erro: limite de memória excedido. Pedido: %zu bytes, Usado: %zu bytes, Limite: %zu bytes\n",
                size, memory_used, MEMORY_LIMIT_BYTES);
        return NULL;
    }

    MemHeader* header = (MemHeader*) malloc(total_size);
    if (!header) return NULL;

    header->size = size;
    memory_used += total_size;

    // Retorna o ponteiro logo após o header
    return (void*)(header + 1);
}

// Libera memória e atualiza uso
void free_memory(void* ptr) {
    if (!ptr) return;

    MemHeader* header = ((MemHeader*)ptr) - 1;
    memory_used -= (header->size + sizeof(MemHeader));
    free(header);
}

void get_memory_usage(size_t* used, size_t* total) {
    if (used) *used = memory_used;
    if (total) *total = MEMORY_LIMIT_BYTES;
}

void check_available_memory(size_t size) {
    if (memory_used + size > MEMORY_LIMIT_BYTES) {
        fprintf(stderr, "Memória insuficiente para alocar %zu bytes\n", size);
        exit(EXIT_FAILURE);
    }
}
