#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens/tokens.h"
#include "memory/memory_controller.h"

void setUp(void)
{
    // Executado antes de cada teste
}

void tearDown(void)
{
    // Executado depois de cada teste
}

#pragma region try_parse_data_type
void test_try_parse_data_type____with_valid_data_type____should_return_sucess()
{
    const char *data_types[] = {
        "inteiro",
        "decimal",
        "texto"};

    for (int i = 0; i < 3; i++)
    {
        Token *token = allocate_memory(sizeof(Token));
        TEST_ASSERT_NOT_NULL(token);

        int result = try_parse_data_type(data_types[i], token);
        TEST_ASSERT_EQUAL_INT(TRY_PARSE_DATA_TYPE_TOKEN_SUCCESS, result);
        TEST_ASSERT_EQUAL_INT(TOKEN_VAR_DATA_TYPE_INT + i, token->type);
        free_memory(token);
    }
}

void test_try_parse_data_type____with_invalid_data_type____should_return_not_found()
{
    const char *invalid_data_types[] = {
        "inteirox",
        "decimall",
        "textoo",
        "",
        " ",
        "xinteiro",
        "xdecimal",
        "xtexto"
        "_inteiro",
        "_decimal",
        "_texto"
        "__inteiro_",
        "__decimal_",
        "__texto_"
    };

    for (int i = 0; i < 5; i++)
    {
        Token *token = allocate_memory(sizeof(Token));
        TEST_ASSERT_NOT_NULL(token);

        int result = try_parse_data_type(invalid_data_types[i], token);
        TEST_ASSERT_EQUAL_INT(TRY_PARSE_DATA_TYPE_TOKEN_NOT_FOUND, result);
        free_memory(token);
    }

}

void test_try_parse_data_type____with_null_token____should_return_error()
{
    const char *data_type = "inteiro";

    int result = try_parse_data_type(data_type, NULL);
    TEST_ASSERT_EQUAL_INT(TRY_PARSE_DATA_TYPE_TOKEN_ERROR, result);
}

void test_try_parse_data_type____with_null_data_type____should_return_error()
{
    Token *token = allocate_memory(sizeof(Token));
    TEST_ASSERT_NOT_NULL(token);

    int result = try_parse_data_type(NULL, token);
    TEST_ASSERT_EQUAL_INT(TRY_PARSE_DATA_TYPE_TOKEN_ERROR, result);
    free_memory(token);
}   

#pragma endregion

#pragma region try_parse_functions
void test_try_parse_functions____with_valid_function____should_return_sucess()
{
    const char* functions[] = {
        "principal",
        "funcao"
    };
    
    for (int i = 0; i < 2; i++)
    {
        TokenType token_type;
        int result = try_parse_functions(functions[i], strlen(functions[i]), &token_type);
        TEST_ASSERT_EQUAL_INT(TRY_PARSE_FUNCTION_TOKEN_SUCCESS, result);
        TEST_ASSERT_EQUAL_INT(TOKEN_MAIN_FUNCTION + i, token_type);
    }
}

void test_try_parse_functions____with_invalid_function____should_return_not_found()
{
    const char* functions[] = {
        "funcaox",
        "principalx"
        "_funcao",
        "_principal",
        "__funcao_",
        "__principal_"
        "xprincipal",
        " ",
        ""
    };

    for (int i = 0; i < 2; i++)
    {
        TokenType token_type;
        int result = try_parse_functions(functions[i], strlen(functions[i]), &token_type);
        TEST_ASSERT_EQUAL_INT(TRY_PARSE_FUNCTION_TOKEN_NOT_FOUND, result);
    }
}

void test_try_parse_functions____with_null_token____should_return_error()
{
    int result = try_parse_functions("funcao", 6, NULL);
    TEST_ASSERT_EQUAL_INT(TRY_PARSE_FUNCTION_TOKEN_ERROR, result);
}

void test_try_parse_functions____with_null_function____should_return_error()
{
    TokenType token_type;
    int result = try_parse_functions(NULL, 0, &token_type);
    TEST_ASSERT_EQUAL_INT(TRY_PARSE_FUNCTION_TOKEN_ERROR, result);
}

#pragma endregion

#pragma region try_parse_console_ops
void test_try_parse_console_ops____with_valid_op____should_return_sucess()
{
    const char* functions[] = {
        "leia",
        "escreva"
    };
    
    for (int i = 0; i < 2; i++)
    {
        TokenType token_type;
        int result = try_parse_console_ops(functions[i], strlen(functions[i]), &token_type);
        TEST_ASSERT_EQUAL_INT(TRY_PARSE_CONSOLE_OPS_TOKEN_SUCCESS, result);
        TEST_ASSERT_EQUAL_INT(TOKEN_LEIA + i, token_type);
    }
}

void test_try_parse_console_ops____with_invalid_op____should_return_not_found()
{
    const char* functions[] = {
        "leiax",
        "escrevax"
        "_leia",
        "_escreva",
        "__leia_",
        "__escreva_"
        "xescreva",
        " ",
        ""
    };

    for (int i = 0; i < 2; i++)
    {
        TokenType token_type;
        int result = try_parse_console_ops(functions[i], strlen(functions[i]), &token_type);
        TEST_ASSERT_EQUAL_INT(TRY_PARSE_CONSOLE_OPS_TOKEN_NOT_FOUND, result);
    }
}

void test_try_parse_console_ops____with_null_token____should_return_error()
{
    int result = try_parse_console_ops("leia", 4, NULL);
    TEST_ASSERT_EQUAL_INT(TRY_PARSE_CONSOLE_OPS_TOKEN_ERROR, result);
}

void test_try_parse_console_ops____with_null_op____should_return_error()
{
    TokenType token_type;
    int result = try_parse_console_ops(NULL, 0, &token_type);
    TEST_ASSERT_EQUAL_INT(TRY_PARSE_CONSOLE_OPS_TOKEN_ERROR, result);
}

#pragma endregion

#pragma region try_parse_conditional
void test_try_parse_conditional____with_valid_token____should_return_sucess()
{
    const char* functions[] = {
        "se",
        "senao"
    };
    
    for (int i = 0; i < 2; i++)
    {
        TokenType token_type;
        int result = try_parse_conditional(functions[i], strlen(functions[i]), &token_type);
        TEST_ASSERT_EQUAL_INT(TRY_PARSE_CONDITIONAL_TOKEN_SUCCESS, result);
        TEST_ASSERT_EQUAL_INT(TOKEN_SE + i, token_type);
    }
}

void test_try_parse_conditional____with_invalid_token____should_return_not_found()
{
    const char* functions[] = {
        "se_",
        "senao_",
        "_se",
        "_senao",
        "__se_",
        "__senao_",
        "xse",
        " ",
        ""
    };
    
    for (int i = 0; i < 2; i++)
    {
        TokenType token_type;
        int result = try_parse_conditional(functions[i], strlen(functions[i]), &token_type);
        TEST_ASSERT_EQUAL_INT(TRY_PARSE_CONDITIONAL_TOKEN_NOT_FOUND, result);
    }
}

void test_try_parse_conditional____with_null_token____should_return_error()
{
    int result = try_parse_conditional("se", 2, NULL);
    TEST_ASSERT_EQUAL_INT(TRY_PARSE_CONDITIONAL_TOKEN_ERROR, result);
}

void test_try_parse_conditional____with_null_function____should_return_error()
{
    TokenType token_type;
    int result = try_parse_conditional(NULL, 0, &token_type);
    TEST_ASSERT_EQUAL_INT(TRY_PARSE_CONDITIONAL_TOKEN_ERROR, result);
}

#pragma endregion

int main(void)
{
    UNITY_BEGIN();

    // data type
    RUN_TEST(test_try_parse_data_type____with_valid_data_type____should_return_sucess);
    RUN_TEST(test_try_parse_data_type____with_invalid_data_type____should_return_not_found);
    RUN_TEST(test_try_parse_data_type____with_null_token____should_return_error);
    RUN_TEST(test_try_parse_data_type____with_null_data_type____should_return_error);
   

    // functions
    RUN_TEST(test_try_parse_functions____with_valid_function____should_return_sucess);
    RUN_TEST(test_try_parse_functions____with_invalid_function____should_return_not_found);
    RUN_TEST(test_try_parse_functions____with_null_token____should_return_error);
    RUN_TEST(test_try_parse_functions____with_null_function____should_return_error);
   

    // console ops
    RUN_TEST(test_try_parse_console_ops____with_valid_op____should_return_sucess);
    RUN_TEST(test_try_parse_console_ops____with_invalid_op____should_return_not_found);
    RUN_TEST(test_try_parse_console_ops____with_null_token____should_return_error);
    RUN_TEST(test_try_parse_console_ops____with_null_op____should_return_error);
    

    // conditional
    RUN_TEST(test_try_parse_conditional____with_valid_token____should_return_sucess);
    RUN_TEST(test_try_parse_conditional____with_invalid_token____should_return_not_found);
    RUN_TEST(test_try_parse_conditional____with_null_token____should_return_error);
    RUN_TEST(test_try_parse_conditional____with_null_function____should_return_error);

    return UNITY_END();
}