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
    size_t initial_used = get_memory_usage();

    void* ptr = allocate_memory(1024); // 1 KB
    TEST_ASSERT_NOT_NULL(ptr);

    size_t used_after_alloc = get_memory_usage();
    TEST_ASSERT_TRUE(used_after_alloc > initial_used);

    free_memory(ptr);

    size_t used_after_free = get_memory_usage();
    TEST_ASSERT_EQUAL_UINT(initial_used, used_after_free);
}

// Testa limite de memória
void test_allocate_exceeding_limit(void) {
    void* ptr = allocate_memory(MEMORY_LIMIT_BYTES + 1);
    TEST_ASSERT_NULL(ptr);
}

// Testa check_available_memory com tamanho válido
void test_check_available_memory_pass(void) {
    check_available_memory(1024);
    TEST_PASS_MESSAGE("check_available_memory passed for valid size");
}

// Testa check_available_memory com tamanho inválido (excede limite)
void test_check_available_memory_fail(void) {
    void* ptr = allocate_memory(MEMORY_LIMIT_BYTES + 1);
    TEST_ASSERT_NULL(ptr);
}

// --- TESTES PARA reallocate_memory ---

// Testa realocação aumentando o tamanho
void test_reallocate_memory_increase(void) {
    size_t initial_used = get_memory_usage();

    void* ptr = allocate_memory(512);
    TEST_ASSERT_NOT_NULL(ptr);

    // Preenche com dados para garantir que ponteiro é válido
    memset(ptr, 0xAA, 512);

    void* new_ptr = reallocate_memory(ptr, 1024);
    TEST_ASSERT_NOT_NULL(new_ptr);

    // O conteúdo antigo deve estar preservado (pelo menos os primeiros 512 bytes)
    TEST_ASSERT_EQUAL_UINT8(0xAA, ((unsigned char*)new_ptr)[0]);

    size_t used_after_realloc = get_memory_usage();
    TEST_ASSERT_TRUE(used_after_realloc > initial_used);

    free_memory(new_ptr);

    size_t used_after_free = get_memory_usage();
    TEST_ASSERT_EQUAL_UINT(initial_used, used_after_free);
}

// Testa realocação reduzindo o tamanho
void test_reallocate_memory_decrease(void) {
    size_t initial_used = get_memory_usage();

    void* ptr = allocate_memory(1024);
    TEST_ASSERT_NOT_NULL(ptr);

    void* new_ptr = reallocate_memory(ptr, 512);
    TEST_ASSERT_NOT_NULL(new_ptr);

    TEST_ASSERT_EQUAL_UINT(initial_used + sizeof(MemHeader) + 512, get_memory_usage());

    free_memory(new_ptr);

    size_t used_after_free = get_memory_usage();
    TEST_ASSERT_EQUAL_UINT(initial_used, used_after_free);
}

// Testa realocação com ponteiro NULL (deve alocar novo bloco)
void test_reallocate_memory_null_ptr(void) {
    size_t initial_used = get_memory_usage();

    void* ptr = reallocate_memory(NULL, 256);
    TEST_ASSERT_NOT_NULL(ptr);

    size_t used_after_alloc = get_memory_usage();
    TEST_ASSERT_TRUE(used_after_alloc > initial_used);

    free_memory(ptr);

    size_t used_after_free = get_memory_usage();
    TEST_ASSERT_EQUAL_UINT(initial_used, used_after_free);
}

// Testa realocação que excede limite de memória
void test_reallocate_memory_exceed_limit(void) {
    void* ptr = allocate_memory(1024);
    TEST_ASSERT_NOT_NULL(ptr);

    // Tenta realocar para tamanho maior que o limite total
    void* new_ptr = reallocate_memory(ptr, MEMORY_LIMIT_BYTES + 1);
    TEST_ASSERT_NULL(new_ptr);

    // Libera o original
    free_memory(ptr);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_allocate_and_free_memory);
    RUN_TEST(test_allocate_exceeding_limit);
    RUN_TEST(test_check_available_memory_pass);
    RUN_TEST(test_check_available_memory_fail);

    RUN_TEST(test_reallocate_memory_increase);
    RUN_TEST(test_reallocate_memory_decrease);
    RUN_TEST(test_reallocate_memory_null_ptr);
    RUN_TEST(test_reallocate_memory_exceed_limit);

    return UNITY_END();
}
