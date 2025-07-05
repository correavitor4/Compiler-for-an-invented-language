#define my_fgets_mocked my_fgets_mocked_mocked

#include "unity.h"
#include <string.h>  // Inclua para strncpy
#include "config/config.h"

#undef my_fgets_mocked  // Desfaz a definição para evitar conflitos

static const char* mock_input = NULL;

char* my_fgets_mocked(char* str, int num, FILE* stream) {
    (void)stream; // evita warning unused parameter
    if (!mock_input) return NULL;
    strncpy(str, mock_input, num - 1);
    str[num - 1] = '\0';
    return str;
}

void setUp(void) {
    mock_input = NULL;
}

void tearDown(void) {
    mock_input = NULL;
}

void test_read_directory_path_default(void) {
    mock_input = "\n"; // Simula Enter (string vazia)
    read_directory_path();
    TEST_ASSERT_EQUAL_STRING("files/", get_directory_path());
}

void test_read_directory_path_custom(void) {
    const char* custom_path = "custom_directory/";
    set_directory_path(custom_path);  // Define valor inicial
    mock_input = "another_directory/\n"; // Simula entrada do usuário
    read_directory_path();
    TEST_ASSERT_EQUAL_STRING("another_directory/", get_directory_path());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_read_directory_path_default);
    RUN_TEST(test_read_directory_path_custom);
    return UNITY_END();
}
