// parser/parser.c

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

// A estrutura interna do parser
struct Parser {
    Token *tokens;
    int token_count;
    int current_pos;
};

// --- Funções Auxiliares Internas ---

// Retorna o token atual sem avançar
static Token current_token(Parser *p) {
    if (p->current_pos < p->token_count)
        return p->tokens[p->current_pos];
    // Se estivermos fora dos limites, retorna o último token (deve ser EOF)
    return p->tokens[p->token_count - 1];
}

// Avança para o próximo token
static void advance_token(Parser *p) {
    if (p->current_pos < p->token_count) {
        p->current_pos++;
    }
}

// Verifica se o token atual é do tipo esperado.
// Se for, avança. Se não, imprime um erro e encerra.
static void expect_token(Parser *p, TokenType type, const char *error_msg) {
    Token tok = current_token(p);
    if (tok.type == type) {
        advance_token(p);
    } else {
        fprintf(stderr, "[Erro Sintático] Linha %lu: %s. Esperado token do tipo %d, mas encontrado %d ('%s').\n",
                tok.line_num, error_msg, type, tok.type, tok.literal);
        exit(EXIT_FAILURE);
    }
}

// Pula todos os tokens EOL (fim de linha) que encontrar
static void skip_eols(Parser *p) {
    while (current_token(p).type == TOKEN_EOL) {
        advance_token(p);
    }
}

// --- Funções de Análise da Gramática (Protótipos) ---
static void parse_statement(Parser *p);
static void parse_expression(Parser *p);

// --- Implementação das Funções de Análise ---

// Analisa um bloco de código entre chaves {}
static void parse_block(Parser *p) {
    expect_token(p, TOKEN_LBRACE, "Esperado '{' para iniciar um bloco");
    skip_eols(p);
    while (current_token(p).type != TOKEN_RBRACE && current_token(p).type != TOKEN_EOF) {
        parse_statement(p);
        skip_eols(p);
    }
    expect_token(p, TOKEN_RBRACE, "Esperado '}' para fechar um bloco");
}

// Analisa uma expressão simples
static void parse_expression(Parser *p) {
    // Apenas consome tokens até um delimitador
    while (current_token(p).type != TOKEN_SEMICOLON &&
           current_token(p).type != TOKEN_RPAREN &&
           current_token(p).type != TOKEN_COMMA &&
           current_token(p).type != TOKEN_EOL &&
           current_token(p).type != TOKEN_EOF) {
        advance_token(p);
    }
}

// Analisa uma lista de argumentos (ex: em 'escreva("ola", !a)')
static void parse_argument_list(Parser *p) {
    expect_token(p, TOKEN_LPAREN, "Esperado '(' antes da lista de argumentos");
    if (current_token(p).type != TOKEN_RPAREN) {
        parse_expression(p);
        while (current_token(p).type == TOKEN_COMMA) {
            advance_token(p);
            parse_expression(p);
        }
    }
    expect_token(p, TOKEN_RPAREN, "Esperado ')' após a lista de argumentos");
}

// Analisa uma declaração de variável (ex: inteiro !a, !b=7;)
static void parse_variable_declaration(Parser *p) {
    // Guarda o tipo da variável que está sendo declarada
    TokenType declaration_type = current_token(p).type;
    advance_token(p); // Consome o token de tipo (ex: TOKEN_INT_TYPE)

    // Loop para permitir múltiplas declarações na mesma linha (ex: inteiro !a, !b;)
    while (1) {
        expect_token(p, TOKEN_IDENT_VAR, "Esperado um identificador de variável (iniciado com '!')");

        // MODIFICADO: Usa os novos tokens de tipo
        if (declaration_type == TOKEN_DEC_TYPE && current_token(p).type == TOKEN_LBRACKET) {
            expect_token(p, TOKEN_LBRACKET, "Esperado '[' para iniciar o especificador de formato decimal");
            expect_token(p, TOKEN_DECIMAL, "Esperado um NÚMERO DECIMAL como limitador de valor");
            expect_token(p, TOKEN_RBRACKET, "Esperado ']' para fechar o especificador de formato decimal");
        }
        else if (declaration_type == TOKEN_INT_TYPE && current_token(p).type == TOKEN_LBRACKET) {
            advance_token(p); // Consome '['
            expect_token(p, TOKEN_INT, "Tamanho do vetor deve ser um inteiro");
            expect_token(p, TOKEN_RBRACKET, "Esperado ']' para fechar a declaração de vetor");
        }

        // Após a variável e seu possível especificador, pode haver uma atribuição.
        if (current_token(p).type == TOKEN_ASSIGN) {
            advance_token(p); // Consome '='
            parse_expression(p);
        }

        // Se não houver uma vírgula, a declaração terminou. Saia do loop.
        if (current_token(p).type != TOKEN_COMMA) {
            break;
        }
        
        advance_token(p); // Consome a vírgula para a próxima declaração na mesma linha.
    }
    
    // Toda declaração deve terminar com ponto e vírgula.
    expect_token(p, TOKEN_SEMICOLON, "Esperado ';' no final da declaração de variável");
}

// Analisa uma atribuição (ex: !b2 = !a;)
static void parse_assignment_statement(Parser *p) {
    expect_token(p, TOKEN_IDENT_VAR, "Esperado uma variável para atribuição");
    expect_token(p, TOKEN_ASSIGN, "Esperado '=' em uma atribuição");
    parse_expression(p);
    expect_token(p, TOKEN_SEMICOLON, "Esperado ';' no final da atribuição");
}

// Analisa os comandos 'leia' ou 'escreva'
static void parse_io_statement(Parser *p) {
    TokenType type = current_token(p).type;
    advance_token(p); // Consome 'leia' ou 'escreva'
    parse_argument_list(p);
    if (type == TOKEN_READ)
      expect_token(p, TOKEN_SEMICOLON, "Esperado ';' no final da chamada 'leia'");
    else
      expect_token(p, TOKEN_SEMICOLON, "Esperado ';' no final da chamada 'escreva'");
}

// Analisa um comando 'se'/'senao'
static void parse_if_statement(Parser *p) {
    advance_token(p); // Consome 'se'
    expect_token(p, TOKEN_LPAREN, "Esperado '(' após 'se'");
    parse_expression(p);
    expect_token(p, TOKEN_RPAREN, "Esperado ')' após a condição do 'se'");
    skip_eols(p);

    if (current_token(p).type == TOKEN_LBRACE) parse_block(p);
    else parse_statement(p);

    skip_eols(p);
    if (current_token(p).type == TOKEN_ELSE) {
        advance_token(p); // Consome 'senao'
        skip_eols(p);
        if (current_token(p).type == TOKEN_LBRACE) parse_block(p);
        else parse_statement(p);
    }
}

static void parse_for_statement(Parser *p) {
    // 1. Consome a palavra-chave 'para'
    expect_token(p, TOKEN_FOR, "Esperado 'para' para iniciar um loop");

    // 2. Consome o parêntese de abertura '('
    expect_token(p, TOKEN_LPAREN, "Esperado '(' após a palavra-chave 'para'");

    // 3. Analisa a seção de INICIALIZAÇÃO
    // A inicialização é opcional, mas se existir, deve ser uma atribuição.
    if (current_token(p).type != TOKEN_SEMICOLON) {
        parse_assignment_statement(p); // Esta função já espera um ';' no final
    } else {
        expect_token(p, TOKEN_SEMICOLON, "Esperado ';' após a inicialização (vazia) do loop 'para'");
    }

    // 4. Analisa a seção de CONDIÇÃO
    // A condição é opcional. Se estiver vazia, o loop é infinito.
    if (current_token(p).type != TOKEN_SEMICOLON) {
        parse_expression(p);
    }
    expect_token(p, TOKEN_SEMICOLON, "Esperado ';' após a condição do loop 'para'");

    // 5. Analisa a seção de INCREMENTO
    // O incremento também é opcional.
    if (current_token(p).type != TOKEN_RPAREN) {
        parse_expression(p);
    }

    // 6. Consome o parêntese de fechamento ')'
    expect_token(p, TOKEN_RPAREN, "Esperado ')' para fechar a declaração do loop 'para'");
    skip_eols(p);

    // 7. Analisa o CORPO do loop (pode ser um bloco {} ou um único comando)
    if (current_token(p).type == TOKEN_LBRACE) {
        parse_block(p);
    } else {
        parse_statement(p);
    }
}

// Analisa a função 'principal'
static void parse_main_function(Parser *p) {
    advance_token(p); // Consome 'principal'
    expect_token(p, TOKEN_LPAREN, "Esperado '(' na função principal");
    expect_token(p, TOKEN_RPAREN, "Esperado ')' na função principal");
    skip_eols(p);
    parse_block(p);
}

static void parse_statement(Parser *p) {
    switch (current_token(p).type) {
        // MODIFICADO: Casos atualizados para os novos tokens _TYPE
        case TOKEN_INT_TYPE:
        case TOKEN_DEC_TYPE:
        case TOKEN_TEXT_TYPE:
            parse_variable_declaration(p);
            break;
        case TOKEN_IDENT_VAR:
            parse_assignment_statement(p);
            break;
        case TOKEN_IF:
            parse_if_statement(p);
            break;
        case TOKEN_FOR:
            parse_for_statement(p);
            break;
        case TOKEN_READ:
        case TOKEN_PRINT:
            parse_io_statement(p);
            break;
        case TOKEN_MAIN:
            parse_main_function(p);
            break;
        default:
            fprintf(stderr, "[Erro Sintático] Linha %lu: Comando inesperado iniciado com token '%s' (Tipo: %d).\n",
                    current_token(p).line_num, current_token(p).literal, current_token(p).type);
            exit(EXIT_FAILURE);
    }
}

// Ponto de entrada do programa
static void parse_program(Parser *p) {
    skip_eols(p);
    while (current_token(p).type != TOKEN_EOF) {
        parse_statement(p);
        skip_eols(p);
    }
}


// --- Função Pública da API ---

int parse_source_code(Token *tokens, int token_count) {
    // 1. Cria e inicializa a estrutura do Parser
    Parser p = {
        .tokens = tokens,
        .token_count = token_count,
        .current_pos = 0
    };

    // 2. Inicia a análise a partir da regra principal da gramática
    parse_program(&p);

    // 3. Se chegou aqui, a análise foi bem-sucedida
    return 0; // Sucesso
}