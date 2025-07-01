#include "lex.h"
#include "../memory/memory_controller.h"
#include "../reader/reader.h"
#include "../config/options.h"
#include <dirent.h>
#include <string.h>
#include "lex_errors.h"
#include "../tokens/tokens.h"

int tokens_count = 0; // número de tokens no vetor

// Declaração da função evaluate_token
int evaluate_token(const char *token) {
    return -1;
}

void lex_init()
{
}

// Expande o buffer do token se necessário
// Retorna 0 em caso de sucesso, -1 em caso de erro
// O buffer é realocado para o dobro do tamanho atual
// O ponteiro char_accumulator deve ser atualizado para apontar para o novo buffer
// O current_size deve ser atualizado para o novo tamanho do buffer
int expand_token_accumulator_buffer_size(char **char_accumulator, size_t *current_size)
{
    size_t new_size = *current_size * 2;
    char *new_buf = reallocate_memory(*char_accumulator, new_size);
    if (!new_buf)
    {
        fprintf(stderr, "Erro ao realocar memória para o token.\n");
        free_memory(*char_accumulator);
        return -1;
        TOKEN_ACCUMULATOR_MEMORY_ALLOCATION_ERROR;
    }
    *char_accumulator = new_buf;
    *current_size = new_size;
    return 0;
}

int evaluate_token_accumulator_buffer_size(char **char_accumulator, size_t *char_accumulator_size)
{
    if (*char_accumulator >= char_accumulator_size)
    {
        int res = expand_token_accumulator_buffer_size(char_accumulator, char_accumulator_size);
        if (res < 0)
        {
            return res;
        }
    }

    return 0;
}

// Ele vai verificar todos os arquivos .txt do diretório de arquivos
// e ler o conteúdo deles, armazenando os tokens encontrados
// no vetor de tokens. Retorna 0 em caso de sucesso
// Error codes:
// 1 - Erro ao ler o diretório
// 2 - Erro ao ler o arquivo
short read_files()
{
    const char *dir_path = get_directory_path();

    if (!dir_path)
    {
        fprintf(stderr, "Erro: Caminho do diretório não definido.\n");
        return 1;
    }

    DIR *dir = opendir(dir_path);
    if (!dir)
    {
        perror("Erro ao abrir diretório");
        return 1;
    }

    struct dirent *entry;

    // Buffer para armazenar o caminho completo do arquivo
    char filepath[1024];

    // Verifica se o diretório está vazio
    while ((entry = readdir(dir)) != NULL)
    {
        // Ignora "." e ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Verifica se termina com ".txt"
        size_t len = strlen(entry->d_name);
        if (len < 4 || strcmp(entry->d_name + len - 4, ".txt") != 0)
            continue;

        // Monta o caminho completo do arquivo
        snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, entry->d_name);

        // Cria FileReader
        FileReader *fr = file_reader_create(filepath);
        if (!fr)
        {
            fprintf(stderr, "Erro ao abrir arquivo: %s\n", filepath);
            closedir(dir);
            return 2;
        }

        // Lê linha a linha
        char *line;
        while ((line = filereader_get_next_line(fr)) != NULL)
        {
            printf("Lendo linha: %s\n", line);
        }
        
        filereader_destroy(fr);
    }

    closedir(dir);
    return 0;
}

// Função para processar uma linha e gerar tokens
// void lex_line(const char *line)
int lex_line(const char *line, int line_length)
{
}

// Pega o próximo token da linha e chama uma função para processá-lo
// Atualiza o current_position para a posição do próximo token
// Retorna 0 em caso de sucesso.
// Em caso de erro, ele retorna um código de erro negativo.
int next_token(const char *line, int *current_position)
{

    size_t char_accumulator_size = get_initial_lex_token_accumulator_size();
    char *char_accumulator = allocate_memory(char_accumulator_size);

    size_t char_accumulator_index = 0;
    while (line[*current_position] != '\0' && line[*current_position] != ' ' && line[*current_position] != '\n')
    {
        if (char_accumulator_index >= char_accumulator_size)
        {
            evaluate_token_accumulator_buffer_size(&char_accumulator, &char_accumulator_size);
        }
        char_accumulator[char_accumulator_index] = line[*current_position];
        char_accumulator_index++;
        (*current_position)++;
    }
    char_accumulator[char_accumulator_index] = '\0';

    int token_evaluation_result = evaluate_token(char_accumulator);
    if (token_evaluation_result < 0)
    {
        return token_evaluation_result;
    }

    return 0;
}

int evaluate_accumulator(const char *char_accumulator, int char_accumulator_size)
{
    if (char_accumulator_size == 0)
        return 0;
}