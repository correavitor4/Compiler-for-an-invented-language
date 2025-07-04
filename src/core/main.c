#include <stdio.h>
#include <stdlib.h>
// #include "../config/options.h"
#include "../reader/reader.h"

int main(int argc, char *argv[])
{

    // NOTE: Movi a implementação da leitura do lex para aqui só para testar o FileReader.

    FileReader *fr = file_reader_create("/data/Workspace/cpp/src/files/file1.txt");

    if (!fr)
    {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", "file1.txt");
        return 2;
    }

    printf("Reading file: %s\n", "./file1.txt");

    // Lê linha a linha
    char *line;
    while ((line = file_reader_get_next_line(fr)) != NULL)
    {
        printf("Lendo linha: %s\n", line);

        file_reader_free_line(line); // Libera a linha lida
    }

    file_reader_destroy(fr);

    return 0;
}