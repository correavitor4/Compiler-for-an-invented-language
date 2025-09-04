#include "./semantic.h"

/* Prototypes */
TokenType navigate_ast(ASTNode *node, ScopeManager *sm);
void validate_function_declaration(ASTNode *func_node, ScopeManager *sm, Symbol *func_symbol);
char *token_type_to_string(TokenType type);
TokenType get_expression_type(ASTNode *node, ScopeManager *sm);

char *token_type_to_string(TokenType type)
{
    switch (type)
    {
    case TOKEN_INT_TYPE:
        return "INTEIRO";
    case TOKEN_DEC_TYPE:
        return "DECIMAL";
    case TOKEN_TEXT_TYPE:
        return "TEXTO";
    case TOKEN_VOID_TYPE:
        return "VAZIO";
    default:
        return "DESCONHECIDO";
    }
}

TokenType navigate_ast(ASTNode *node, ScopeManager *sm)
{
    if (node == NULL)
        return TOKEN_ILLEGAL;

    switch (node->type)
    {
    case AST_MAIN_NODE:
    {
        scope_manager_enter_scope(sm, "main");
        for (int i = 0; i < node->child_count; i++)
        {
            navigate_ast(node->children[i], sm);
        }
        scope_manager_exit_scope(sm);
        break;
    }
    case AST_EXPRESSION_NODE:
    {
        get_expression_type(node, sm);

        break;
    }
    case AST_FUNCTION_DECLARATION_NODE:
    {
        Symbol *symbol = scope_manager_lookup(sm, node->literal);
        if (symbol)
        {
            scope_manager_enter_scope(sm, node->literal);
            validate_function_declaration(node, sm, symbol);
        }
        else
        {
            fprintf(stderr, "[ERROR|Semantic]: Função '%s' não declarada na linha %d.\n", node->literal, node->line_num);
        }

        for (int i = 0; i < node->child_count; i++)
        {
            navigate_ast(node->children[i], sm);
        }

        scope_manager_exit_scope(sm);

        break;
    }
    case AST_VARIABLE_DECLARATION_NODE:
    {
        Symbol *symbol = scope_manager_lookup(sm, node->literal);
        if (symbol)
        {
            if (node->child_count > 0)

            {
                if (node->children[0]->type == AST_ASSIGNMENT_NODE)
                {
                    if (symbol->type != get_expression_type(node->children[0]->children[0], sm))
                    {
                        fprintf(stderr, "[WARN|Semantic]: Atribuição de valor incompatível para a variável '%s' do tipo '%s' na linha %d.\n", node->literal, token_type_to_string(symbol->type), node->line_num);
                    }
                }
            }
        }
        break;
    }
    default:
        for (int i = 0; i < node->child_count; i++)
        {
            navigate_ast(node->children[i], sm);
        }
        break;
    }
}

TokenType get_expression_type(ASTNode *node, ScopeManager *sm)
{
    if (node == NULL)
        return TOKEN_ILLEGAL;

    switch (node->type)
    {
    case AST_INT_LITERAL_NODE:
    {
        return TOKEN_INT_TYPE;
        break;
    }
    case AST_DECIMAL_LITERAL_NODE:
    {
        return TOKEN_DEC_TYPE;
        break;
    }
    case AST_TEXT_LITERAL_NODE:
    {
        return TOKEN_TEXT_TYPE;
        break;
    }
    case AST_VARIABLE_REF_NODE:
    {
        Symbol *symbol = scope_manager_lookup(sm, node->literal);
        if (symbol)
        {
            return symbol->type;
        }
        else
        {
            fprintf(stderr, "[ERROR|Semantic]: Variável '%s' não declarada na linha %d.\n", node->literal, node->line_num);
        }
        break;
    }
    case AST_FUNCTION_CALL_NODE:
    {
        Symbol *symbol = scope_manager_lookup(sm, node->literal);
        if (symbol)
        {
            return symbol->type;
        }
        else
        {
            fprintf(stderr, "[ERROR|Semantic]: Função '%s' não declarada na linha %d.\n", node->literal, node->line_num);
        }

        break;
    }
    case AST_EXPRESSION_NODE:
    {
        if (node->child_count >= 2)
        {
            ASTNode *left = node->children[0];
            ASTNode *operator = node->children[1];
            ASTNode *right = node->children[2];

            TokenType left_type = get_expression_type(left, sm);
            TokenType right_type = get_expression_type(right, sm);

            if (left_type != right_type)
            {
                fprintf(stderr, "[WARN|Semantic]: Tipos incompatíveis na expressão '%s %s %s' na linha %d.\n", left->literal, operator->literal, right->type != AST_EXPRESSION_NODE ? right->literal : "...", node->line_num);
            }
        }
        else
        {
            fprintf(stderr, "[WARN|Semantic]: Expressão '%s' incorreta na linha %d.\n", node->literal, node->line_num);
        }
        break;
    }
    case AST_GROUP_NODE:
    {
        if (node->child_count == 1)
        {
            return get_expression_type(node->children[0], sm);
        }
        else
        {
            fprintf(stderr, "[WARN|Semantic]: Grupo de expressão incorreto na linha %d.\n", node->line_num);
        }
        break;
    }
    default:
        break;
    }
}

void validate_function_declaration(ASTNode *func_node, ScopeManager *sm, Symbol *func_symbol)
{

    if (func_node == NULL || func_symbol == NULL)
        return;

    ASTNode *func_block = func_node->children[func_node->child_count - 1];

    if (func_block == NULL || func_block->type != AST_BLOCK_NODE)
    {
        fprintf(stderr, "[WARN|Semantic]: Função '%s' deve ter um bloco de código a partir da linha %d.\n", func_node->literal, func_node->line_num);
        return;
    }

    ASTNode *func_return = func_block->children[func_block->child_count - 1];
    if ((func_return == NULL && func_return->type != AST_FUNCTION_RETURN_NODE) && func_symbol->type != TOKEN_VOID_TYPE)
    {
        fprintf(stderr, "[WARN|Semantic]: Função '%s' deve ter uma instrução de retorno a partir da linha %d.\n", func_node->literal, func_node->line_num);
        return;
    }

    switch (func_return->children[0]->type)
    {
    case AST_INT_LITERAL_NODE:
        func_symbol->type = TOKEN_INT_TYPE;
        break;
    case AST_DECIMAL_LITERAL_NODE:
        func_symbol->type = TOKEN_DEC_TYPE;
        break;
    case AST_TEXT_LITERAL_NODE:
        func_symbol->type = TOKEN_TEXT_TYPE;
        break;
    case AST_VARIABLE_REF_NODE:
    {
        // Verifica o tipo da variável referenciada
        // e compara com o tipo de retorno da função
        Symbol *var_symbol = scope_manager_lookup(sm, func_return->children[0]->literal);
        func_symbol->type = var_symbol ? var_symbol->type : TOKEN_ILLEGAL;
        break;
    }
    case AST_EXPRESSION_NODE:
    {
        // Avalia a expressão e determina seu tipo
        func_symbol->type = get_expression_type(func_return->children[0], sm);
        break;
    }
    default:
        break;
    }
}

short validate_ast(ASTNode *root, ScopeManager *sm)
{
    if (root == NULL || sm == NULL)
    {
        fprintf(stderr, "[ERROR|Compiler]: AST ou ScopeManager nulo.\n");
        return 0;
    }

    navigate_ast(root, sm);

    return 1;
}