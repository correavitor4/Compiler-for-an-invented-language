
#include "lex.h"

#include "hash/hash.h"
#include "tokens/tokens.h"

#include "memory/memory_controller.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief Reads the next character or token from the input stream in the lexer.
 *
 * This function advances the lexer by reading the next character or token
 * from its input source and updates the internal state of the lexer accordingly.
 *
 * @param l Pointer to the Lexer structure to operate on.
 */
void read(Lexer *l)
{

    if (l->read_position >= strlen(l->current_line))
    {
        l->ch = '\0'; // Fim da linha atual
    }
    else
    {
        l->ch = l->current_line[l->read_position];
    }

    l->position = l->read_position;
    l->read_position += 1;
}

static char peek(Lexer* l) {
    if (l->read_position >= strlen(l->current_line)) {
        return '\0';
    }
    return l->current_line[l->read_position];
}

/**
 * @brief Returns the next character in the input stream without advancing the lexer position.
 *
 * @param l Pointer to the Lexer structure.
 * @return The next character to be read from the input, or a special value (e.g., EOF) if at the end.
 */
static char *read_number(Lexer *l)
{
    size_t start_pos = l->position;
    while (isdigit(l->ch))
    {
        read(l);
    }
    if (l->ch == '.' && isdigit(peek(l)))
    {
        read(l);
        while (isdigit(l->ch))
        {
            read(l);
        }
    }
    size_t length = l->position - start_pos;

    char *num = allocate_memory(length + 1);
    strncpy(num, &l->current_line[start_pos], length);

    num[length] = '\0';
    return num;
}

/**
 * Reads a string literal from the input using the given lexer.
 *
 * This function assumes that the lexer is currently positioned at the opening
 * quote of a string literal. It processes the input, handling escape sequences
 * as necessary, and returns a newly allocated null-terminated string containing
 * the parsed value. The lexer position is advanced past the closing quote.
 *
 * @param l Pointer to the Lexer instance from which to read the string.
 * @return A pointer to a newly allocated string containing the parsed value,
 *         or NULL if an error occurs (e.g., unterminated string or allocation failure).
 *         The caller is responsible for freeing the returned string.
 */
// TODO: Handle line breaks with \ or another escape sequence.
static char *read_string(Lexer *l)
{
    size_t start_pos = l->position + 1;

    read(l);

    while (1)
    {
        if (l->ch == '\n' || l->ch == '\0') {
            return NULL;
        }
        if (l->ch == '"') {
            break;
        }

        read(l);
    }
    size_t length = l->position - start_pos;

    char *str = allocate_memory(length + 1);
    strncpy(str, &l->current_line[start_pos], length);

    str[length] = '\0';
    if (l->ch == '"')
    {
        read(l);
    }
    return str;
}

/**
 * Reads an identifier from the current position of the lexer.
 *
 * This function scans the input starting from the current position in the lexer,
 * collects characters that are valid as part of an identifier (typically letters,
 * digits, and underscores), and returns a newly allocated string containing the
 * identifier. The lexer's position is advanced past the identifier.
 *
 * @param l Pointer to the Lexer structure.
 * @return A pointer to a newly allocated null-terminated string containing the identifier.
 *         The caller is responsible for freeing the returned string.
 */
static char* read_identifier(Lexer* l) {
    size_t start_pos = l->position;
    if (l->ch == '!' || (l->ch == '_' && peek(l) == '_')) {
        read(l);
        if (l->ch == '_') read(l);
    }
    while (isalnum(l->ch) || l->ch == '_') {
         read(l);
    }
    size_t length = l->position - start_pos;

    char* ident = allocate_memory(length + 1);
    strncpy(ident, &l->current_line[start_pos], length);

    ident[length] = '\0';
    return ident;
}

/**
 * @brief Skips over any whitespace characters (spaces, tabs).
 * @param l The lexer instance.
 */
static void skip_whitespace(Lexer *l)
{
    while (l->ch == ' ' || l->ch == '\t' || l->ch == '\n' || l->ch == '\r') {
        read(l);
    }
}

/**
 * @brief Looks up the token type for a given identifier string.
 *
 * This function takes a string representing an identifier and returns
 * the corresponding TokenType. It is typically used to distinguish
 * between keywords and user-defined identifiers during lexical analysis.
 *
 * @param ident The identifier string to look up.
 * @return The TokenType associated with the identifier.
 */
TokenType lookup(const char *ident)
{

    unsigned long hash = HASH.djb2(ident);

    // NOTE: The hash values below are precomputed for the keywords.
    // They are based on the djb2 hash function applied to the keyword strings.
    // If you change the keywords, you need to update these hash values accordingly.

    // The if clause is used in cases which collision may occur.
    switch (hash)
    {
    // --- RESERVED KEYWORDS ---
    // case hash("se"):
    case 5863805UL:
        if (strcmp(ident, "se") == 0)
            return TOKEN_IF;
        break;

    // case hash("senao"):
    case 210727683387UL:
        if (strcmp(ident, "senao") == 0)
            return TOKEN_ELSE;
        break;

    // case hash("para"):
    case 6385575337UL:
        if (strcmp(ident, "para") == 0)
            return TOKEN_FOR;
        break;

    // case hash("leia"):
    case 6385435648UL:
        if (strcmp(ident, "leia") == 0)
            return TOKEN_READ;
        break;
    // case hash("escreva"):
    case 229464902111278UL:
        if (strcmp(ident, "escreva") == 0)
            return TOKEN_PRINT;
        break;

    // case hash("funcao"):
    case 6953523768225UL:
        if (strcmp(ident, "funcao") == 0)
            return TOKEN_FUNCTION;
        break;

    // case hash("principal"):
    case 249902713864986279UL:
        if (strcmp(ident, "principal") == 0)
            return TOKEN_MAIN;
        break;

    // --- TYPES ---
    // case hash("inteiro"):
    case 229469892003903UL:
        if (strcmp(ident, "inteiro") == 0)
            return TOKEN_INT;
        break;

    // case hash("decimal"):
    case 229463062432404UL:
        if (strcmp(ident, "decimal") == 0)
            return TOKEN_DECIMAL;
        break;

    // case hash("texto"):
    case 210728880825UL:
        if (strcmp(ident, "texto") == 0)
            return TOKEN_TEXT;
        break;
    }
    // No match found, return TOKEN_ILLEGAL
    return TOKEN_ILLEGAL;
}

/**
 * @brief Retrieves the next token from the input stream managed by the lexer.
 *
 * This function analyzes the current state of the lexer and returns the next
 * available token. It advances the lexer's position as necessary.
 *
 * @param l Pointer to the Lexer structure containing the input stream and state.
 * @return The next Token parsed from the input.
 */
Token _next_token_impl(Lexer *l)
{
    Token tok;
    tok.literal = NULL;

    tok.line_num = l->line_num;

    skip_whitespace(l);

    switch (l->ch)
    {
    case '=':
        if (peek(l) == '=')
        {
            read(l);
            tok.type = TOKEN_EQ;

            tok.literal = allocate_memory(sizeof(char) * 3);
            memcpy(tok.literal, "==", 3);
        }
        else
        {
            tok.type = TOKEN_ASSIGN;

            tok.literal = allocate_memory(sizeof(char) * 2);
            memcpy(tok.literal,"=", 2);
        }
        break;
    case '<':
        if (peek(l) == '>')
        {
            read(l);
            tok.type = TOKEN_NOT_EQ;

            tok.literal = allocate_memory(sizeof(char) * 3);
            memcpy(tok.literal, "<>", 3);
        }
        else if (peek(l) == '=')
        {
            read(l);
            tok.type = TOKEN_LTE;

            tok.literal = allocate_memory(sizeof(char) * 3);
            memcpy(tok.literal, "<=", 3);
        }
        else
        {
            tok.type = TOKEN_LT;

            tok.literal = allocate_memory(sizeof(char) * 2);
            memcpy(tok.literal, "<", 2);
        }
        break;
    case '>':
        if (peek(l) == '=')
        {
            read(l);
            tok.type = TOKEN_GTE;

            tok.literal = allocate_memory(sizeof(char) * 3);
            memcpy(tok.literal, ">=", 3);
        }
        else
        {
            tok.type = TOKEN_GT;

            tok.literal = allocate_memory(sizeof(char) * 2);
            memcpy(tok.literal, ">", 2);
        }
        break;
    case '+':
        tok.type = TOKEN_PLUS;

        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, "+", 2);
        break;
    case '-':
        tok.type = TOKEN_MINUS;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, "-", 2);
        break;
    case '*':
        tok.type = TOKEN_ASTERISK;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, "*", 2);
        break;
    case '/':
        tok.type = TOKEN_SLASH;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, "/", 2);
        break;
    case '&':
        if (peek(l) == '&')
        {
            read(l);
            tok.type = TOKEN_AND;
            tok.literal = allocate_memory(sizeof(char) * 3);
            memcpy(tok.literal, "&&", 3);
        }
        else
        {
            tok.type = TOKEN_ILLEGAL; // Single '&' is illegal in this context
            tok.literal = allocate_memory(sizeof(char) * 2);
            memcpy(tok.literal, "&", 2);
        }
        break;
    case '|':
        if (peek(l) == '|')
        {
            read(l);
            tok.type = TOKEN_OR;
            tok.literal = allocate_memory(sizeof(char) * 3);
            memcpy(tok.literal, "||", 3);
        }
        else
        {
            tok.type = TOKEN_ILLEGAL; // Single '|' is illegal in this context
            tok.literal = allocate_memory(sizeof(char) * 2);
            memcpy(tok.literal, "|", 2);
        }
        break;
    case ';':
        tok.type = TOKEN_SEMICOLON;

        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, ";", 2);
        break;
    case '(':
        tok.type = TOKEN_LPAREN;

        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal,"(", 2);
        break;
    case ')':
        tok.type = TOKEN_RPAREN;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, ")", 2);
        break;
    case '{':
        tok.type = TOKEN_LBRACE;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, "{", 2);
        break;
    case '}':
        tok.type = TOKEN_RBRACE;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, "}", 2);
        break;
    case '[':
        tok.type = TOKEN_LBRACKET;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, "[", 2);
        break;
    case ']':
        tok.type = TOKEN_RBRACKET;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, "]", 2);
        break;
    case ',':
        tok.type = TOKEN_COMMA;
        tok.literal = allocate_memory(sizeof(char) * 2);
        memcpy(tok.literal, ",", 2);
        break;
    case '"':
        tok.literal = read_string(l);

        tok.type = tok.literal == NULL ? TOKEN_TEXT_INVALID : TOKEN_TEXT;
        return tok;
    case '\0':
        tok.type = TOKEN_EOL;

        tok.literal = allocate_memory(sizeof(char) * 4);
        memcpy(tok.literal, "EOL", 4);
        break; // Fim da linha
    default:
        if (l->ch == '!' || l->ch == '_' || isalpha(l->ch))
        {
            tok.literal = read_identifier(l);
            if (tok.literal[0] == '!')
            {
                tok.type = TOKEN_IDENT_VAR;
            }
            else if (tok.literal[0] == '_' && tok.literal[1] == '_')
            {
                tok.type = TOKEN_IDENT_FUNC;
            }
            else
            {
                tok.type = lookup(tok.literal);
            }
            return tok;
        }
        else if (isdigit(l->ch))
        {
            tok.literal = read_number(l);
            tok.type = (strchr(tok.literal, '.') != NULL) ? TOKEN_DECIMAL : TOKEN_INT;
            return tok;
        }
        else
        {
            tok.type = TOKEN_ILLEGAL;
            tok.literal = allocate_memory(sizeof(char) * 2);
            tok.literal[0] = l->ch;
            tok.literal[1] = '\0';
        }
        break;
    }
    read(l);
    return tok;
}

/**
 * @brief Creates a new Lexer instance for the given input string.
 *
 * This function allocates and initializes a new Lexer object that will
 * process the provided input string for lexical analysis.
 *
 * @param input The input string to be tokenized by the lexer.
 * @return Pointer to the newly created Lexer instance, or NULL on failure.
 */
Lexer *_new_impl(const char *input)
{

    Lexer *l = allocate_memory(sizeof(Lexer));

    l->current_line = NULL;
    l->line_num = 0;
    l->position = 0;
    l->read_position = 0;
    l->ch = 0;

    return l;
}

/**
 * @brief Loads a line of text into the lexer for processing.
 *
 * This function initializes the lexer with the provided line of source code,
 * setting up internal state as needed for lexical analysis.
 *
 * @param l Pointer to the Lexer structure to be updated.
 * @param line The line of text to load into the lexer.
 * @param line_num The line number in the source file (used for error reporting).
 */
void _load_line_impl(Lexer *l, const char *line, int line_num)
{
    if (l == NULL || line == NULL)
        return;

    l->current_line = line;
    l->line_num = line_num;

    l->position = 0;
    l->read_position = 0;
    l->ch = 0;

    read(l);
}

/**
 * @brief Global instance of the lexer API.
 *
 * This constant defines the interface and implementation for the lexer,
 * providing functions and data structures required for lexical analysis.
 * It is intended to be used as the main entry point for lexer operations.
 */
const LEXER_API LEXER = {
    .next_token = _next_token_impl,
    .new = _new_impl,
    .load_line = _load_line_impl
};