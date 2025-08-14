#include "parse-tree.h"
#include "memory/memory_controller.h"
#include <stdbool.h>
#include "reader/reader.h"
#include "lex/lex.h"

bool has_main_function_declared = false;

ParseTreeNode *ParseTree(Token *token_vector, int token_count) {
    Token current_token;
    for(int i=0; i < token_count; i++) {
        current_token = token_vector[i];
        if (current_token.type == TOKEN_MAIN) {
            if (has_main_function_declared) {
                fprintf(stderr, "[ERROR]: Função main já declarada.\n");
                return NULL; // Retorna erro                
            }
            has_main_function_declared = true;
        }
    }
}

void parse_main_function(Token *token_vector, int token_count, int current_token_index) {
    if (MAIN_FUNCTION)
}