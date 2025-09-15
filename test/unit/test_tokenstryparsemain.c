#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokens/tokens.h"
#include "memory/memory_controller.h"

bool try_parse_data_type_should_return_success = false;
bool try_parse_data_type_should_return_error = false;
bool try_parse_data_type_should_return_not_found = true;

void setUp(void)
{
    // Set all flag to default values
    try_parse_data_type_should_return_success = false;
    try_parse_data_type_should_return_error = false;
    try_parse_data_type_should_return_not_found = true;
}

void tearDown(void)
{
    // Executado depois de cada teste
}

#pragma region mocks
int try_parse_data_type(char *variable, TokenType *token_type)
{
    if (try_parse_data_type_should_return_not_found)
    {
        return TRY_PARSE_DATA_TYPE_TOKEN_NOT_FOUND;
    }
    else if (try_parse_data_type_should_return_error)
    {
        return TRY_PARSE_DATA_TYPE_TOKEN_ERROR;
    }
    else if (try_parse_data_type_should_return_success)
    {
        *token_type = TOKEN_VAR_DATA_TYPE_INT;
        return TRY_PARSE_DATA_TYPE_TOKEN_SUCCESS;
    }
}
#pragma endregion

#pragma region tests
void test_try_parse_data_type_should_return_success(void)
{
    try_parse_data_type_should_return_success = true;
    try_parse_data_type_should_return_not_found = false;
    try_parse_data_type_should_return_error = false;

    Token *tokenobj = allocate_memory(sizeof(Token));
    TEST_ASSERT_NOT_NULL(tokenobj);

    int result = try_parse_token("inteiro", strlen("inteiro"), *tokenobj);
    TEST_ASSERT_EQUAL(TRY_PARSE_IS_A_TOKEN_SUCCESS, result);
}

void test_try_parse_data_type_should_return_error(void)
{
    try_parse_data_type_should_return_error = true;
    
    Token *tokenobj = allocate_memory(sizeof(Token));
    TEST_ASSERT_NOT_NULL(tokenobj);

    int result = try_parse_token("inteiro", strlen("inteiro"), *tokenobj);
    TEST_ASSERT_EQUAL(TRY_PARSE_DATA_TYPE_TOKEN_ERROR, result);
}

#pragma endregion


int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_try_parse_data_type_should_return_success);
    RUN_TEST(test_try_parse_data_type_should_return_error);

    return UNITY_END();
}