#include "parse-tree.h"
#include "memory/memory_controller.h"
#include <stdbool.h>
#include "reader/reader.h"
#include "lex/lex.h"
#include "tokens/tokens.h"

const int PARSE_FALSE = 1;
const int PARSE_TRUE = 0;
const int PARSE_ERROR = -1;

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

ParseTreeNode* create_parse_tree_node(ParseTreeNodeType type, char *literal, int line_num) {
    ParseTreeNode *node = (ParseTreeNode *)allocate_memory(sizeof(ParseTreeNode));
    node->type = type;
    node->literal = literal;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

int parse_main_function(Token *token_vector, int token_count, int current_token_index) {
    if (token_vector[current_token_index].type != TOKEN_MAIN) return;

    if (has_main_function_declared) {
        fprintf(stderr, "[ERROR]: Função main já declarada.\n");
        return PARSE_ERROR; // Retorna erro               
    }

    ParseTreeNode *main_function_node = create_parse_tree_node(PARSE_TREE_NODE_MAIN_FUNCTION, token_vector[current_token_index].literal, token_vector[current_token_index].line_num);
    has_main_function_declared = true;

    // Next allowable tokens after main
    ParseTreeNodeType next_allowable_tokens[] = {
        TOKEN_LPAREN,
    };
}