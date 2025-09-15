#ifndef READER_H
#define READER_H

#include <stdio.h>  // Required for the FILE type
#include <stddef.h> // Required for the size_t type

#define INITIAL_BUFFER_SIZE 256

/**
 * @brief Holds the internal state for the file reader.
 *
 * This struct should be treated as an opaque type; its members should
 * only be accessed by the file_reader_* functions.
 */
typedef struct {
    FILE *file;
    char *buffer;
    size_t buffer_size;
} FileReader;


// --- Function Prototypes ---
FileReader *file_reader_create(const char *filename);
char *file_reader_get_next_line(FileReader *fr);
void file_reader_free_line(char *line);
void file_reader_destroy(FileReader *fr);
int reset_buffer_size(FileReader *fr);


#endif // READER_H