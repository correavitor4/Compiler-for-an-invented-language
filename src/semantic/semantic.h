#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>
#include "symbol_table/symbol_table.h"
#include "ast/ast.h"

TokenType navigate_ast(ASTNode* node, ScopeManager* sm);

#endif // SEMANTIC_H