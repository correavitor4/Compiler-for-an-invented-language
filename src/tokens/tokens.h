#ifndef TOKENS_H
#define TOKENS_H

<<<<<<< Updated upstream
typedef enum {
    // --- Special Tokens ---
    TOKEN_EOF,          // End of File marker
    TOKEN_ILLEGAL,      // For unrecognized characters/symbols
    TOKEN_NEWLINE,       // Newline character
    TOKEN_WHITESPACE,   // Espaços em branco

    // --- Identifiers & Literals ---
    TOKEN_IDENT,        // Identificador de variável ou função
    TOKEN_INTEIRO,      // 1, 25, 100
    TOKEN_DECIMAL,      // 3.14, 0.5

    // --- Operators ---
    TOKEN_ASSIGN,       // =
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_ASTERISK,     // *
    TOKEN_SLASH,        // /
    TOKEN_EQ,           // ==
    TOKEN_NOT_EQ,       // !=

    // --- Delimiters ---
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_COMMA,        // ,
    TOKEN_SEMICOLON,    // ;

    // --- Keywords ---
    TOKEN_SE,           // se
    TOKEN_SENAO,        // senao

    // --- Misc ---
    TOKEN_COMMENT,      // Comentários
    TOKEN_STRING,       // "string literal"

    // --- Invalid ---
    TOKEN_INVALID       // Para tokens ou sequências inválidas
=======
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
>>>>>>> Stashed changes

} TokenType;

#endif