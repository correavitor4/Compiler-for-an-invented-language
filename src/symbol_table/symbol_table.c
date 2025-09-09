#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash/hash.h"
#include "memory/memory_controller.h"
#include "utils/string/string_utils.h"
#include "./symbol_table.h"

// --- Configuration ---
#define INITIAL_SYMBOL_TABLE_SIZE 64
#define INITIAL_ALL_SCOPES_SIZE 32
#define LOAD_FACTOR_THRESHOLD 0.75

// --- Struct Definitions ---

// Represents a single symbol table for one scope. Now fully dynamic.
typedef struct SymbolTable {
    Symbol **table; // Pointer to the array of symbols
    int size;       // Current capacity of the table
    int count;      // Number of items currently in the table
} SymbolTable;

// Represents a scope, containing a symbol table and a link to its parent
typedef struct Scope {
    unsigned long id;          // The hashed key of this scope
    struct Scope *parent;      // Pointer to the parent scope
    SymbolTable *symbol_table; // The actual table of symbols
    struct Scope *next;        // For collision handling in all_scopes
} Scope;

// The main manager. Now fully dynamic.
struct ScopeManager {
    Scope **all_scopes; // Hash table of all scopes
    int size;           // Current capacity of the all_scopes table
    int count;          // Number of scopes created
    Scope *current_scope; // Direct pointer to the current active scope
};


// --- Internal Function Prototypes ---
static SymbolTable* symbol_table_create_internal();
static void symbol_table_destroy_internal(SymbolTable *st);
static void symbol_table_resize(SymbolTable *st);
static int symbol_table_insert_internal(SymbolTable *st, char *name, SymbolKind kind, TokenType type);
static Symbol* symbol_table_lookup_internal(SymbolTable *st, char *name);
static void scope_manager_resize(ScopeManager *sm);


// --- Scope Manager Functions ---

ScopeManager* scope_manager_create() {
    ScopeManager *sm = allocate_memory(sizeof(ScopeManager));
    
    // MODIFIED: Initialize dynamic members
    sm->size = INITIAL_ALL_SCOPES_SIZE;
    sm->count = 0;
    sm->all_scopes = allocate_memory(sizeof(Scope*) * sm->size);
    sm->current_scope = NULL;
    
    for (int i = 0; i < sm->size; i++) {
        sm->all_scopes[i] = NULL;
    }

    scope_manager_enter_scope(sm, "global");
    return sm;
}

void scope_manager_destroy(ScopeManager *sm) {
    if (!sm) return;
    for (int i = 0; i < sm->size; i++) {
        Scope *current = sm->all_scopes[i];
        while (current) {
            Scope *temp = current;
            current = current->next;
            symbol_table_destroy_internal(temp->symbol_table);
            free_memory(temp);
        }
    }
    // MODIFIED: Free the table itself
    free_memory(sm->all_scopes);
    free_memory(sm);
}

void scope_manager_enter_scope(ScopeManager *sm, const char *key) {
    // Check load factor and resize if necessary before any calculations
    float current_load_factor = (float)sm->count / sm->size;
    if (current_load_factor > LOAD_FACTOR_THRESHOLD) {
        scope_manager_resize(sm);
    }

    
    unsigned long id = HASH.djb2(key);
    unsigned long index = id % sm->size;

    // This loop traverses the linked list at the correct index (handles collisions).
    for (Scope *scope = sm->all_scopes[index]; scope != NULL; scope = scope->next) {
        if (scope->id == id) {
            // Scope already exists, just set it as current and return.
            sm->current_scope = scope;
            return;
        }
    }

    // If the loop finishes, the scope does not exist. Create a new one.
    Scope *new_scope = allocate_memory(sizeof(Scope));
    new_scope->id = id;
    new_scope->symbol_table = symbol_table_create_internal();
    new_scope->parent = sm->current_scope;
    new_scope->next = NULL;

    // Insert the new scope at the front of the list at the calculated index.
    if (sm->all_scopes[index] != NULL) {
        new_scope->next = sm->all_scopes[index];
    }
    sm->all_scopes[index] = new_scope;
    
    // The new scope becomes the current one.
    sm->current_scope = new_scope;
    
    sm->count++;
}


void scope_manager_exit_scope(ScopeManager *sm) {
    if (sm->current_scope && sm->current_scope->parent) {
        sm->current_scope = sm->current_scope->parent;
    }
}

int scope_manager_insert(ScopeManager *sm, char *name, SymbolKind kind, TokenType type) {
    if (!sm->current_scope) return -1;
    return symbol_table_insert_internal(sm->current_scope->symbol_table, name, kind, type);
}

Symbol* scope_manager_lookup(ScopeManager *sm, char *name) {
    for (Scope *scope = sm->current_scope; scope != NULL; scope = scope->parent) {
        Symbol *found = symbol_table_lookup_internal(scope->symbol_table, name);
        if (found) {
            return found;
        }
    }
    return NULL;
}

// NEW: Resize function for the Scope Manager
static void scope_manager_resize(ScopeManager *sm) {
    int old_size = sm->size;
    Scope **old_scopes = sm->all_scopes;

    int new_size = old_size * 2;
    Scope **new_scopes = allocate_memory(sizeof(Scope*) * new_size);
    for (int i = 0; i < new_size; i++) {
        new_scopes[i] = NULL;
    }

    // Rehash all existing scopes
    for (int i = 0; i < old_size; i++) {
        Scope *current = old_scopes[i];
        while (current) {
            Scope *next = current->next;
            // Recalculate index based on the scope's stored ID and new size
            unsigned long new_index = current->id % new_size;
            // Insert into new table
            current->next = new_scopes[new_index];
            new_scopes[new_index] = current;
            current = next;
        }
    }

    free_memory(old_scopes);
    sm->all_scopes = new_scopes;
    sm->size = new_size;
}

// --- Internal Symbol Table Functions ---

static SymbolTable* symbol_table_create_internal() {
    SymbolTable *st = allocate_memory(sizeof(SymbolTable));
    // MODIFIED: Initialize dynamic members
    st->size = INITIAL_SYMBOL_TABLE_SIZE;
    st->count = 0;
    st->table = allocate_memory(sizeof(Symbol*) * st->size);
    for (int i = 0; i < st->size; i++) {
        st->table[i] = NULL;
    }
    return st;
}

static void symbol_table_destroy_internal(SymbolTable *st) {
    if (!st) return;
    for (int i = 0; i < st->size; i++) {
        Symbol *current = st->table[i];
        while (current) {
            Symbol *temp = current;
            current = current->next;
            free_memory(temp->name);
            if (temp->params) {
                Param *param = temp->params;
                while (param) {
                    Param *next = param->next;
                    free_memory(param);
                    param = next;
                }
            }
            free_memory(temp);
        }
    }
    // MODIFIED: Free the table itself
    free_memory(st->table);
    free_memory(st);
}

static int symbol_table_insert_internal(SymbolTable *st, char *name, SymbolKind kind, TokenType type) {
    // MODIFIED: Check load factor and resize if necessary
    float current_load_factor = (float)st->count / st->size;
    if (current_load_factor > LOAD_FACTOR_THRESHOLD) {
        symbol_table_resize(st);
    }
    
    unsigned long index = HASH.djb2(name) % st->size;
    for (Symbol *current = st->table[index]; current != NULL; current = current->next) {
        if (strcmp(current->name, name) == 0) return 0;
    }
    
    Symbol *new_symbol = allocate_memory(sizeof(Symbol));

    new_symbol->name = strdup(name);
    new_symbol->type = type;
    new_symbol->kind = kind;
    new_symbol->next = st->table[index];
    new_symbol->params = NULL;

    st->table[index] = new_symbol;
    st->count++;
    return 1;
}

static Symbol* symbol_table_lookup_internal(SymbolTable *st, char *name) {
    unsigned long index = HASH.djb2(name) % st->size;
    for (Symbol *current = st->table[index]; current != NULL; current = current->next) {
        if (strcmp(current->name, name) == 0) return current;
    }
    return NULL;
}

// NEW: Resize function for a single Symbol Table
static void symbol_table_resize(SymbolTable *st) {
    int old_size = st->size;
    Symbol **old_table = st->table;
    
    int new_size = old_size * 2;
    Symbol **new_table = allocate_memory(sizeof(Symbol*) * new_size);
    for (int i = 0; i < new_size; i++) {
        new_table[i] = NULL;
    }
    
    // Rehash all existing symbols
    for (int i = 0; i < old_size; i++) {
        Symbol *current = old_table[i];
        while (current) {
            Symbol *next = current->next;
            // Recalculate index with the new size
            unsigned long new_index = HASH.djb2(current->name) % new_size;
            // Insert into new table
            current->next = new_table[new_index];
            new_table[new_index] = current;
            current = next;
        }
    }
    
    free_memory(old_table);
    st->table = new_table;
    st->size = new_size;
}

void bind_function_params(Symbol *func_symbol, Param *params) {
    if (!func_symbol || func_symbol->kind != KIND_FUNC) return;

    
    func_symbol->params = params;
}