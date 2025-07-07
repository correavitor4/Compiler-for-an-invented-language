#include "tokens.h" 
#include "memory/memory_controller.h"
#include <cstdio>
#include <ctype.h>  // para isalpha, isdigit

//TODO: testar
int try_parse_token(char *token_string, int token_string_length, Token *token)
{
    TokenType *token_type = &token->type;

    // 1. Verifica se é um token de tipo de variável
    int result = try_parse_data_data(token_string, token_type);
    if (result == TRY_PARSE_DATA_TYPE_TOKEN_SUCCESS) {
        token->literal = allocate_memory(token_string_length + 1);
        if (token->literal == NULL) {
            fprintf(stderr, "Erro: falha ao alocar memória para o literal do token.\n");
            return TRY_PARSE_DATA_TYPE_TOKEN_ERROR;
        }
        memcpy(token->literal, token_string, token_string_length);
        token->literal[token_string_length] = '\0';
        return TRY_PARSE_DATA_TYPE_TOKEN_SUCCESS;
    }

    // 2. Verifica se é um token de função
    result = try_parse_functions(token_string, token_string_length, token_type);
    if (result == TRY_PARSE_FUNCTION_TOKEN_SUCCESS) {
        token->literal = allocate_memory(token_string_length + 1);
        if (token->literal == NULL) {
            fprintf(stderr, "Erro: falha ao alocar memória para o literal do token.\n");
            return TRY_PARSE_FUNCTION_TOKEN_ERROR;
        }
        memcpy(token->literal, token_string, token_string_length);
        token->literal[token_string_length] = '\0';
        return TRY_PARSE_FUNCTION_TOKEN_SUCCESS;
    }

    // 3. Verifica se é um token de operações de console
    result = try_parse_console_ops(token_string, token_string_length, token_type);
    if (result == TRY_PARSE_CONSOLE_OPS_TOKEN_SUCCESS) {
        token->literal = allocate_memory(token_string_length + 1);
        if (token->literal == NULL) {
            fprintf(stderr, "Erro: falha ao alocar memória para o literal do token.\n");
            return TRY_PARSE_CONSOLE_OPS_TOKEN_ERROR;
        }
        memcpy(token->literal, token_string, token_string_length);
        token->literal[token_string_length] = '\0';
        return TRY_PARSE_CONSOLE_OPS_TOKEN_SUCCESS;
    }

    // 4. Verifica se é um token de senão condicional
    result = try_parse_conditional(token_string, token_string_length, token_type);
    if (result == TRY_PARSE_CONDITIONAL_TOKEN_SUCCESS) {
        token->literal = allocate_memory(token_string_length + 1);
        if (token->literal == NULL) {
            fprintf(stderr, "Erro: falha ao alocar memória para o literal do token.\n");
            return TRY_PARSE_CONDITIONAL_TOKEN_ERROR;    
        }
        memcpy(token->literal, token_string, token_string_length);
        token->literal[token_string_length] = '\0';
        return TRY_PARSE_CONDITIONAL_TOKEN_SUCCESS;
    }

    // 5. Verifica se é um token de para
    result = try_parse_for_loop(token_string, token_string_length, token_type);
    if (result == TRY_PARSE_FOR_LOOP_TOKEN_SUCCESS) {
        token->literal = allocate_memory(token_string_length + 1);
        if (token->literal == NULL) {
            fprintf(stderr, "Erro: falha ao alocar memória para o literal do token.\n");
            return TRY_PARSE_FOR_LOOP_TOKEN_ERROR;
        }
        memcpy(token->literal, token_string, token_string_length);
        token->literal[token_string_length] = '\0';
        return TRY_PARSE_FOR_LOOP_TOKEN_SUCCESS;
    }

    // 6. Verifica se é um token de nome de função
    result = try_parse_function_name(token_string, token_string_length, token_type);
    if (result == TRY_PARSE_FUNCTION_NAME_TOKEN_SUCCESS) {
        token->literal = allocate_memory(token_string_length + 1);
        if (token->literal == NULL) {
            fprintf(stderr, "Erro: falha ao alocar memória para o literal do token.\n");
            return TRY_PARSE_FUNCTION_NAME_TOKEN_ERROR;
        }
        memcpy(token->literal, token_string, token_string_length);
        token->literal[token_string_length] = '\0';
        return TRY_PARSE_FUNCTION_NAME_TOKEN_SUCCESS;
    }

    // 7. Verifica se é um token de variável
    result = try_parse_variable(token_string, token_string_length, token_type);
    if (result == TRY_PARSE_VARIABLE_TOKEN_SUCCESS) {
        token->literal = allocate_memory(token_string_length + 1);
        if (token->literal == NULL) {
            fprintf(stderr, "Erro: falha ao alocar memória para o literal do token.\n");
            return TRY_PARSE_VARIABLE_TOKEN_ERROR;
        }
        memcpy(token->literal, token_string, token_string_length);
        token->literal[token_string_length] = '\0';
        return TRY_PARSE_VARIABLE_TOKEN_SUCCESS;
    }

    // 8. Verifica se é um token de special token
    result = try_parse_special_token(token_string, token_string_length, token_type);
    if (result == TRY_PARSE_SPECIAL_TOKEN_SUCCESS) {
        token->literal = allocate_memory(token_string_length + 1);
        if (token->literal == NULL) {
            fprintf(stderr, "Erro: falha ao alocar memória para o literal do token.\n");
            return TRY_PARSE_SPECIAL_TOKEN_ERROR;
        }
        memcpy(token->literal, token_string, token_string_length);
        token->literal[token_string_length] = '\0';
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }
    return TRY_PARSE_TOKEN_ERROR;
}


//TODO: testar
//TODO: corrigir comentários (tipo de variável -> tipo de dado)
int try_parse_data_type(char *variable, TokenType *token_type) {
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

//TODO: testar
int try_parse_special_token(char *token_string, int token_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_SPECIAL_TOKEN_ERROR;
    }
    if (token_string == NULL || token_string_length <= 0) {
        fprintf(stderr, "Erro: string de token inválida.\n");
        return TRY_PARSE_SPECIAL_TOKEN_ERROR;
    }
    #pragma endregion

    // 1. Verifica se é um parêntese aberto
    if (strncmp(token_string, "(", 1) == 0) {
        *token_type = TOKEN_PARENTESES_ABRIR;
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }

    // 2. Verifica se é um parêntese fechado
    if (strncmp(token_string, ")", 1) == 0) {
        *token_type = TOKEN_PARENTESES_FECHAR;
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }

    // 3. Verifica se é uma chave aberta
    if (strncmp(token_string, "{", 1) == 0) {
        *token_type = TOKEN_CHAVE_ABRIR;
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }

    // 4. Verifica se é uma chave fechada
    if (strncmp(token_string, "}", 1) == 0) {
        *token_type = TOKEN_CHAVE_FECHAR;
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }

    // 5. Verifica se é uma vírgula
    if (strncmp(token_string, ",", 1) == 0) {
        *token_type = TOKEN_VIRGULA;
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }

    // 6. Verifica se é um ponto e vírgula
    if (strncmp(token_string, ";", 1) == 0) {
        *token_type = TOKEN_PONTO_E_VIRGULA;
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }

    // 7. Verifica se é um colchete aberto
    if (strncmp(token_string, "[", 1) == 0) {
        *token_type = TOKEN_COCHETE_ABRIR;
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }

    // 8. Verifica se é um colchete fechado
    if (strncmp(token_string, "]", 1) == 0) {
        *token_type = TOKEN_COCHETE_FECHAR;
        return TRY_PARSE_SPECIAL_TOKEN_SUCCESS;
    }

    // 9. Se chegou aqui, token especial desconhecido
    return TRY_PARSE_SPECIAL_TOKEN_NOT_FOUND;
}

//TODO: testar
int try_parse_arithmetic_operator(char *operator_string, int operator_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_ERROR;
    }
    if (operator_string == NULL || operator_string_length <= 0) {
        fprintf(stderr, "Erro: string de operador inválida.\n");
        return TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_ERROR;
    }
    #pragma endregion

    // 1. Verifica se é um operador de adição
    if (strncmp(operator_string, "+", 1) == 0) {
        *token_type = TOKEN_OPERADOR_SOMA;
        return TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_SUCCESS;
    }

    // 2. Verifica se é um operador de subtração
    if (strncmp(operator_string, "-", 1) == 0) {
        *token_type = TOKEN_OPERADOR_SUBTRACAO;
        return TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_SUCCESS;
    }

    // 3. Verifica se é um operador de multiplicação
    if (strncmp(operator_string, "*", 1) == 0) {
        *token_type = TOKEN_OPERADOR_MULTIPLICACAO;
        return TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_SUCCESS;
    }

    // 4. Verifica se é um operador de divisão
    if (strncmp(operator_string, "/", 1) == 0) {
        *token_type = TOKEN_OPERADOR_DIVISAO;
        return TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_SUCCESS;
    }

    // 5. Verifica se é um operador de exponenciação
    if (strncmp(operator_string, "^", 1) == 0) {
        *token_type = TOKEN_OPERADOR_EXPONENCIACAO;
        return TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_SUCCESS;
    }

    // 6. Se chegou aqui, operador aritmético desconhecido
    return TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_NOT_FOUND;
}

TODO: testar
int try_parse_comparison_operator(char *operator_string, int operator_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_ERROR;
    }
    if (operator_string == NULL || operator_string_length <= 0) {
        fprintf(stderr, "Erro: string de operador inválida.\n");
        return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_ERROR;
    }
    #pragma endregion

    // 1. Verifica se é um operador de comparação de igualdade
    if (strncmp(operator_string, "==", 2) == 0) {
        *token_type = TOKEN_OPERADOR_COMPARACAO_IGUAL;
        return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_SUCCESS;
    }

    // 2. Verifica se é um operador de comparação de diferença
    if (strncmp(operator_string, "!=", 2) == 0) {
        *token_type = TOKEN_OPERADOR_COMPARACAO_DIFERENTE;
        return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_SUCCESS;
    }

    // 3. Verifica se é um operador de comparação maior que
    if (strncmp(operator_string, ">", 1) == 0) {
        *token_type = TOKEN_OPERADOR_COMPARACAO_MAIOR;
        return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_SUCCESS;
    }

    // 4. Verifica se é um operador de comparação maior ou igual a
    if (strncmp(operator_string, ">=", 2) == 0) {
        *token_type = TOKEN_OPERADOR_COMPARACAO_MAIOR_IGUAL;
        return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_SUCCESS;
    }

    // 5. Verifica se é um operador de comparação menor que
    if (strncmp(operator_string, "<", 1) == 0) {
        *token_type = TOKEN_OPERADOR_COMPARACAO_MENOR;
        return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_SUCCESS;
    }

    // 6. Verifica se é um operador de comparação menor ou igual a
    if (strncmp(operator_string, "<=", 2) == 0) {
        *token_type = TOKEN_OPERADOR_COMPARACAO_MENOR_IGUAL;
        return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_SUCCESS;
    }

    // 7. Se chegou aqui, operador de comparação desconhecido
    return TRY_PARSE_COMPARISON_OPERATOR_TOKEN_NOT_FOUND;
}

TODO: testar
int try_parse_logical_operator(char *operator_string, int operator_string_length, TokenType *token_type) {
    #pragma region programação defensiva
    if (token_type == NULL) {
        fprintf(stderr, "Erro: ponteiro token_type é NULL.\n");
        return TRY_PARSE_LOGICAL_OPERATOR_TOKEN_ERROR;
    }
    if (operator_string == NULL || operator_string_length <= 0) {
        fprintf(stderr, "Erro: string de operador lógico inválida.\n");
        return TRY_PARSE_LOGICAL_OPERATOR_TOKEN_ERROR;
    }
    #pragma endregion

    // 1. Verifica se é um operador lógico E
    if (strncmp(operator_string, "&&", 2) == 0) {
        *token_type = TOKEN_OPERADOR_LOGICO_E;
        return TRY_PARSE_LOGICAL_OPERATOR_TOKEN_SUCCESS;
    }

    // 2. Verifica se é um operador lógico OU
    if (strncmp(operator_string, "||", 2) == 0) {
        *token_type = TOKEN_OPERADOR_LOGICO_OU;
        return TRY_PARSE_LOGICAL_OPERATOR_TOKEN_SUCCESS;
    }

    // 3. Se chegou aqui, operador lógico desconhecido
    return TRY_PARSE_LOGICAL_OPERATOR_TOKEN_NOT_FOUND;
}

