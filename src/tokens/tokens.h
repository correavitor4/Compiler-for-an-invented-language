#ifndef TOKENS_H
#define TOKENS_H

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

} TokenType;

#endif