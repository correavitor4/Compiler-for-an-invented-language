#include "memory_controller.h"

#include "memory_controller.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Definição e inicialização da constante
const size_t MEMORY_LIMIT_BYTES = 2 * 1024 * 1024; // 2 MB

// Variável estática para controlar memória usada
static size_t memory_used = 0;

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

size_t get_memory_usage() {
    return memory_used;
}

void check_available_memory(size_t size) {
    if (memory_used + size > MEMORY_LIMIT_BYTES) {
        fprintf(stderr, "Memória insuficiente para alocar %zu bytes\n", size);
        exit(EXIT_FAILURE);
    }
}

void* reallocate_memory(void* ptr, size_t new_size) {
    if (!ptr) {
        // Se ptr é NULL, comporta-se como allocate_memory
        return allocate_memory(new_size);
    }

    MemHeader* old_header = ((MemHeader*)ptr) - 1;
    size_t old_total_size = old_header->size + sizeof(MemHeader);
    size_t new_total_size = new_size + sizeof(MemHeader);

    // Verifica se há memória suficiente para o aumento
    if (new_total_size > old_total_size) {
        size_t diff = new_total_size - old_total_size;
        if (memory_used + diff > MEMORY_LIMIT_BYTES) {
            fprintf(stderr, "Erro: limite de memória excedido na realocação. Pedido: %zu bytes, Usado: %zu bytes, Limite: %zu bytes\n",
                    new_size, memory_used, MEMORY_LIMIT_BYTES);
            return NULL;
        }
    }

    // Realoca o bloco incluindo o cabeçalho
    MemHeader* new_header = realloc(old_header, new_total_size);
    if (!new_header) {
        // Falha na realocação, ponteiro original permanece válido
        return NULL;
    }

    // Atualiza o tamanho no header e o controle de memória
    memory_used += (new_total_size - old_total_size);
    new_header->size = new_size;

    // Retorna o ponteiro para a área útil (após o cabeçalho)
    return (void*)(new_header + 1);
}
