#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash/hash.h"
#include "memory/memory_controller.h"
#include "./symbol_table.h"

#define SYMBOL_TABLE_SIZE 512
#define MAX_SCOPE_DEPTH 8 


typedef struct SymbolTable {
    Symbol *table[SYMBOL_TABLE_SIZE];
} SymbolTable;

struct ScopeManager {
    SymbolTable *scope_stack[MAX_SCOPE_DEPTH];
    int current_scope_level;
};

/**
 * @brief Creates and initializes a new internal SymbolTable instance.
 *
 * This function is intended for internal use within the symbol table module.
 * It allocates memory for a SymbolTable structure and sets up its initial state.
 *
 * @return Pointer to the newly created SymbolTable, or NULL on failure.
 */
static SymbolTable* symbol_table_create_internal() { 
    SymbolTable *st = allocate_memory(sizeof(SymbolTable));
    if (!st) return NULL;
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        st->table[i] = NULL;
    }
    return st;
 }

/**
 * @brief Destroys the internal data structures of the given symbol table.
 *
 * This function is intended for internal use only. It releases all resources
 * associated with the symbol table, including memory allocated for symbols and
 * any auxiliary data structures. The symbol table pointer itself is not freed.
 *
 * @param st Pointer to the SymbolTable to be destroyed.
 */
static void symbol_table_destroy_internal(SymbolTable *st) {
    if (!st) return;
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        Symbol *current = st->table[i];
        while (current != NULL) {
            Symbol *temp = current;
            current = current->next;
            free_memory(temp->name);
            free_memory(temp);
        }
    }
    free_memory(st);
}

/**
 * @brief Creates and initializes a new ScopeManager instance.
 *
 * This function allocates memory for a ScopeManager structure and sets up
 * its initial state. The ScopeManager is used to manage scopes in the symbol table,
 * allowing for tracking of variable and symbol definitions across different scopes.
 *
 * @return Pointer to the newly created ScopeManager instance, or NULL if allocation fails.
 */
ScopeManager* scope_manager_create() {
    ScopeManager *sm = allocate_memory(sizeof(ScopeManager));
    sm->current_scope_level = -1; 
    scope_manager_enter_scope(sm); 
    return sm;
}

/**
 * @brief Destroys the given ScopeManager instance and frees associated resources.
 *
 * This function should be called when the ScopeManager is no longer needed.
 * It will release all memory and resources allocated for managing scopes.
 *
 * @param sm Pointer to the ScopeManager to be destroyed.
 */
void scope_manager_destroy(ScopeManager *sm) {
    while (sm->current_scope_level >= 0) {
        scope_manager_exit_scope(sm);
    }
    free_memory(sm);
}

/**
 * @brief Enters a new scope in the scope manager.
 *
 * This function creates and enters a new scope within the provided ScopeManager instance.
 * It is typically used to manage symbol tables or variable lifetimes in a compiler or interpreter.
 *
 * @param sm Pointer to the ScopeManager structure.
 */
void scope_manager_enter_scope(ScopeManager *sm) {
    if (sm->current_scope_level >= MAX_SCOPE_DEPTH - 1) {
        fprintf(stderr, "Erro: Profundidade máxima de escopo excedida.\n");
        exit(EXIT_FAILURE);
    }
    sm->current_scope_level++;
    sm->scope_stack[sm->current_scope_level] = symbol_table_create_internal();
}

/**
 * @brief Exits the current scope in the scope manager.
 *
 * This function removes the top-most scope from the scope manager,
 * effectively reverting to the previous scope. It is typically used
 * when leaving a block or function to restore the symbol table to its
 * previous state.
 *
 * @param sm Pointer to the ScopeManager instance.
 */
void scope_manager_exit_scope(ScopeManager *sm) {
    if (sm->current_scope_level < 0) return;
    symbol_table_destroy_internal(sm->scope_stack[sm->current_scope_level]);
    sm->scope_stack[sm->current_scope_level] = NULL;
    sm->current_scope_level--;
}

/**
 * Inserts a new symbol into the current scope managed by the ScopeManager.
 *
 * @param sm    Pointer to the ScopeManager instance.
 * @param name  Name of the symbol to insert.
 * @param kind  Kind of the symbol (e.g., variable, function).
 * @param type  Token type representing the symbol's data type.
 * @return      0 on success, non-zero on failure (e.g., if the symbol already exists).
 */
int scope_manager_insert(ScopeManager *sm, const char *name, SymbolKind kind, TokenType type) {
    SymbolTable *current_scope = sm->scope_stack[sm->current_scope_level];
    unsigned long index = HASH.djb2(name) % SYMBOL_TABLE_SIZE;
    
    Symbol *current = current_scope->table[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 0;
        }
        current = current->next;
    }

    Symbol *new_symbol = allocate_memory(sizeof(Symbol));
    if (!new_symbol) return -1;

    new_symbol->name = name;
    if (!new_symbol->name) {
        free_memory(new_symbol);
        return -1;
    }

    new_symbol->type = type;
    new_symbol->kind = kind;
    new_symbol->next = current_scope->table[index];
    current_scope->table[index] = new_symbol;

    return 1; 
}

/**
 * Looks up a symbol by name in the current scope managed by the ScopeManager.
 *
 * @param sm    Pointer to the ScopeManager instance.
 * @param name  The name of the symbol to look up.
 * @return      Pointer to the Symbol if found, or NULL if not found.
 */
Symbol* scope_manager_lookup(ScopeManager *sm, const char *name) {
    
    for (int i = sm->current_scope_level; i >= 0; i--) {
        SymbolTable *scope = sm->scope_stack[i];
        
        unsigned long index = HASH.djb2(name) % SYMBOL_TABLE_SIZE;
        Symbol *current = scope->table[index];
        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                return current; 
            }
            current = current->next;
        }
    }
    return NULL; 
}
