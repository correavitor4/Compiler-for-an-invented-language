#include "tokens.h" 
#include "memory/memory_controller.h"
#include <cstdio>
#include <ctype.h>  // para isalpha, isdigit

#pragma region reserved keywords definition
#pragma endregion

//TODO: testar
int try_parse_token(char *token_string, int token_string_length, Token *token)
{

}

//TODO: testar
int try_parse_variable(char *variable_string, int variable_string_length, Token *token) {
    int pos = 0;
}

//TODO: testar
//TODO: corrigir comentários (tipo de variável -> tipo de dado)
int try_parse_data_data(char *variable, TokenType *token_type) {
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_DATA_TYPE_TOKEN_ERROR;
    }

    // 1 Veririca se é um token de declaração de tipo de variável "interio"
    if (strncmp(variable, "inteiro", 7) == 0) {
        *token_type = TOKEN_VAR_TYPE_DECLARATION_INT;
        return TRY_PARSE_DATA_TYPE_TOKEN_SUCCESS;
    }

    // 2 Verifica se é um token de declaração de tipo de variável "decimal"
    if (strncmp(variable, "decimal", 7) == 0) {
        *token_type = TOKEN_VAR_TYPE_DECLARATION_DECIMAL;
        return TRY_PARSE_DATA_TYPE_TOKEN_SUCCESS;
    }

    // 3 Verifica se é um token de declaração de tipo de variável "texto"
    if (strncmp(variable, "texto", 5) == 0) {
        *token_type = TOKEN_VAR_TYPE_DECLARATION_TEXT;
        return TRY_PARSE_DATA_TYPE_TOKEN_SUCCESS;
    }

    // 4 Se não for nenhum dos tipos de variável, retorna o código de não encontrado
    return TRY_PARSE_DATA_TYPE_TOKEN_NOT_FOUND
}

//TODO: testar
int try_parse_functions(char *function_string, int function_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_FUNCTION_TOKEN_ERROR;
    }

    if (function_string == NULL || function_string_length <= 0) {
        fprintf(stderr, "Erro: string de função inválida.\n");
        return TRY_PARSE_FUNCTION_TOKEN_ERROR;
    }
    #pragma endregion

    // 1 Verifica se é um token de função
    if (strncmp(function_string, "funcao", 6) == 0) {
        *token_type = TOKEN_FUNCTION;
        return TRY_PARSE_FUNCTION_TOKEN_SUCCESS;
    }

    // 2 Verifica se é um token de função principal
    else if (strncmp(function_string, "funcao_principal", 16) == 0) {
        *token_type = TOKEN_MAIN_FUNCTION;
        return TRY_PARSE_FUNCTION_TOKEN_SUCCESS;
    }

    // 3 Se não for nenhum dos tipos de função, retorna o código de não encontrado
    return TRY_PARSE_FUNCTION_TOKEN_NOT_FOUND;
}

//TODO: testar
int try_parse_console_ops(char *console_string, int console_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_CONSOLE_OPS_TOKEN_ERROR;
    }

    if (console_string == NULL || console_string_length <= 0) {
        fprintf(stderr, "Erro: string de função inválida.\n");
        return TRY_PARSE_CONSOLE_OPS_TOKEN_ERROR;
    }
    #pragma endregion

    // 1 Verifica se é um token de leitura
    if (strncmp(console_string, "leia", 4) == 0) {
        *token_type = TOKEN_LEIA;
        return TRY_PARSE_CONSOLE_OPS_TOKEN_SUCCESS;
    }

    // 2 Verifica se é um token de escrita
    else if (strncmp(console_string, "escreva", 7) == 0) {
        *token_type = TOKEN_ESCREVA;
        return TRY_PARSE_CONSOLE_OPS_TOKEN_SUCCESS;
    }

    // 3 Se não for nenhum dos tipos de função, retorna o código de não encontrado
    return TRY_PARSE_CONSOLE_OPS_TOKEN_NOT_FOUND;
}

//TODO: testar
int try_parse_conditional(char *conditional_string, int conditional_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_CONDITIONAL_TOKEN_ERROR;
    }

    if (conditional_string == NULL || conditional_string_length <= 0) {
        fprintf(stderr, "Erro: string de função inválida.\n");
        return TRY_PARSE_CONDITIONAL_TOKEN_ERROR;
    }
    #pragma endregion

    // 1 Verifica se é um token de condição
    if (strncmp(conditional_string, "se", 2) == 0) {
        *token_type = TOKEN_CONDITIONAL;
        return TRY_PARSE_CONDITIONAL_TOKEN_SUCCESS;
    }

    
    if (strncmp(conditional_string, "senao", 5) == 0) {
        *token_type = TOKEN_ELSE;
        return TRY_PARSE_CONDITIONAL_TOKEN_SUCCESS;
    }

    // 2 Se não for nenhum dos tipos de condição, retorna o código de não encontrado
    return TRY_PARSE_CONDITIONAL_TOKEN_NOT_FOUND;
}

//TODO: testar
int try_parse_para(char *para_string, int para_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_FOR_LOOP_TOKEN_ERROR;
    }
    if (para_string == NULL || para_string_length <= 0) {
        fprintf(stderr, "Erro: string de função inválida.\n");
        return TRY_PARSE_FOR_LOOP_TOKEN_ERROR;
    }
    #pragma endregion

    // 1 Verifica se é um token de laço "para"
    if (strncmp(para_string, "para", 4) == 0) {
        *token_type = TOKEN_FOR_LOOP;
        return TRY_PARSE_FOR_LOOP_TOKEN_SUCCESS;
    }

    // 2 Se não for nenhum dos tipos de laço, retorna o código de não encontrado
    return TRY_PARSE_FOR_LOOP_TOKEN_NOT_FOUND;
}

/TODO: testar
int try_parse_function_name(char *function_name_string, int function_name_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_FUNCTION_NAME_TOKEN_ERROR;
    }
    if (function_name_string == NULL || function_name_string_length <= 0) {
        fprintf(stderr, "Erro: string de nome de função inválida.\n");
        return TRY_PARSE_FUNCTION_NAME_TOKEN_ERROR;
    }
    #pragma endregion
    
    // 1 Verifica se é um token de nome de função   

}

//TODO: testar
int try_parse_function_name(char *function_name_string, int function_name_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_FUNCTION_NAME_TOKEN_ERROR;
    }
    if (function_name_string == NULL || function_name_string_length <= 0) {
        fprintf(stderr, "Erro: string de nome de função inválida.\n");
        return TRY_PARSE_FUNCTION_NAME_TOKEN_ERROR;
    }
    #pragma endregion

    // 1. Verifica se começa com "__"
    if (function_name_length < 3) { // mínimo "__x"
        return TRY_PARSE_FUNCTION_NAME_TOKEN_NOT_FOUND;
    }
    if (!(function_name_string[0] == '_' && function_name_string[1] == '_')) {
        return TRY_PARSE_FUNCTION_NAME_TOKEN_NOT_FOUND;
    }

    // 2. Verifica o primeiro caractere após "__"
    char first_char = function_name_string[2];
    if (!( (first_char >= 'a' && first_char <= 'z') || 
           (first_char >= 'A' && first_char <= 'Z') ||
           (first_char >= '0' && first_char <= '9') )) {
        return TRY_PARSE_FUNCTION_NAME_TOKEN_NOT_FOUND;
    }

    // 3. Verifica os caracteres seguintes (se houver)
    int i;
    for (i = 3; i < function_name_length; i++) {
        char c = function_name_string[i];
        if (!((c >= 'a' && c <= 'z') || 
              (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9'))) {
            // Caractere inválido no nome da função
            return TRY_PARSE_FUNCTION_NAME_TOKEN_NOT_FOUND;
        }
    }

    // Se chegou aqui, nome válido
    *token_type = TOKEN_NOME_FUNCAO;
    return TRY_PARSE_FUNCTION_NAME_TOKEN_SUCCESS;
}

//TODO: testar
int try_parse_variable(char *variable_string, int variable_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token == NULL) {
        fprintf(stderr, "Erro: ponteiro token é NULL.\n");
        return TRY_PARSE_VARIABLE_TOKEN_ERROR;
    }
    if (variable_string == NULL || variable_string_length <= 0) {
        fprintf(stderr, "Erro: string de variável inválida.\n");
        return TRY_PARSE_VARIABLE_TOKEN_ERROR;
    }
    #pragma endregion

    // 1. Verifica se começa com '!'
    if (variable_string[0] != '!') {
        return TRY_PARSE_VARIABLE_TOKEN_NOT_FOUND;
    }

    // 2. Verifica se tem pelo menos 2 caracteres (ex: "!a")
    if (variable_string_length < 2) {
        return TRY_PARSE_VARIABLE_TOKEN_NOT_FOUND;
    }

    // 3. Verifica se o primeiro caractere após '!' é letra minúscula (a-z)
    char first_char = variable_string[1];
    if (!(first_char >= 'a' && first_char <= 'z')) {
        return TRY_PARSE_VARIABLE_TOKEN_NOT_FOUND;
    }

    // 4. Verifica os caracteres seguintes (se houver)
    int i;
    for (i = 2; i < variable_string_length; i++) {
        char c = variable_string[i];
        if (!((c >= 'a' && c <= 'z') || 
              (c >= 'A' && c <= 'Z') ||
              (c >= '0' && c <= '9'))) {
            // Caractere inválido no nome da variável
            return TRY_PARSE_VARIABLE_TOKEN_NOT_FOUND;
        }
    }

    // 5. Se chegou aqui, nome válido
    *token_type = TOKEN_VARIABLE_NAME; // Ajuste conforme sua enumeração
    // Copie o nome para o token, se necessário
    // strncpy(token->value, variable_string, variable_string_length);
    // token->value[variable_string_length] = '\0';

    return TRY_PARSE_VARIABLE_TOKEN_SUCCESS;
}
