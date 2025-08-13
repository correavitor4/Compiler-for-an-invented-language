#include "parse-tree.h"
#include "memory/memory_controller.h"
#include <stdbool.h>
#include "reader/reader.h"

bool has_main_function_declared = false;

ParseTreeNode *ParseTree(const char *file_name){
    FileReader *fr = file_reader_create(file_name);
    if (!fr) {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", file_name);
        return NULL;
    }

    for(;;){
        char *line = file_reader_get_next_line(fr);
    }
}