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
    AST_IF_NODE, // New node type for if statements,
    AST_FOR_NODE, // New node type for for loops,
    AST_FOR_STATEMENT_NODE,
    AST_FOR_STATEMENT_INITIALIZATION_NODE, // ( {non empty initialization }; ...; ...) for loop
    AST_FOR_STATEMENT_EMPTY_INITIALIZATION_NODE, // ( {empty initialization} ; ...; ...) for loop with empty initialization (x is empty)
    AST_FOR_STATEMENT_CONDITION_NODE, // (...; {condition}; ...) for loop
    AST_FOR_STATEMENT_EMPTY_CONDITION_NODE, // (...; {empty condition}; ...) for loop with empty condition (x is empty)
    AST_FOR_STATEMENT_CONTROL_NODE, // (...; ...; {control}) for loop
    AST_FOR_STATEMENT_EMPTY_CONTROL_NODE, // (...; ...; {empty control}) for loop with empty control (x is empty)
    AST_BLOCK_NODE, // New node type for blocks (enclosed in { } )
    AST_IF_BLOCK_NODE, // New node type for if blocks (enclosed in { } )
    AST_IF_INLINE_EXECUTION_CODE_NODE, // New node type for if inline execution code (not enclosed in { } )
    AST_ELSE_EXCUTION_CODE_BLOCK_NODE, // New node type for else execution code (not enclosed in { } )
    AST_ELSE_INLINE_EXECUTION_CODE_NODE, // New node type for else inline execution code (not enclosed in {
    AST_IF_STATEMENT_EXPRESSION_NODE, // New node type for if statement expressions
    AST_DECIMAL_VARIABLE_DECLARATION_NODE, // New node type for decimal variable declarations
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
    "FOR Node",
    "FOR Statement Node",
    "FOR Statement Initialization Node",
    "FOR Statement Empty Initialization Node",
    "FOR Statement Condition Node",
    "FOR Statement Empty Condition Node",
    "FOR Statement Control Node",
    "FOR Statement Empty Control Node",
    "Block Node",
    "IF Block Node",
    "IF Inline Execution Code Node",
    "ELSE Execution Code Block Node",
    "ELSE Inline Execution Code Node",
    "IF Statement Expression Node",
    "Decimal Variable Declaration Node",
};


#endif