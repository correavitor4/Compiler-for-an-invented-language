#include "ast-node-types.h"
#include "lex/lex.h"
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
ASTNode* ast_add_existing_child_copy(ASTNode *parent, ASTNode *child);
ASTNode* generate_temporary_node(char *literal, int line_num);
ASTNode* pop_child(ASTNode *parent);
ASTNode* delete_node(ASTNode *node);
void *ast_destroy(ASTNode *root);

#endif