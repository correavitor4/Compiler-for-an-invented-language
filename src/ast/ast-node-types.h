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
} ASTNodeType;

static const char *ASTNodeTypeStrings[] = {
    "Root Node",
    "Main Function Node",
    "Function Declaration Node",
    "Int Variable Declaration Node",
    "Text Variable Declaration Node",
    "Variable Reference Node",
    "Function Call Node"
};


#endif