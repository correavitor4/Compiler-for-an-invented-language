#include "reader.h"
#include <string.h>
#include "../memory/memory_controller.h" // Assuming this handles allocate/reallocate/free

/**
 * @brief Creates and initializes a new FileReader instance.
 * @param filename The path to the file to be read.
 * @return A pointer to the new FileReader on success, or NULL on failure.
 */
FileReader *file_reader_create(const char *filename) {
    // 1. Allocate memory for the FileReader struct itself.
    FileReader *fr = allocate_memory(sizeof(FileReader));
    if (!fr) {
        return NULL;
    }

    // 2. Open the file.
    fr->file = fopen(filename, "r");
    if (!fr->file) {
        free_memory(fr); // Clean up allocated struct on failure.
        return NULL;
    }

    // 3. Allocate the initial internal buffer.
    fr->buffer_size = INITIAL_BUFFER_SIZE;
    fr->buffer = allocate_memory(fr->buffer_size);
    if (!fr->buffer) {
        fclose(fr->file);
        free_memory(fr);
        return NULL;
    }

    return fr;
}

/**
 * @brief Reads the next line from the file.
 * @param fr A pointer to the FileReader instance.
 * @return A dynamically allocated string with the line's content.
 * The CALLER IS RESPONSIBLE for freeing this memory.
 * Returns NULL at the end of the file or on error.
 */
char *file_reader_get_next_line(FileReader *fr) {
    if (!fr || !fr->file) {
        return NULL;
    }

    size_t pos = 0;
    int c;

    // Read characters into the internal buffer until newline or EOF.
    while ((c = fgetc(fr->file)) != EOF) {
        // Expand the internal buffer if it's full (-1 to save space for '\0').
        if (pos >= fr->buffer_size - 1) {
            size_t new_size = fr->buffer_size * 2;
            char *new_buf = reallocate_memory(fr->buffer, new_size);
            
            if (!new_buf) {
                // On reallocation failure, return NULL to signal a critical error.
                return NULL;
            }
            fr->buffer = new_buf;
            fr->buffer_size = new_size;
        }

        fr->buffer[pos++] = (char)c;
        if (c == '\n') {
            break;
        }
    }

    // If no characters were read (EOF at the beginning of a call), we are done.
    if (pos == 0) {
        return NULL;
    }

    // Null-terminate the string in the internal buffer.
    fr->buffer[pos] = '\0';

    char *line = allocate_memory(pos + 1); // This decouples the returned pointer from the FileReader's internal state, to prevent dangling pointers.

    if (!line) {
        return NULL; // Memory allocation failed.
    }
    
    // Copy the content from the internal buffer to the new string.
    memcpy(line, fr->buffer, pos + 1);

    return line; // Return the safe, newly allocated line.
}

/**
 * @brief Frees the FileReader line and its resources.
 * @param fr A pointer to the FileReader instance to be freed.
 * This function should be called when the going to the next line.
 */
void file_reader_free_line(char *line) {
    if (line) {
        free_memory(line); // Free the line memory allocated by file_reader_get_next_line.
    }
}

/**
 * @brief Closes the file and frees all memory used by the FileReader.
 * @param fr A pointer to the FileReader instance to be destroyed.
 */
void file_reader_destroy(FileReader *fr) {
    if (!fr) {
        return;
    }
    if (fr->file) {
        fclose(fr->file);
    }
    // Free the internal buffer and the struct itself.
    free_memory(fr->buffer);
    free_memory(fr);
}