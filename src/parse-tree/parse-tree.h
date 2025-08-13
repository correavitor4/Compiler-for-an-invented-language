#ifndef PARSE_TREE_H
#define PARSE_TREE_H
#include <stdbool.h>

#include "parse-node-types.h"

typedef struct {
    ParseTreeNodeType type; // Tipo do nó
    char *literal;
    struct ParseTreeNode **children; // Ponteiro para um array de filhos
    int child_count;
} ParseTreeNode;

typedef struct {
    ParseTree (*new)();
    bool (*map_tree)(ParseTree *tree, char *file_path);
    void (*destroy)(ParseTree *tree);
} ParseTree;

#endif