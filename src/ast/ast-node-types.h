#ifndef AST_NODE_TYPES_H
#define AST_NODE_TYPES_H

typedef enum {
    AST_INVALID_NODE,
    AST_ROOT_NODE, 
    AST_MAIN_NODE,
    AST_FUNCTION_DECLARATION_NODE,
    AST_FUNCTION_RETURN_NODE,
    AST_VARIABLE_DECLARATION_NODE,
    AST_VARIABLE_REF_NODE,
    AST_FUNCTION_CALL_NODE,
    AST_ASSIGNMENT_NODE,
    AST_INT_LITERAL_NODE,
    AST_DECIMAL_LITERAL_NODE,
    AST_TEXT_LITERAL_NODE,
    AST_OP_NODE, // New node type for operators (+, -, *, /, etc.)
    AST_GROUP_NODE, // New node type for grouping expressions (parentheses)
    AST_EXPRESSION_NODE, // New node type for expressions
    AST_IF_NODE, // New node type for if statements,
    AST_ELSE_NODE, // New node type for else statements,
    AST_FOR_NODE, // New node type for for loops,
    AST_BLOCK_NODE // New node type for code blocks
} ASTNodeType;

static const char *ASTNodeTypeStrings[] = {
    "INVALID Node",
    "ROOT Node",
    "MAIN Node",
    "FUNCTION Declaration Node",
    "FUNCTION Return Node",
    "VARIABLE Declaration Node",
    "VARIABLE Reference Node",
    "FUNCTION Call Node",
    "ASSIGNMENT Node",
    "INT Literal Node",
    "DECIMAL Literal Node",
    "TEXT Literal Node",
    "OPERATOR Node",
    "GROUP Node",
    "EXPRESSION Node",
    "IF Node",
    "ELSE Node",
    "FOR Node",
    "BLOCK Node"
};


#endif