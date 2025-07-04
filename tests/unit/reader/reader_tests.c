#include "unity.h"
#include "../../../reader/reader.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../../memory/memory_controller.h"
#include <string.h>

// Helpers para criar arquivos temporários para teste
static const char *test_file_name = "test_file.txt";

static void create_test_file(const char *content)
{
    FILE *f = fopen(test_file_name, "w");
    TEST_ASSERT_NOT_NULL(f);
    fputs(content, f);
    fclose(f);
}

static void delete_test_file()
{
    remove(test_file_name);
}

void setUp(void)
{
    // Executado antes de cada teste
}

void tearDown(void)
{
    // Executado depois de cada teste
    delete_test_file();
}

// Testa criação e destruição do FileReader
void test_filereader_create_destroy(void)
{
    create_test_file("Linha 1\nLinha 2\n");

    FileReader *fr = filereader_create(test_file_name);
    TEST_ASSERT_NOT_NULL(fr);
    TEST_ASSERT_NOT_NULL(fr->file);
    TEST_ASSERT_NOT_NULL(fr->buffer);
    TEST_ASSERT_EQUAL_UINT(INITIAL_BUFFER_SIZE, fr->buffer_size);
    filereader_destroy(fr);
}

// Testa leitura linha a linha
void test_filereader_read_lines(void)
{
    create_test_file("Linha 1\nLinha 2\nLinha 3");

    FileReader *fr = filereader_create(test_file_name);
    TEST_ASSERT_NOT_NULL(fr);

    char *line = filereader_get_next_line(fr);
    TEST_ASSERT_NOT_NULL(line);
    TEST_ASSERT_EQUAL_STRING("Linha 1\n", line);

    line = filereader_get_next_line(fr);
    TEST_ASSERT_NOT_NULL(line);
    TEST_ASSERT_EQUAL_STRING("Linha 2\n", line);

    line = filereader_get_next_line(fr);
    TEST_ASSERT_NOT_NULL(line);
    TEST_ASSERT_EQUAL_STRING("Linha 3", line);

    // Fim do arquivo: deve retornar NULL
    line = filereader_get_next_line(fr);
    TEST_ASSERT_NULL(line);

    // Verifica uso de memória após leitura

    filereader_destroy(fr);
}

// Testa reset do buffer
void test_reset_buffer_size_to_initial(void)
{
    create_test_file("Linha 1\n");

    FileReader *fr = filereader_create(test_file_name);
    TEST_ASSERT_NOT_NULL(fr);

    // Força buffer maior para testar reset
    char *new_buf = reallocate_memory(fr->buffer, INITIAL_BUFFER_SIZE * 4);
    TEST_ASSERT_NOT_NULL(new_buf);
    fr->buffer = new_buf;
    fr->buffer_size = INITIAL_BUFFER_SIZE * 4;

    int res = reset_buffer_size_to_initial(fr);
    TEST_ASSERT_EQUAL_INT(0, res);
    TEST_ASSERT_EQUAL_UINT(INITIAL_BUFFER_SIZE, fr->buffer_size);

    filereader_destroy(fr);
}

// Testa comportamento com arquivo inexistente
void test_filereader_create_nonexistent_file(void)
{

    FileReader *fr = filereader_create("arquivo_inexistente.txt");
    TEST_ASSERT_NULL(fr);
}

// Testa leitura de arquivo vazio
void test_filereader_read_empty_file(void)
{
    create_test_file("");
    FileReader *fr = filereader_create(test_file_name);
    TEST_ASSERT_NOT_NULL(fr);

    char *line = filereader_get_next_line(fr);
    TEST_ASSERT_NULL(line);

    filereader_destroy(fr);
}

void test_create_with_invalid_filename(void)
{
    FileReader *fr = filereader_create(NULL);
    TEST_ASSERT_NULL(fr);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_filereader_create_destroy);
    RUN_TEST(test_filereader_read_lines);
    RUN_TEST(test_reset_buffer_size_to_initial);
    RUN_TEST(test_filereader_create_nonexistent_file);
    RUN_TEST(test_filereader_read_empty_file);
    RUN_TEST(test_create_with_invalid_filename);

    return UNITY_END();
}
