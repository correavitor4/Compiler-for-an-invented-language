#include "unity.h"

#include "lex/lex.h"
#include "tokens/tokens.h"
#include "memory/memory_controller.h"

#include <stdlib.h>
#include <string.h>

// Global lexer instance for use in tests
static Lexer* l = NULL;

// This function is run before each test
void setUp(void) {
    l = LEXER.new(NULL);
    TEST_ASSERT_NOT_NULL(l);
}

// This function is run after each test
void tearDown(void) {

}

// Helper to test a token with a DYNAMIC literal that must be freed
static void assert_dynamic_token(Token tok, TokenType expected_type, const char* expected_literal, int expected_line) {
    TEST_ASSERT_EQUAL_INT(expected_type, tok.type);
    TEST_ASSERT_EQUAL_STRING(expected_literal, tok.literal);
    TEST_ASSERT_EQUAL_INT(expected_line, tok.line_num);
}

// Helper to test a token with a STATIC literal that must NOT be freed
static void assert_static_token(Token tok, TokenType expected_type, const char* expected_literal, int expected_line) {
    TEST_ASSERT_EQUAL_INT(expected_type, tok.type);
    TEST_ASSERT_EQUAL_STRING(expected_literal, tok.literal);
    TEST_ASSERT_EQUAL_INT(expected_line, tok.line_num);
    // Do not free the static literal
}

// Keywords are likely static strings, so they should not be freed.
void test_keywords_and_identifiers(void) {
    LEXER.load_line(l, "se senao para leia escreva funcao principal inteiro decimal texto", 1);

    Token t;
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_IF, "se", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ELSE, "senao", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_FOR, "para", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_READ, "leia", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_PRINT, "escreva", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_FUNCTION, "funcao", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_MAIN, "principal", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_INT, "inteiro", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_DECIMAL, "decimal", 1);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_TEXT, "texto", 1);
}

void test_keywords_and_identifiers_invalid(void) {
    LEXER.load_line(l, "s sen p para123 leia_123 escrva fncao princial iteiro deimal teto", 2);

    Token t;

    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "s", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "sen", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "p", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "para123", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "leia_123", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "escrva", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "fncao", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "princial", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "iteiro", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "deimal", 2);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ILLEGAL, "teto", 2);
}

// Variable/function identifiers are dynamic and must be freed.
void test_ident_var_and_func(void) {
    LEXER.load_line(l, "!var __func !vAr __fUnc", 2);

    Token t;
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_IDENT_VAR, "!var", 2);
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_IDENT_FUNC, "__func", 2);
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_IDENT_VAR, "!vAr", 2);
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_IDENT_FUNC, "__fUnc", 2);
}

void test_ident_var_and_func_invalid(void) {
    LEXER.load_line(l, "!var123 __func_123 !Var __Func", 2);

    Token t;
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_IDENT_VAR, "!var123", 2);
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_IDENT_FUNC, "__func_123", 2);
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_IDENT_VAR, "!Var", 2);
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_IDENT_FUNC, "__Func", 2);
}


// Numbers are parsed into new strings, so they are dynamic.
void test_numbers(void) {
    LEXER.load_line(l, "123 45.67", 3);

    Token t;
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_INT, "123", 3);
    t = LEXER.next_token(l); assert_dynamic_token(t, TOKEN_DECIMAL, "45.67", 3);
}

// Single-character operators and delimiters are static.
void test_operators_and_delimiters(void) {
    LEXER.load_line(l, "= == < <= > >= <> + - * / && || ; ( ) { } [ ] ,", 4);

    Token t;
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ASSIGN, "=", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_EQ, "==", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_LT, "<", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_LTE, "<=", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_GT, ">", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_GTE, ">=", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_NOT_EQ, "<>", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_PLUS, "+", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_MINUS, "-", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_ASTERISK, "*", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_SLASH, "/", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_AND, "&&", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_OR, "||", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_SEMICOLON, ";", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_LPAREN, "(", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_RPAREN, ")", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_LBRACE, "{", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_RBRACE, "}", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_LBRACKET, "[", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_RBRACKET, "]", 4);
    t = LEXER.next_token(l); assert_static_token(t, TOKEN_COMMA, ",", 4);
}

// String literal contents are dynamic.
void test_string_literal(void) {
    LEXER.load_line(l, "\"hello world\"", 5);

    Token t = LEXER.next_token(l);
    TEST_ASSERT_EQUAL_INT(TOKEN_TEXT, t.type);
    TEST_ASSERT_EQUAL_STRING("hello world", t.literal);
    TEST_ASSERT_EQUAL_INT(5, t.line_num);
}

// An unterminated string has no literal to free.
void test_unterminated_string(void) {
    LEXER.load_line(l, "\"unterminated", 6);

    Token t = LEXER.next_token(l);
    TEST_ASSERT_EQUAL_INT(TOKEN_TEXT_INVALID, t.type);
    TEST_ASSERT_NULL(t.literal); // Expect literal to be NULL for invalid strings
}

// The captured illegal character is dynamic.
void test_illegal_token(void) {
    LEXER.load_line(l, "@", 7);

    Token t = LEXER.next_token(l);
    assert_dynamic_token(t, TOKEN_ILLEGAL, "@", 7);
}

// The "EOL" literal is static.
void test_eol_token(void) {
    LEXER.load_line(l, "", 8);

    Token t = LEXER.next_token(l);
    assert_static_token(t, TOKEN_EOL, "EOL", 8);
}

// Main function to run all the tests
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_keywords_and_identifiers);
    RUN_TEST(test_keywords_and_identifiers_invalid);
    RUN_TEST(test_ident_var_and_func);
    RUN_TEST(test_ident_var_and_func_invalid);
    RUN_TEST(test_numbers);
    RUN_TEST(test_operators_and_delimiters);
    RUN_TEST(test_string_literal);
    RUN_TEST(test_unterminated_string);
    RUN_TEST(test_illegal_token);
    RUN_TEST(test_eol_token);
    return UNITY_END();
}