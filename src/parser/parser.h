// parser/parser.h

#ifndef PARSER_H
#define PARSER_H

#include "lex/lex.h"
#include "memory/memory_controller.h"
#include "symbol_table/symbol_table.h"

typedef struct Parser Parser;

/**
 * @brief Executa a análise sintática em um vetor de tokens.
 * * @param tokens O vetor de tokens gerado pelo lexer.
 * @param token_count O número total de tokens no vetor.
 * @return int Retorna 0 em caso de sucesso e um valor diferente de 0 em caso de erro.
 */
int parse_source_code(Token *tokens, int token_count, ScopeManager *sm);

#endif // PARSER_H