#ifndef TOKENS_H
#define TOKENS_H

#pragma region "Error Codes"

#define TOKEN_PARSE_ERROR_NULL_POINTER -1
#define TOKEN_PARSE_VAR_INVALID_VARIABLE_NAME -2

#pragma endregion

#pragma region "Result Codes"

#define TOKEN_PARSE_SUCCESS 0
#define TOKEN_PARSE_NOT_A_VAR 1

#pragma endregion



#pragma region "Reserved Keywords"
// variable declaration types
#define VAR_INT_DECLARATION "inteiro"
#define VAR_DECIMAL_DECLARATION "decimal"
#define VAR_STRING_DECLARATION "texto"

#pragma endregion


typedef enum
{
    VAR_TOKEN,
} TokenType;

typedef struct
{
    TokenType type;
    char *lexeme;      // texto original
    int lexeme_length; // tamanho do lexeme
    int line;          // para mensagens de erro
} Token;


int try_parse_token(char *token_string, int token_string_length, Token *token);
#endif