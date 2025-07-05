#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reader/reader.h"
#include "memory/memory_controller.h"

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
void test_file_reader_create_destroy(void)
{
    create_test_file("Linha 1\nLinha 2\n");

    FileReader *fr = file_reader_create(test_file_name);
    TEST_ASSERT_NOT_NULL(fr);
    TEST_ASSERT_NOT_NULL(fr->file);
    TEST_ASSERT_NOT_NULL(fr->buffer);
    TEST_ASSERT_EQUAL_UINT(INITIAL_BUFFER_SIZE, fr->buffer_size);
    file_reader_destroy(fr);
}

// Testa leitura linha a linha
void test_file_reader_read_lines(void)
{
    create_test_file("Linha 1\nLinha 2\nLinha 3");

    FileReader *fr = file_reader_create(test_file_name);
    TEST_ASSERT_NOT_NULL(fr);

    char *line = file_reader_get_next_line(fr);
    TEST_ASSERT_NOT_NULL(line);
    TEST_ASSERT_EQUAL_STRING("Linha 1\n", line);

    line = file_reader_get_next_line(fr);
    TEST_ASSERT_NOT_NULL(line);
    TEST_ASSERT_EQUAL_STRING("Linha 2\n", line);

    line = file_reader_get_next_line(fr);
    TEST_ASSERT_NOT_NULL(line);
    TEST_ASSERT_EQUAL_STRING("Linha 3", line);

    // Fim do arquivo: deve retornar NULL
    line = file_reader_get_next_line(fr);
    TEST_ASSERT_NULL(line);

    // Verifica uso de memória após leitura

    file_reader_destroy(fr);
}

// Testa comportamento com arquivo inexistente
void test_file_reader_create_nonexistent_file(void)
{

    FileReader *fr = file_reader_create("arquivo_inexistente.txt");
    TEST_ASSERT_NULL(fr);
}

// Testa leitura de arquivo vazio
void test_file_reader_read_empty_file(void)
{
    create_test_file("");
    FileReader *fr = file_reader_create(test_file_name);
    TEST_ASSERT_NOT_NULL(fr);

    char *line = file_reader_get_next_line(fr);
    TEST_ASSERT_NULL(line);

    file_reader_destroy(fr);
}

void test_create_with_invalid_filename(void)
{
    FileReader *fr = file_reader_create(NULL);
    TEST_ASSERT_NULL(fr);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_file_reader_create_destroy);
    RUN_TEST(test_file_reader_read_lines);
    RUN_TEST(test_file_reader_create_nonexistent_file);
    RUN_TEST(test_file_reader_read_empty_file);
    RUN_TEST(test_create_with_invalid_filename);

    return UNITY_END();
}
