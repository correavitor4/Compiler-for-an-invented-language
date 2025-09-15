#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include "memory/memory_controller.h"
#include "ast-node-types.h"
#include <time.h>


void printDot(ASTNode *node, FILE *file) {
    if (!node) return;

    const char *typeStr = "UNKNOWN";
    if (node->type >= 0 && node->type < sizeof(ASTNodeTypeStrings)/sizeof(ASTNodeTypeStrings[0])) {
        typeStr = ASTNodeTypeStrings[node->type];
    }

    fprintf(file, "  \"%p\" [label=\"%s\\nType: %s\\nLine: %d\"];\n",
            (void*)node,
            node->literal ? node->literal : "NULL",
            typeStr,
            node->line_num);

    for (int i = 0; i < node->child_count; i++) {
        ASTNode *child = node->children[i];
        if (child) {
            fprintf(file, "  \"%p\" -> \"%p\";\n", (void*)node, (void*)child);
            printDot(child, file);
        }
    }
}


// Função auxiliar para criar diretórios recursivamente (similar ao mkdir -p)
int mkdir_recursive(const char *dir) {
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') {
        tmp[len - 1] = '\0';
    }
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0777) != 0) {
                if (errno != EEXIST) {
                    return -1;  // erro ao criar diretório
                }
            }
            *p = '/';
        }
    }
    if (mkdir(tmp, 0777) != 0) {
        if (errno != EEXIST) {
            return -1;
        }
    }
    return 0;
}

// Função para gerar o arquivo dot completo dado o nó raiz
void generate_dot_file(ASTNode *root) {
    const char *dir_path = "generated-diagrams/AST";
    if (mkdir_recursive(dir_path) != 0) {
        perror("Erro ao criar diretórios");
        return;
    }

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char* filename = allocate_memory(64 * sizeof(char)); // "DD_MM_YYYY_HH_MM_SS.dot" + null terminator
    strftime(filename, 64, "%d_%m_%Y_%H_%M_%S.dot", t);

    char filepath[256]; // Corrigido aqui
    snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, filename);

    FILE *file = fopen(filepath, "w");
    if (!file) {
        perror("Erro ao abrir arquivo para escrita");
        return;
    }

    fprintf(file, "digraph AST {\n");
    fprintf(file, "  node [shape=box, style=filled, color=\"#E0E0E0\"];\n");
    printDot(root, file);
    fprintf(file, "}\n");
    fclose(file);

    free_memory(filename);
}


