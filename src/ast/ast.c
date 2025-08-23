#include "ast.h"
#include "memory/memory_controller.h"

ASTNode *ast_generate_root()
{
    ASTNode *root = allocate_memory(sizeof(ASTNode));
    root->type = AST_ROOT_NODE;
    root->literal = NULL;
    root->children = NULL;
    root->parent = NULL;
    root->child_count = 0;
    root->line_num = 0;
    return root;
}

ASTNode* ast_add_child(ASTNode *parent, ASTNodeType type, char *literal, int line_num)
{
    if (parent == NULL)
        return NULL;

    ASTNode *child = allocate_memory(sizeof(ASTNode));
    child->type = type;
    child->literal = literal;
    child->children = NULL;
    child->parent = parent;
    child->child_count = 0;
    child->line_num = line_num;

    parent->children = (ASTNode **)reallocate_memory(parent->children, sizeof(ASTNode *) * (parent->child_count + 1));
    parent->children[parent->child_count] = child;
    parent->child_count++;

    return child;
}