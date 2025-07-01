#include "options.h"
#include <stdio.h>
#include <string.h>

static char directory_path[256] = "files/";

// I created a custom fgets function to "mock" the fgets function for testing purposes.
#ifndef TESTING
char *my_fgets(char *str, int num, FILE *stream)
{
    return fgets(str, num, stream);
}
#endif

void read_directory_path()
{
    char entrada[256];

    printf("Use o diretório padrão \"%s\"? (Enter (vazio) para sim, ou digite outro caminho): ", directory_path);
    if (my_fgets(entrada, sizeof(entrada), stdin) != NULL)
    {
        size_t len = strlen(entrada);
        if (len > 0 && entrada[len - 1] == '\n')
        {
            entrada[len - 1] = '\0';
        }

        if (strlen(entrada) > 0)
        {
            strncpy(directory_path, entrada, sizeof(directory_path));
        }
        directory_path[sizeof(directory_path) - 1] = '\0';
    }
}
void load_options()
{
    read_directory_path();
}

const char *get_directory_path()
{
    return directory_path;
}

void set_directory_path(const char *path)
{
    strncpy(directory_path, path, sizeof(directory_path) - 1);
    directory_path[sizeof(directory_path) - 1] = '\0';
}



// Lex options
int initial_lex_token_accumulator_size = 64;
// TODO: Must be tested
int get_initial_lex_token_accumulator_size()
{
    return initial_lex_token_accumulator_size;
}