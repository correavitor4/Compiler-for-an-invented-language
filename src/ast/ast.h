#include "ast-node-types.h"
#ifndef AST_H
#define AST_H

typedef struct ASTNode
{
    ASTNodeType type;
    char *literal;
    struct ASTNode **children; // Ponteiro para um array de filhos
    struct ASTNode *parent;
    int child_count;
    int line_num;
} ASTNode;

ASTNode *ast_generate_root();
ASTNode * ast_add_child(ASTNode *parent, ASTNodeType type, char *literal, int line_num);

#endif