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

/**
 * @brief Adds a new child node to the given parent node.
 *
 * This function will allocate enough space to store the new child node, and
 * will add it to the end of the list of children of the parent node.
 *
 * @param parent The parent node to add the new child node to.
 * @param child The new child node to be added.
 * @return The new child node that was added, or NULL if the parent or child is NULL.
 */
ASTNode *ast_add_existing_child_copy(ASTNode *parent, ASTNode *child)
{
    if (parent == NULL || child == NULL)
        return NULL;

   ASTNode *child_copy = allocate_memory(sizeof(ASTNode));
    child_copy->type = child->type;
    child_copy->literal = child->literal;
    child_copy->children = NULL;
    child_copy->parent = parent;
    child_copy->child_count = 0;
    child_copy->line_num = child->line_num;

    for (int i = 0; i < child->child_count; i++)
    {
        ast_add_existing_child_copy(child_copy, child->children[i]);
    }

    parent->children = (ASTNode **)reallocate_memory(parent->children, sizeof(ASTNode *) * (parent->child_count + 1));
    parent->children[parent->child_count] = child_copy;
    parent->child_count++;
    child_copy->parent = parent; // Update the parent pointer of the copied child
    
    return child;
}

ASTNode* generate_temporary_node(char *literal, int line_num)
{
    ASTNode *temp_node = allocate_memory(sizeof(ASTNode));
    temp_node->type = AST_ASSIGNMENT_TEMPORARY_NODE;
    temp_node->literal = literal;
    temp_node->children = NULL;
    temp_node->parent = NULL;
    temp_node->child_count = 0;
    temp_node->line_num = line_num;
    return temp_node;
}

/**
 * @brief Pops the last child node from the given parent node.
 *
 * This function decrements the child count of the given parent node and
 * returns the last child node that was previously stored in the parent's
 * children array. If the parent is NULL or has no children, this function
 * returns NULL.
 *
 * @param parent Pointer to the parent node from which the last child node
 *               should be popped.
 * @return Pointer to the popped child node, or NULL if the parent has no
 *         children.
 */
ASTNode* pop_child(ASTNode *parent)
{
    if (parent == NULL || parent->child_count == 0)
        return NULL;

    ASTNode *child = parent->children[parent->child_count - 1];
    parent->child_count--;
    parent->children = (ASTNode **)reallocate_memory(parent->children, sizeof(ASTNode *) * parent->child_count);

    return child;
}

ASTNode *delete_node(ASTNode *node)
{
    if (node == NULL)
        return NULL;

    // Recursively delete all child nodes
    for (int i = 0; i < node->child_count; i++)
    {
        delete_node(node->children[i]);
    }

    // Free the children array
    free_memory(node->children);

    // Free the node itself
    free_memory(node);

    return NULL;
}