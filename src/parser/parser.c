#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "symbol_table/symbol_table.h"
#include "utils/string/string_utils.h"

ASTNode *ast_root_node = NULL;

int expression_counter = 0;

/* PROTOTYPES */
static void parse_function_call(Parser *p, ASTNode *parent);
static void parse_statement(Parser *p, ASTNode *parent);
static void parse_expression(Parser *p, ASTNode *parent);
static void parse_group(Parser *p, ASTNode *parent);

/**
 * @brief Retrieves the current token from the parser.
 *
 * This function returns the token that the parser is currently processing.
 *
 * @param p Pointer to the Parser instance.
 * @return The current Token being processed.
 */
static Token current_token(Parser *p)
{
    if (p->current_pos < p->token_count)
        return p->tokens[p->current_pos];

    return p->tokens[p->token_count - 1];
}

/**
 * @brief Advances the parser to the next token in the input stream.
 *
 * This function updates the internal state of the parser to point to the next token,
 * typically used during parsing to consume tokens sequentially.
 *
 * @param p Pointer to the Parser structure whose token stream is being advanced.
 */
static void advance_token(Parser *p)
{
    if (p->current_pos < p->token_count)
    {
        p->current_pos++;
    }
}

static Token peek_token(Parser *p)
{
    if (p->current_pos + 1 < p->token_count)
        return p->tokens[p->current_pos + 1];

    return p->tokens[p->token_count - 1];
}

/**
 * @brief Expects the next token in the parser to be of a specific type.
 *
 * Checks if the current token in the parser matches the expected token type.
 * If the token does not match, reports an error using the provided error message.
 *
 * @param p Pointer to the Parser instance.
 * @param type The expected TokenType.
 * @param error_msg Error message to display if the token does not match.
 */
static void expect_token(Parser *p, TokenType type, const char *error_msg)
{
    Token tok = current_token(p);
    if (tok.type == type)
    {
        advance_token(p);
    }
    else
    {
        fprintf(stderr, "[ERROR|Syntax] Linha %lu: %s. Esperado token do tipo %d, mas encontrado %d ('%s').\n",
                tok.line_num, error_msg, type, tok.type, tok.literal);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Skips end-of-line (EOL) characters in the parser input.
 *
 * Advances the parser position past any consecutive EOL characters,
 * allowing parsing to continue at the next non-EOL character.
 *
 * @param p Pointer to the Parser structure.
 */
static void skip_eols(Parser *p)
{
    while (current_token(p).type == TOKEN_EOL)
    {
        advance_token(p);
    }
}

/**
 * @brief Parses a block of code using the given parser.
 *
 * This function processes a block structure in the source code,
 * updating the parser state as necessary. It is intended for internal
 * use within the parser implementation.
 *
 * @param p Pointer to the Parser instance used for parsing.
 */
static void parse_block(Parser *p, ASTNode *parent)
{
    ASTNode *block_node = ast_add_child(parent, AST_BLOCK_NODE, "block", current_token(p).line_num);
    expect_token(p, TOKEN_LBRACE, "Esperado '{' para iniciar um bloco");
    skip_eols(p);
    while (current_token(p).type != TOKEN_RBRACE && current_token(p).type != TOKEN_EOF)
    {
        parse_statement(p, block_node);
        skip_eols(p);
    }
    expect_token(p, TOKEN_RBRACE, "Esperado '}' para fechar um bloco");
}

static void parse_term(Parser *p, ASTNode *parent)
{
    Token tok = current_token(p);
    switch (tok.type)
    {
    case TOKEN_INT:
        advance_token(p);
        ast_add_child(parent, AST_INT_LITERAL_NODE, tok.literal, tok.line_num);
        break;
    case TOKEN_DECIMAL:
        advance_token(p);
        ast_add_child(parent, AST_DECIMAL_LITERAL_NODE, tok.literal, tok.line_num);
        break;

    case TOKEN_TEXT:
        ast_add_child(parent, AST_TEXT_LITERAL_NODE, tok.literal, tok.line_num);
        advance_token(p);
        break;
    case TOKEN_IDENT_VAR:
        if (tok.type == TOKEN_IDENT_VAR)
        {
            if (scope_manager_lookup(p->sm, tok.literal) == NULL)
            {
                fprintf(stderr, "[ERROR|Syntax] Linha %lu: A variável '%s' não foi declarada.\n",
                        tok.line_num, tok.literal);
                exit(EXIT_FAILURE);
            }

            ast_add_child(parent, AST_VARIABLE_REF_NODE, tok.literal, tok.line_num);
        }
        advance_token(p);
        break;
    case TOKEN_IDENT_FUNC:
        parse_function_call(p, parent);
        break;
    case TOKEN_LPAREN:
        advance_token(p);
        parse_group(p, parent);
        expect_token(p, TOKEN_RPAREN, "Esperado ')' para fechar a expressão");
        break;
    default:
        fprintf(stderr, "[ERROR|Syntax]: Linha %lu: Token inesperado '%s' numa expressão. Esperado um valor, variável ou chamada de função.\n",
                tok.line_num, tok.literal);
        exit(EXIT_FAILURE);
    }
}

static void parse_group(Parser *p, ASTNode *parent)
{
    ASTNode *group_node = ast_add_child(parent, AST_GROUP_NODE, "group", current_token(p).line_num);
    parse_expression(p, group_node);
}

static void parse_expression(Parser *p, ASTNode *parent)
{
    ASTNode *expr_node = ast_add_child(parent, AST_EXPRESSION_NODE, "expression", current_token(p).line_num);
    parse_term(p, expr_node);

    ASTNode *expr_node_itr = expr_node;

    while (current_token(p).type == TOKEN_PLUS || current_token(p).type == TOKEN_MINUS ||
           current_token(p).type == TOKEN_ASTERISK || current_token(p).type == TOKEN_SLASH ||
           current_token(p).type == TOKEN_LT || current_token(p).type == TOKEN_GT ||
           current_token(p).type == TOKEN_EQ || current_token(p).type == TOKEN_NOT_EQ ||
           current_token(p).type == TOKEN_LTE || current_token(p).type == TOKEN_GTE ||
           current_token(p).type == TOKEN_AND || current_token(p).type == TOKEN_OR ||
           current_token(p).type == TOKEN_ASSIGN)
    {
        ast_add_child(expr_node_itr, AST_OP_NODE, current_token(p).literal, current_token(p).line_num);
        
        expr_node_itr = ast_add_child(expr_node_itr, AST_EXPRESSION_NODE, "expression", current_token(p).line_num);
        advance_token(p);
        parse_term(p, expr_node_itr);
    }

    // If the expression has only one child, simplify the tree
    if (expr_node->child_count == 1 && expr_node->type == AST_EXPRESSION_NODE)
    {
        ASTNode *single_child = expr_node->children[0];
        // Remove expr_node from parent
        parent->child_count--;
        parent->children = (ASTNode **)reallocate_memory(parent->children, sizeof(ASTNode *) * parent->child_count);
        // Add single_child directly to parent
        ast_add_existing_child_copy(parent, single_child);
        free_memory(single_child->children);
        free_memory(single_child);

    }

    if ((void*)expr_node != (void*)expr_node_itr && expr_node_itr->type == AST_EXPRESSION_NODE && expr_node_itr->children[0]->child_count == 0)
    {

        ASTNode *single_child = expr_node_itr->children[0];
        ASTNode *parent = expr_node_itr->parent;

        parent->child_count--;
        parent->children = (ASTNode **)reallocate_memory(parent->children, sizeof(ASTNode *) * parent->child_count);
        // Add single_child directly to parent
        ast_add_existing_child_copy(parent, single_child);
        // Free the now unnecessary expr_node

        free_memory(single_child->children);
        free_memory(single_child);
        free_memory(expr_node_itr->children);
        free_memory(expr_node_itr);
    }

    // Free memory only after working with expr_node_itr
    if (expr_node->child_count == 1)
    {
        free_memory(expr_node->children);
        free_memory(expr_node);
    }
}

/**
 * @brief Parses a list of arguments in the current parser context.
 *
 * This function processes the argument list according to the parser's
 * current state, updating internal structures as needed.
 *
 * @param p Pointer to the Parser instance.
 */
static void parse_argument_list(Parser *p, ASTNode *parent)
{
    expect_token(p, TOKEN_LPAREN, "Esperado '(' antes da lista de argumentos");
    if (current_token(p).type != TOKEN_RPAREN)
    {
        parse_expression(p, parent);
        while (current_token(p).type == TOKEN_COMMA)
        {
            advance_token(p);
            parse_expression(p, parent);
        }
    }
    expect_token(p, TOKEN_RPAREN, "Esperado ')' após a lista de argumentos");
}

static void parse_parameter_list(Parser *p, Token func_token, ASTNode *parent)
{
    expect_token(p, TOKEN_LPAREN, "Esperado '(' para a lista de parâmetros da função");

    Param *param_list_head = NULL;

    if (current_token(p).type != TOKEN_RPAREN)
    {

        Param *param_list_tail = NULL;

        while (1)
        {
            int token_type = current_token(p).type;

            if (token_type != TOKEN_INT_TYPE && token_type != TOKEN_TEXT_TYPE && token_type != TOKEN_DEC_TYPE)
            {
                fprintf(stderr, "[ERROR|Syntax]: Linha %lu: Tipo de dado inválido para parâmetro. Esperado 'inteiro', 'decimal' ou 'texto'.\n",
                        current_token(p).line_num);
                exit(EXIT_FAILURE);
            }
            advance_token(p);

            Token param_token = current_token(p);
            expect_token(p, TOKEN_IDENT_VAR, "Esperado um nome de parâmetro (variável)");

            ast_add_child(parent, AST_VARIABLE_DECLARATION_NODE, param_token.literal, param_token.line_num);

            if (!scope_manager_insert(p->sm, param_token.literal, KIND_VAR, token_type))
            {
                fprintf(stderr, "[ERROR|Semantic]: Linha %lu: Parâmetro '%s' redeclarado.\n", param_token.line_num, param_token.literal);
                exit(EXIT_FAILURE);
            }

            Symbol *param_symbol = scope_manager_lookup(p->sm, param_token.literal);
            if (param_symbol)
            {
                Param *new_param = allocate_memory(sizeof(Param));
                new_param->symbol = param_symbol;
                new_param->next = NULL;

                if (param_list_head == NULL)
                {
                    // If the list is empty, this is the first and only node
                    param_list_head = new_param;
                    param_list_tail = new_param;
                }
                else
                {
                    // Otherwise, append to the end of the list and update the tail
                    param_list_tail->next = new_param;
                    param_list_tail = new_param;
                }
            }
            if (current_token(p).type != TOKEN_COMMA)
                break;

            advance_token(p);
        }

        Symbol *func_symbol = scope_manager_lookup(p->sm, func_token.literal);
        if (func_symbol)
        {
            bind_function_params(func_symbol, param_list_head);
        }
    }
    expect_token(p, TOKEN_RPAREN, "Esperado ')' para fechar a lista de parâmetros");
}

static void parse_decimal_specifier(Parser *p)
{
    expect_token(p, TOKEN_LBRACKET, "Esperado '[' para iniciar o especificador de formato decimal");
    expect_token(p, TOKEN_DECIMAL, "Esperado um NÚMERO DECIMAL como limitador de valor");
    expect_token(p, TOKEN_RBRACKET, "Esperado ']' para fechar o especificador de formato decimal");
}

static void parse_function_call(Parser *p, ASTNode *parent)
{
    Token func_token = current_token(p);

    Symbol *func_symbol = scope_manager_lookup(p->sm, func_token.literal);
    if (func_symbol == NULL)
    {
        fprintf(stderr, "[ERROR|Semantic]: Linha %lu: Tentativa de chamar a função não declarada '%s'.\n",
                func_token.line_num, func_token.literal);
        exit(EXIT_FAILURE);
    }
    if (func_symbol->kind != KIND_FUNC)
    {
        fprintf(stderr, "[ERROR|Semantic]: Linha %lu: O identificador '%s' não é uma função e não pode ser chamado.\n",
                func_token.line_num, func_token.literal);
        exit(EXIT_FAILURE);
    }

    ASTNode *func_call_node = ast_add_child(parent, AST_FUNCTION_CALL_NODE, func_token.literal, func_token.line_num);

    advance_token(p);

    parse_argument_list(p, func_call_node);
}

static void parse_function_declaration(Parser *p, ASTNode *parent)
{
    advance_token(p);
    Token func_token = current_token(p);
    expect_token(p, TOKEN_IDENT_FUNC, "Esperado um identificador de função (iniciado com '__')");

    scope_manager_insert(p->sm, func_token.literal, KIND_FUNC, TOKEN_VOID_TYPE);

    scope_manager_enter_scope(p->sm, func_token.literal);

    ASTNode *func_node = ast_add_child(parent, AST_FUNCTION_DECLARATION_NODE, func_token.literal, func_token.line_num);

    parse_parameter_list(p, func_token, func_node);
    skip_eols(p);
    parse_block(p, func_node);

    scope_manager_exit_scope(p->sm);
}

static void parse_return_statement(Parser *p, ASTNode *parent)
{
    ASTNode *return_node = ast_add_child(parent, AST_FUNCTION_RETURN_NODE, "return", current_token(p).line_num);
    advance_token(p);
    parse_expression(p, return_node);
    expect_token(p, TOKEN_SEMICOLON, "Esperado ';' após a expressão de retorno");
}

/**
 * @brief Parses a variable declaration in the source code.
 *
 * This function analyzes the current tokens in the parser to identify and process
 * a variable declaration statement. It updates the parser state accordingly and
 * may add the declared variable to the symbol table.
 *
 * @param p Pointer to the Parser structure containing parsing state and context.
 * @param parent Pointer to the parent node in the AST.
 */
static void parse_variable_declaration(Parser *p, ASTNode *parent)
{

    TokenType declaration_type = current_token(p).type;
    advance_token(p);

    ASTNode **variable_declaration_nodes = allocate_memory(sizeof(ASTNode *));
    int var_decl_count = 0;

    while (1)
    {

        Token var_token = current_token(p);
        expect_token(p, TOKEN_IDENT_VAR, "Esperado um identificador de variável (iniciado com '!')");

        if (!scope_manager_insert(p->sm, var_token.literal, KIND_VAR, declaration_type))
        {
            fprintf(stderr, "[ERROR|Semantic] Linha %lu: A variável '%s' já foi declarada anteriormente.\n",
                    var_token.line_num, var_token.literal);
            exit(EXIT_FAILURE);
        }

        variable_declaration_nodes[var_decl_count++] = ast_add_child(parent, AST_VARIABLE_DECLARATION_NODE, var_token.literal, var_token.line_num);
        variable_declaration_nodes = reallocate_memory(variable_declaration_nodes, sizeof(ASTNode *) * (var_decl_count + 1));

        if (declaration_type == TOKEN_DEC_TYPE && current_token(p).type == TOKEN_LBRACKET)
        {
            parse_decimal_specifier(p);
        }
        else if (declaration_type == TOKEN_INT_TYPE && current_token(p).type == TOKEN_LBRACKET)
        {
            advance_token(p);

            expect_token(p, TOKEN_INT, "Tamanho do vetor deve ser um inteiro");
            expect_token(p, TOKEN_RBRACKET, "Esperado ']' para fechar a declaração de vetor");
        }

        if (current_token(p).type == TOKEN_ASSIGN)
        {
            advance_token(p);

            ASTNode *temporary_tree = generate_temporary_node(NULL, current_token(p).line_num);
            temporary_tree->type = AST_ASSIGNMENT_NODE;
            temporary_tree->literal = "=";

            parse_expression(p, temporary_tree);

            while (var_decl_count > 0)
            {
                ASTNode *var_node = variable_declaration_nodes[--var_decl_count];
                ast_add_existing_child_copy(var_node, temporary_tree);
            }
            for (int i = 0; i < temporary_tree->child_count; i++)
            {
                free_memory(temporary_tree->children[i]);
            }
            free_memory(temporary_tree->children);
            free_memory(temporary_tree);
        }

        if (current_token(p).type != TOKEN_COMMA)
        {
            break;
        }

        advance_token(p);
    }

    free_memory(variable_declaration_nodes);
    expect_token(p, TOKEN_SEMICOLON, "Esperado ';' no final da declaração de variável");
}

/**
 * @brief Parses an assignment statement in the source code.
 *
 * This function analyzes the current tokens in the parser and processes
 * an assignment statement, updating the parser state accordingly.
 *
 * @param p Pointer to the Parser structure containing parsing context.
 */
static void parse_assignment_statement(Parser *p, ASTNode *parent, short is_semicolon_expected)
{
    Token var_token = current_token(p);

    parent = ast_add_child(parent, AST_ASSIGNMENT_NODE, var_token.literal, var_token.line_num);

    if (scope_manager_lookup(p->sm, var_token.literal) == NULL)
    {
        fprintf(stderr, "[ERROR|Semantic] Linha %lu: A variável '%s' não foi declarada.\n",
                var_token.line_num, var_token.literal);
        exit(EXIT_FAILURE);
    }

    expect_token(p, TOKEN_IDENT_VAR, "Esperado uma variável para atribuição");
    expect_token(p, TOKEN_ASSIGN, "Esperado '=' em uma atribuição");
    parse_expression(p, parent);

    parent = parent->parent;

    if (is_semicolon_expected)
        expect_token(p, TOKEN_SEMICOLON, "Esperado ';' no final da atribuição");
}

/**
 * @brief Parses an I/O statement in the given parser context.
 *
 * This function processes input/output statements encountered during parsing.
 *
 * @param p Pointer to the Parser structure containing the current parsing state.
 */
static void parse_io_statement(Parser *p, ASTNode *parent)
{
    ASTNode *io_node = ast_add_child(parent, AST_FUNCTION_CALL_NODE, current_token(p).literal, current_token(p).line_num);
    TokenType type = current_token(p).type;
    advance_token(p);
    parse_argument_list(p, io_node);
    if (type == TOKEN_READ)
        expect_token(p, TOKEN_SEMICOLON, "Esperado ';' no final da chamada 'leia'");
    else
        expect_token(p, TOKEN_SEMICOLON, "Esperado ';' no final da chamada 'escreva'");
}

/**
 * @brief Parses an 'if' statement in the source code.
 *
 * This function analyzes the current token stream in the parser to identify and
 * process an 'if' statement, including its condition and associated statement blocks.
 *
 * @param p Pointer to the Parser structure containing the current parsing state.
 */
static void parse_if_statement(Parser *p, ASTNode *parent)
{
    ASTNode *if_node = ast_add_child(parent, AST_IF_NODE, "if", current_token(p).line_num);
    advance_token(p);
    expect_token(p, TOKEN_LPAREN, "Esperado '(' após 'se'");
    parse_expression(p, if_node);
    expect_token(p, TOKEN_RPAREN, "Esperado ')' após a condição do 'se'");
    skip_eols(p);

    for (int i = 0; i < if_node->child_count; i++)
    {
        ASTNode *child = if_node->children[i];
        if (child->type == AST_EXPRESSION_NODE)
        {
            child->type = AST_EXPRESSION_NODE;
            child->literal = "IF Condition";
        }
    }

    if (current_token(p).type == TOKEN_LBRACE)
    {
        parse_block(p, if_node);
        for (int i = 0; i < if_node->child_count; i++)
        {
            ASTNode *child = if_node->children[i];
            if (child->type == AST_BLOCK_NODE)
            {
                child->type = AST_BLOCK_NODE;
                child->literal = "IF Block";
            }
        }
    }
    else
    {
        parse_statement(p, if_node);
    }

    skip_eols(p);
    if (current_token(p).type == TOKEN_ELSE)
    {
        advance_token(p);
        skip_eols(p);
        if (current_token(p).type == TOKEN_LBRACE)
        {
            parse_block(p, if_node);
            for (int i = 0; i < if_node->child_count; i++)
            {
                ASTNode *child = if_node->children[i];
                if (child->type == AST_BLOCK_NODE)
                {
                    child->type = AST_BLOCK_NODE;
                    child->literal = "ELSE Block";
                }
            }
        }
        else
        {
            parse_statement(p, if_node);
        }
    }
}

/**
 * @brief Parses a 'for' statement in the source code.
 *
 * This function analyzes the tokens provided by the parser to identify and process
 * a 'for' loop construct. It updates the parser state accordingly and generates
 * the necessary intermediate representation or AST nodes for the 'for' statement.
 *
 * @param p Pointer to the Parser structure containing the current parsing context.
 */
static void parse_for_statement(Parser *p, ASTNode *parent)
{
    expect_token(p, TOKEN_FOR, "Esperado 'para' para iniciar um loop");

    ASTNode *for_node = ast_add_child(parent, AST_FOR_NODE, "for", current_token(p).line_num);

    expect_token(p, TOKEN_LPAREN, "Esperado '(' após a palavra-chave 'para'");

    if (current_token(p).type != TOKEN_SEMICOLON)
    {

        if (current_token(p).type == TOKEN_IDENT_VAR)
        {
            parse_assignment_statement(p, for_node, 1);
        }
        else
        {
            parse_variable_declaration(p, for_node);
        }
    }
    else
    {
        expect_token(p, TOKEN_SEMICOLON, "Esperado ';' após a inicialização (vazia) do loop 'para'");
    }

    if (current_token(p).type != TOKEN_SEMICOLON)
    {
        parse_expression(p, for_node);
        expect_token(p, TOKEN_SEMICOLON, "Esperado ';' após a condição do loop 'para'");
    }
    else
    {
        expect_token(p, TOKEN_SEMICOLON, "Esperado ';' após a condição vazia do loop 'para'");
    }

    if (current_token(p).type != TOKEN_RPAREN)
    {
        parse_assignment_statement(p, for_node, 0);
        expect_token(p, TOKEN_RPAREN, "Esperado ')' para fechar a declaração do loop 'para'");
    }
    else
    {
        expect_token(p, TOKEN_RPAREN, "Esperado ')' para fechar a declaração do loop 'para'");
    }

    skip_eols(p);

    if (current_token(p).type == TOKEN_LBRACE)
    {
        parse_block(p, for_node);
    }
    else
    {
        parse_statement(p, parent);
    }
}

/**
 * @brief Parses the main function in the source code.
 *
 * This function analyzes the tokens provided by the parser to identify and process
 * the main function definition. It updates the parser state accordingly.
 *
 * @param p Pointer to the Parser structure containing parsing context and state.
 */
static void parse_main_function(Parser *p, ASTNode *parent)
{
    ASTNode *main_node = ast_add_child(parent, AST_MAIN_NODE, "main", current_token(p).line_num);
    advance_token(p);
    expect_token(p, TOKEN_LPAREN, "Esperado '(' na função principal");
    expect_token(p, TOKEN_RPAREN, "Esperado ')' na função principal");
    skip_eols(p);
    parse_block(p, main_node);
}

/**
 * @brief Parses a single statement from the input using the provided parser.
 *
 * This function analyzes the current position of the parser and processes
 * a statement according to the language grammar. It updates the parser's
 * internal state as necessary.
 *
 * @param p Pointer to the Parser structure containing parsing context and state.
 */
static void parse_statement(Parser *p, ASTNode *parent)
{
    switch (current_token(p).type)
    {

    case TOKEN_INT_TYPE:
    case TOKEN_DEC_TYPE:
    case TOKEN_TEXT_TYPE:
            parse_variable_declaration(p, parent);
        break;
    case TOKEN_IDENT_VAR:
        parse_assignment_statement(p, parent, 1);
        break;
    case TOKEN_FUNCTION:
        parse_function_declaration(p, parent);
        break;
    case TOKEN_RETURN:
        parse_return_statement(p, parent);
        break;
    case TOKEN_IF:
        parse_if_statement(p, parent);
        break;
    case TOKEN_FOR:
        parse_for_statement(p, parent);
        break;
    case TOKEN_READ:
    case TOKEN_PRINT:
        parse_io_statement(p, parent);
        break;
    case TOKEN_MAIN:
        parse_main_function(p, parent);
        break;
    default:
        fprintf(stderr, "[ERROR|Syntax] Linha %lu: Comando inesperado iniciado com token '%s' (Tipo: %d).\n",
                current_token(p).line_num, current_token(p).literal, current_token(p).type);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Parses the entire program using the given parser.
 *
 * This function processes the input according to the program's grammar rules,
 * updating the parser state as necessary. It is typically the entry point for
 * parsing a complete source file or code block.
 *
 * @param p Pointer to the Parser structure containing parsing state and input.
 */
static void parse_program(Parser *p, ASTNode *parent)
{
    skip_eols(p);
    while (current_token(p).type != TOKEN_EOF)
    {
        parse_statement(p, parent);
        skip_eols(p);
    }
}

/**
 * Parses the provided source code tokens and manages scopes.
 *
 * @param tokens        Array of tokens representing the source code.
 * @param token_count   Number of tokens in the array.
 * @param sm            Pointer to the ScopeManager for handling scope information.
 * @return              Status code indicating success or failure of parsing.
 */
int parse_source_code(Token *tokens, int token_count, ScopeManager *sm)
{
    ast_root_node = ast_generate_root();

    Parser p = {
        .tokens = tokens,
        .token_count = token_count,
        .current_pos = 0,
        .sm = sm};

    parse_program(&p, ast_root_node);

    return 0;
}

ASTNode *get_ast()
{
    return ast_root_node;
}