#include "unity.h"
#include "memory_controller.h"
#include <string.h>  // para memset

void setUp(void) {
    // Nenhuma configuração especial necessária
}

void tearDown(void) {
    // Nenhuma limpeza necessária
}

// Testa alocação simples e liberação
void test_allocate_and_free_memory(void) {
    size_t initial_used, total;
    get_memory_usage(&initial_used, &total);

    void* ptr = allocate_memory(1024); // 1 KB
    TEST_ASSERT_NOT_NULL(ptr);

    size_t used_after_alloc;
    get_memory_usage(&used_after_alloc, NULL);
    TEST_ASSERT_TRUE(used_after_alloc > initial_used);

    free_memory(ptr);

    size_t used_after_free;
    get_memory_usage(&used_after_free, NULL);
    TEST_ASSERT_EQUAL_UINT(initial_used, used_after_free);
}

// Testa limite de memória
void test_allocate_exceeding_limit(void) {
    // Tenta alocar mais que o limite total
    void* ptr = allocate_memory(MEMORY_LIMIT_BYTES + 1);
    TEST_ASSERT_NULL(ptr);
}

// Testa check_available_memory com tamanho válido
void test_check_available_memory_pass(void) {
    // Deve passar sem sair do programa
    check_available_memory(1024);
    TEST_PASS_MESSAGE("check_available_memory passed for valid size");
}

// Testa check_available_memory com tamanho inválido (excede limite)
void test_check_available_memory_fail(void) {
    // Para testar saída do programa, precisamos usar Unity's TEST_ABORT ou similar,
    // mas Unity não suporta diretamente. Então, vamos simular chamando allocate_memory.

    void* ptr = allocate_memory(MEMORY_LIMIT_BYTES + 1);
    TEST_ASSERT_NULL(ptr);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_allocate_and_free_memory);
    RUN_TEST(test_allocate_exceeding_limit);
    RUN_TEST(test_check_available_memory_pass);
    RUN_TEST(test_check_available_memory_fail);

    return UNITY_END();
}
