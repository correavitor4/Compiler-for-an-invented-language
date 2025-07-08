#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

#pragma region mock try_parse_data_type
bool try_parse_data_type_should_return_success = false;
bool try_parse_data_type_should_return_error = false;
bool try_parse_data_type_should_return_not_found = true;

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

int try_parse_data_type(char *variable, TokenType *token_type)
{
}

int main(void)
{
    UNITY_BEGIN();

    return UNITY_END();
}