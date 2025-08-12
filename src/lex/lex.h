
#ifndef LEX_H
#define LEX_H

#include "tokens/tokens.h"

typedef struct
{
    TokenType type; // Tipo do token
    char *literal;  // String literal do token
    int line_num;   // Linha onde o token foi encontrado
} Token;

typedef struct
{
    const char *current_line; // Linha atual do texto sendo analisado
    int line_num;             // Número da linha atual
    int position;             // Posição atual no texto
    int read_position;        // Posição de leitura atual
    char ch;                  // Caractere atual sendo analisado
} Lexer;

typedef struct
{
    Token (*next_token)(Lexer *l);                               // Função para obter o próximo token
    Lexer *(*new)(const char *input);                            // Função para criar um novo Lexer
    void (*load_line)(Lexer *l, const char *line, int line_num); // Função para carregar uma linha no Lexer
    void (*destroy)(Lexer *l);                                   // Função para destruir o Lexer
} LEXER_API;

extern const LEXER_API LEXER;

#endif // LEX_H