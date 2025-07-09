#ifndef TOKENS_H
#define TOKENS_H

typedef enum
{
    // --- Tokens de Controle ---
    TOKEN_ILLEGAL, // Token/caractere desconhecido
    TOKEN_TEXT_INVALID, // Texto inválido (ex: "texto sem fechamento)
    TOKEN_EOL,     // End of Line (Fim de Linha)

    // --- Identificadores e Literais (Valores) ---
    TOKEN_IDENT_FUNC, // Iniciada por __ (ex: __minhaFuncao)
    TOKEN_IDENT_VAR,  // Iniciada por ! (ex: !minhaVariavel)
    TOKEN_INT,        // Números inteiros (ex: 10, 50, 123) - inteiro
    TOKEN_DECIMAL,    // Números de ponto flutuante (ex: 10.5, 0.25) - decimal
    TOKEN_TEXT,       // Strings de texto (ex: "ola mundo") - texto

    // --- Operadores ---
    TOKEN_ASSIGN,   // = (Atribuição)
    TOKEN_PLUS,     // + (Soma)
    TOKEN_MINUS,    // - (Subtração)
    TOKEN_ASTERISK, // * (Multiplicação)
    TOKEN_SLASH,    // / (Divisão)

    // --- Operadores Lógicos e de Comparação ---
    TOKEN_LT, // < (Menor que)
    TOKEN_GT, // > (Maior que)

    TOKEN_EQ,     // == (Igual a)
    TOKEN_NOT_EQ, // <> (Diferente de)
    TOKEN_LTE,    // <= (Menor ou igual a)
    TOKEN_GTE,    // >= (Maior ou igual a)

    TOKEN_AND, // && (E Lógico)
    TOKEN_OR,  // || (OU Lógico)

    // --- Delimitadores ---
    TOKEN_COMMA,     // , (Vírgula)
    TOKEN_SEMICOLON, // ; (Ponto e vírgula)

    TOKEN_LPAREN,   // ( (Parêntese esquerdo)
    TOKEN_RPAREN,   // ) (Parêntese direito)
    TOKEN_LBRACE,   // { (Chave esquerda)
    TOKEN_RBRACE,   // } (Chave direita)
    TOKEN_LBRACKET, // [ (Colchete esquerdo)
    TOKEN_RBRACKET, // ] (Colchete direito)

    // --- Palavras-chave da Linguagem ---
    TOKEN_MAIN,     // "principal"
    TOKEN_FUNCTION, // "funcao"
    TOKEN_READ,     // "leia"
    TOKEN_PRINT,    // "escreva"
    TOKEN_IF,       // "se"
    TOKEN_ELSE,     // "senao"
    TOKEN_FOR,      // "para"

} TokenType;

#pragma region return codes
//try parse (general)
#define TRY_PARSE_IS_A_TOKEN_SUCCESS 0
#define TRY_PARSE_IS_A_NOT_TOKEN 1
#define TRY_PARSE_IS_A_TOKEN_ERROR -1

// try parse data type
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

// try parse special token
#define TRY_PARSE_SPECIAL_TOKEN_SUCCESS 0
#define TRY_PARSE_SPECIAL_TOKEN_NOT_FOUND 1
#define TRY_PARSE_SPECIAL_TOKEN_ERROR -1

// try parse arithmetic operator
#define TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_SUCCESS 0
#define TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_NOT_FOUND 1
#define TRY_PARSE_ARITHMETIC_OPERATOR_TOKEN_ERROR -1

// try parse comparison operator
#define TRY_PARSE_COMPARISON_OPERATOR_TOKEN_SUCCESS 0
#define TRY_PARSE_COMPARISON_OPERATOR_TOKEN_NOT_FOUND 1
#define TRY_PARSE_COMPARISON_OPERATOR_TOKEN_ERROR -1

// try parse logical operator
#define TRY_PARSE_LOGICAL_OPERATOR_TOKEN_SUCCESS 0
#define TRY_PARSE_LOGICAL_OPERATOR_TOKEN_NOT_FOUND 1
#define TRY_PARSE_LOGICAL_OPERATOR_TOKEN_ERROR -1

#pragma endregion

#endif