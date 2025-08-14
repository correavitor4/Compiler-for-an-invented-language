#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "parse-node-types.h"

typedef struct
{
    ParseTreeNodeType type; // Tipo do nó
    char *literal;
    struct ParseTreeNode **children; // Ponteiro para um array de filhos
    int child_count;
    int line_num;
} ParseTreeNode;

ParseTreeNode *ParseTree(Token *token_vector, int token_count);
#endif