#include "./semantic.h"

void navigate_ast(ASTNode* node, ScopeManager* sm) {
    if (node == NULL) return;

    switch (node->type) {
        case AST_FUNCTION_DECLARATION_NODE: {
            const char* func_name = node->literal;
            scope_manager_enter_scope(sm, func_name);
            break;
        }
        case AST_ASSIGNMENT_NODE: {
            Symbol* symbol = scope_manager_lookup(sm, node->literal);
            if (symbol) {
                //
            }
            break;
        }
        default:
            break;
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

short validate_ast(ASTNode* root, ScopeManager* sm) {
    if (root == NULL || sm == NULL) {
        fprintf(stderr, "Erro: AST ou ScopeManager nulo.\n");
        return 0;
    }

    navigate_ast(root, sm);

    return 1;
}