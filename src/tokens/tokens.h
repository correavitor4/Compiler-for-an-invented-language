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

#endif