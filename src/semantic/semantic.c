#include "./semantic.h"

void navigate_ast(ASTNode* node, ScopeManager* sm) {
    if (node == NULL) return;

    switch (node->type) {
        case AST_FUNCTION_DECLARATION_NODE: {
            const char* func_name = node->literal;
            scope_manager_enter_scope(sm);
            break;
        }
        case AST_ASSIGNMENT_NODE: {
            // Step 1: Get the declared type of the variable on the left
            Symbol* symbol = scope_manager_lookup(sm, node->literal);
            if (symbol) {
                //
            }
            break;
        }
        default:
            break;
    }

    for (int i = 0; i < node->child_count; i++) {
        navigate_ast(node->children[i], sm);
    }

    switch (node->type) {
        case AST_FUNCTION_DECLARATION_NODE:
        case AST_BLOCK_NODE:
        case AST_IF_NODE:
        case AST_FOR_NODE:
            scope_manager_exit_scope(sm);
            break;
        default:
            break;
    }
}