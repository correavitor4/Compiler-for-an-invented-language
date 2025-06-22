#include "reader.h"
#include <string.h>
#include "../memory/memory_controller.h"

FileReader *filereader_create(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return NULL;

    FileReader *fr = allocate_memory(sizeof(FileReader));

    if (!fr)
    {
        fclose(f);
        return NULL;
    }

    fr->file = f;
    fr->buffer_size = INITIAL_BUFFER_SIZE;
    fr->buffer = allocate_memory(fr->buffer_size);
    if (!fr->buffer)
    {
        fclose(f);
        free_memory(fr);
        return NULL;
    }

    // Aqui estou assegurando que o buffer também esteja contando no limitador de memória
    return fr;
}

char *filereader_get_next_line(FileReader *fr)
{
    if (!fr || !fr->file)
        return NULL;

    // Resetando o buffer para o tamanho inicial
    reset_buffer_size_to_initial(fr);

    size_t pos = 0;
    int c;

    while ((c = fgetc(fr->file)) != EOF)
    {
        // Expande buffer se necessário
        if (pos + 1 >= fr->buffer_size)
        {
            size_t new_size = fr->buffer_size * 2;
            char *new_buf = reallocate_memory(fr->buffer, new_size);
            if (!new_buf)
                return NULL;
            fr->buffer = new_buf;
            fr->buffer_size = new_size;

            // Aqui estou assegurando que o buffer também esteja contando no limitador de memória
            fr->buffer_size = new_size;
        }
        fr->buffer[pos++] = (char)c;
        if (c == '\n')
            break;
    }

    // EOF no início, então o buffer deve ser liberado em memória
    if (pos == 0)
    {
        free_memory(fr->buffer);
        fr->buffer = NULL;
        fr->buffer_size = 0;
        return NULL;
    }

    fr->buffer[pos] = '\0';
    return fr->buffer;
}

int reset_buffer_size_to_initial(FileReader *fr)
{
    if (!fr)
        return -1;

    if (fr->buffer)
    {
        // Se o buffer já estiver no tamanho inicial, não há necessidade de realocar
        if (fr->buffer_size == INITIAL_BUFFER_SIZE)
            return 0;

        free_memory(fr->buffer);
    }

    fr->buffer = allocate_memory(INITIAL_BUFFER_SIZE);
    if (!fr->buffer)
    {
        fr->buffer_size = 0;
        return -1;
    }
    fr->buffer_size = INITIAL_BUFFER_SIZE;

    return 0;
}

void filereader_destroy(FileReader *fr)
{
    if (!fr)
        return;

    if (fr->file)
        fclose(fr->file);
    free_memory(fr->buffer);
    free_memory(fr);

    fr = NULL;
}
