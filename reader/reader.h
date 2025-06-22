#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_BUFFER_SIZE 1024

// Aparentemente o ponteiro File é gerenciado internamente pelo sistema
// e o tamanho dele não é acessível diretamente. Sendo assim, não contarei]
// o tamanho do ponteiro File no controle de memória, apenas o buffer alocado
typedef struct {
    FILE *file;
    char *buffer;
    size_t buffer_size;
} FileReader;

// Cria um FileReader para o arquivo dado, retorna NULL em erro
FileReader *filereader_create(const char *filename);

// Lê a próxima linha do arquivo, retorna NULL no fim do arquivo ou erro
// A linha retornada está em buffer interno, válida até próxima chamada
char *filereader_get_next_line(FileReader *fr);

// Libera recursos do FileReader
void filereader_destroy(FileReader *fr);

int reset_buffer_size_to_initial(FileReader *fr);

#endif
