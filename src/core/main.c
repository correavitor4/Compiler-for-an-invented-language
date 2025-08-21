#include <stdio.h>
#include <stdlib.h>
// #include "../config/options.h"

#include "hash/hash.h"
#include "symbol_table/symbol_table.h"
#include "memory/memory_controller.h"
#include "reader/reader.h"
#include "lex/lex.h"
#include "parser/parser.h"

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

            printf("Token { Linha: %lu, Tipo: %d, Literal: \"%s\", Posição: %lu }\n", tok.line_num, tok.type, tok.literal, tok.position);

            // Armazena o token no vetor
            tokens_count++;
            tokens_vector = reallocate_memory(tokens_vector, sizeof(Token) * tokens_count);
            if (!tokens_vector) {
                fprintf(stderr, "Erro ao alocar memória para tokens.\n");
                free_memory(tok.literal);
                LEXER.destroy(lexer);
                file_reader_destroy(fr);
                return 1;
            }
            tokens_vector[tokens_count - 1] = tok;

            TokenType type = tok.type;

            if (type == TOKEN_EOL) {
                break;
            }
        }

        printf("Fim da linha %lu\n\n", line_counter);

        file_reader_free_line(line);

        line_counter++;
    }

    // Após lido, adiciona o token EOF (End of File)
    tokens_count++;

    tokens_vector = reallocate_memory(tokens_vector, sizeof(Token) * tokens_count);
    if (!tokens_vector) {
        fprintf(stderr, "Erro ao realocar memória para o token EOF.\n");
        return 1;
    }

    Token eof_token = {
        .type = TOKEN_EOF,
        .literal = "EOF",
        .line_num = line_counter,
        .position = 0
    };

    // Armazena o token EOF na última posição do vetor.
    tokens_vector[tokens_count - 1] = eof_token;

    // 5. (Opcional) Imprime o token EOF para confirmar que foi adicionado.
    printf("Token { Linha: %lu, Tipo: %d, Literal: \"%s\", Posição: %lu }\n", eof_token.line_num, eof_token.type, eof_token.literal, eof_token.position);
    printf("Fim da tokenização. Total de tokens: %d\n\n", tokens_count);

    LEXER.destroy(lexer);
    file_reader_destroy(fr);

    ScopeManager *sm = scope_manager_create();
    if (!sm) {
        fprintf(stderr, "Erro fatal: não foi possível criar o gestor de escopos.\n");
        return 1;
    }

    scope_manager_insert(sm, "leia", KIND_FUNC, TOKEN_READ);
    scope_manager_insert(sm, "escreva", KIND_FUNC, TOKEN_PRINT);
    scope_manager_insert(sm, "principal", KIND_FUNC, TOKEN_MAIN);

    printf("--- Iniciando Análise Sintática ---\n");

    if (parse_source_code(tokens_vector, tokens_count, sm) == 0) {
        printf("Análise sintática concluída com sucesso!\n");
    } else {
        // A função do parser já imprime o erro detalhado e encerra,
        // mas este 'else' estaria aqui para outros casos.
        fprintf(stderr, "Falha na análise sintática.\n");
    }


    // Syntax analysis
    printf("Análise sintática:\n");
    

    return 0;
}