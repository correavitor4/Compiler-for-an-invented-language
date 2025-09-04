#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "lex/lex.h" 

typedef struct Param Param;

typedef enum {
    KIND_VAR,
    KIND_FUNC
} SymbolKind;

typedef struct Symbol {
    char *name;
    SymbolKind kind;
    TokenType type;
    Param *params;    
    struct Symbol *next;
} Symbol;

struct Param {
    Symbol *symbol;
    struct Param *next;
};

typedef struct ScopeManager ScopeManager;

ScopeManager* scope_manager_create();

/**
 * @brief Destroys the given ScopeManager instance and frees associated resources.
 *
 * This function should be called when the ScopeManager is no longer needed to
 * release any memory or resources allocated during its lifetime.
 *
 * @param sm Pointer to the ScopeManager to be destroyed.
 */
void scope_manager_destroy(ScopeManager *sm);
/**
 * @brief Enters a new scope in the given ScopeManager.
 *
 * This function creates a new scope context within the provided ScopeManager,
 * allowing for the management of symbols and variables with proper scoping rules.
 *
 * @param sm Pointer to the ScopeManager instance where the new scope will be entered.
 */
void scope_manager_enter_scope(ScopeManager *sm, const char *key);
/**
 * @brief Exits the current scope managed by the given ScopeManager.
 *
 * This function updates the ScopeManager to remove or deactivate the current scope,
 * typically restoring the previous scope context. It is used to manage symbol visibility
 * and lifetimes in a scoped environment, such as a compiler or interpreter.
 *
 * @param sm Pointer to the ScopeManager instance whose current scope should be exited.
 */
void scope_manager_exit_scope(ScopeManager *sm);
/**
 * @brief Inserts a new symbol into the current scope managed by the ScopeManager.
 *
 * This function adds a symbol with the specified name, kind, and type to the current scope.
 *
 * @param sm Pointer to the ScopeManager instance managing the scopes.
 * @param name The name of the symbol to insert.
 * @param kind The kind of the symbol (e.g., variable, function).
 * @param type The token type associated with the symbol.
 * @return int Returns 0 on success, or a negative value on failure (e.g., if the symbol already exists).
 */
int scope_manager_insert(ScopeManager *sm, char *name, SymbolKind kind, TokenType type);
/**
 * Looks up a symbol by name within the current scope managed by the given ScopeManager.
 *
 * @param sm    Pointer to the ScopeManager instance managing the scopes.
 * @param name  The name of the symbol to look up.
 * @return      Pointer to the Symbol if found, or NULL if not found.
 */
Symbol* scope_manager_lookup(ScopeManager *sm, char *name);

void bind_function_params(Symbol *func_symbol, Param *params) ;

#endif // SYMBOL_TABLE_H