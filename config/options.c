#include "options.h"
#include <stdio.h>
#include <string.h>

static char directory_path[256] = DIR_PADRAO; // Valor padrão

void read_directory_path() {
    char entrada[256];

    printf("Use o diretório padrão \"%s\"? (Enter (vazio) para sim, ou digite outro caminho): ", DIR_PADRAO);
    if (fgets(entrada, sizeof(entrada), stdin) != NULL) {
        size_t len = strlen(entrada);
        if (len > 0 && entrada[len - 1] == '\n') {
            entrada[len - 1] = '\0';
        }

        if (strlen(entrada) == 0) {
            strncpy(directory_path, DIR_PADRAO, sizeof(directory_path));
        } else {
            strncpy(directory_path, entrada, sizeof(directory_path));
        }
        directory_path[sizeof(directory_path) - 1] = '\0'; 
    }
}

void load_options() {
    read_directory_path();
}

const char* get_directory_path() {
    return directory_path;
}
