#ifndef AST_NODE_TYPES_H
#define AST_NODE_TYPES_H

typedef enum {
    AST_ROOT_NODE, 
    AST_MAIN_FUNCTION_NODE,
    AST_FUNCTION_DECLARATION_NODE,
    AST_INT_VARIABLE_DECLARATION_NODE,
    AST_TEXT_VARIABLE_DECLARATION_NODE,
    AST_VARIABLE_REF_NODE,
    AST_FUNCTION_CALL_NODE,
    AST_ASSIGNMENT_NODE,
    AST_INT_LITERAL_NODE,
    AST_DECIMAL_LITERAL_NODE,
    AST_TEXT_LITERAL_NODE,
    AST_ASSIGNMENT_TEMPORARY_NODE, // New node type for temporary assignment (can be used for multi-assignments)
    AST_EXPRESSION_NODE, // New node type for expressions
    AST_IF_NODE, // New node type for if statements
} ASTNodeType;

static const char *ASTNodeTypeStrings[] = {
    "Root Node",
    "Main Function Node",
    "Function Declaration Node",
    "Int Variable Declaration Node",
    "Text Variable Declaration Node",
    "Variable Reference Node",
    "Function Call Node",
    "Assignment Node",
    "Int Literal Node",
    "Decimal Literal Node",
    "Text Literal Node",
    "Assignment Temporary Node",
    "Expression Node",
    "IF Node",
};


#endif