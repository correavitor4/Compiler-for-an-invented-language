#include <stdio.h>
#include <stdlib.h>
// #include "../config/options.h"

#include "hash/hash.h"
#include "memory/memory_controller.h"
#include "reader/reader.h"
#include "lex/lex.h"

int main(int argc, char *argv[])
{

    if (argc < 2) {
        fprintf(stderr, "Uso: %s <caminho_do_arquivo>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];

    FileReader *fr = file_reader_create(file_path);

    if (!fr)
    {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", file_path);
        return 2;
    }

    printf("Reading file: %s\n", file_path);

    // Lê linha a linha
    char *line;

    // Inicializa o lexer
    unsigned long line_counter = 0;

    // Vetor para armazenar tokens
    Token *tokens_vector = allocate_memory(sizeof(Token) *0);
    if (!tokens_vector) {
        fprintf(stderr, "Erro ao alocar memória para tokens.\n");
        return 1;
    }

    int tokens_count = 0;

    Lexer *lexer = LEXER.new(fr->buffer);
    while ((line = file_reader_get_next_line(fr)) != NULL)
    {
        printf("Lendo linha: %s\n", line);

        // Carrega a linha no lexer
        LEXER.load_line(lexer, line, line_counter);

        // Processa a linha até o final
        for (;;) {
            Token tok = LEXER.next_token(lexer);

            if (tok.type == TOKEN_TEXT_INVALID) {
                fprintf(stderr, "[ERROR]: Texto sem fechamento na linha %lu:%d\n", line_counter, lexer->position);
                free_memory(tok.literal);
                LEXER.destroy(lexer);
                file_reader_destroy(fr);
                return 1; // Retorna erro
            }
            if (tok.type == TOKEN_ILLEGAL) {
                fprintf(stderr, "[ERROR]: Caractere ilegal '%s' na linha %lu:%d\n", tok.literal, line_counter, lexer->position);
                free_memory(tok.literal);
                LEXER.destroy(lexer);
                file_reader_destroy(fr);
                return 1; // Retorna erro
            }

            printf("Token { Linha: %d, Tipo: %d, Literal: \"%s\" }\n", tok.line_num, tok.type, tok.literal);
            
            // Armazena o token no vetor
            tokens_count++;
            tokens_vector = reallocate_memory(tokens_vector, sizeof(Token) * tokens_count);
            if (!tokens_vector) {
                fprintf(stderr, "Erro ao alocar memória para tokens.\n");
                free_memory(tok.literal);
                LEXER.destroy(lexer);
                file_reader_destroy(fr);
                return 1; // Retorna erro
            }
            tokens_vector[tokens_count - 1] = tok;

            TokenType type = tok.type;

            if (type == TOKEN_EOL) {
                break; // Passa para a próxima linha
            }
        }

        printf("Fim da linha %lu\n\n", line_counter);

        file_reader_free_line(line); // Libera a linha lida

        line_counter++;
    }

    LEXER.destroy(lexer);
    file_reader_destroy(fr);

    return 0;
}