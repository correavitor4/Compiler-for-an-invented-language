#ifndef TOKENS_H
#define TOKENS_H

typedef enum {


} TokenType;

typedef struct {
    TokenType type;      // Tipo do token
    char *literal;       // String literal do token
    int line;            // Linha onde o token foi encontrado
} Token;

#endif