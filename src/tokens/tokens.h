#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    TOKEN_VAR_DATA_TYPE_INT,
    TOKEN_VAR_DATA_TYPE_DECIMAL,
    TOKEN_VAR_DATA_TYPE_TEXT,
    TOKEN_MAIN_FUNCTION,
    TOKEN_FUNCTION,
    TOKEN_LEIA,
    TOKEN_ESCREVA,
    TOKEN_SE,
    TOKEN_SENAO,
    TOKEN_PARA,
    TOKEN_NOME_FUNCAO,
    TOKEN_VARIABLE_NAME,
} TokenType;

typedef struct {
    TokenType type;      // Tipo do token
    char *literal;       // String literal do token
    int line;            // Linha onde o token foi encontrado
} Token;

#pragma region return codes
// try parse r
#define TRY_PARSE_DATA_TYPE_TOKEN_SUCCESS 0
#define TRY_PARSE_DATA_TYPE_TOKEN_NOT_FOUND 1
#define TRY_PARSE_DATA_TYPE_TOKEN_ERROR -1

// try parse function
#define TRY_PARSE_FUNCTION_TOKEN_SUCCESS 0
#define TRY_PARSE_FUNCTION_TOKEN_NOT_FOUND 1
#define TRY_PARSE_FUNCTION_TOKEN_ERROR -1

// try parse console operations
#define TRY_PARSE_CONSOLE_OPS_TOKEN_SUCCESS 0
#define TRY_PARSE_CONSOLE_OPS_TOKEN_NOT_FOUND 1
#define TRY_PARSE_CONSOLE_OPS_TOKEN_ERROR -1

// try parse conditional
#define TRY_PARSE_CONDITIONAL_TOKEN_SUCCESS 0
#define TRY_PARSE_CONDITIONAL_TOKEN_NOT_FOUND 1
#define TRY_PARSE_CONDITIONAL_TOKEN_ERROR -1

// try parse for loop
#define TRY_PARSE_FOR_LOOP_TOKEN_SUCCESS 0
#define TRY_PARSE_FOR_LOOP_TOKEN_NOT_FOUND 1
#define TRY_PARSE_FOR_LOOP_TOKEN_ERROR -1

//try parse function name
#define TRY_PARSE_FUNCTION_NAME_TOKEN_SUCCESS 0
#define TRY_PARSE_FUNCTION_NAME_TOKEN_NOT_FOUND 1
#define TRY_PARSE_FUNCTION_NAME_TOKEN_ERROR -1

// try parse variable
#define TRY_PARSE_VARIABLE_TOKEN_SUCCESS 0
#define TRY_PARSE_VARIABLE_TOKEN_NOT_FOUND 1
#define TRY_PARSE_VARIABLE_TOKEN_ERROR -1

#pragma endregion

#endif