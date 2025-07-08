#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>

void read_directory_path();
void load_options();
const char* get_directory_path();
void set_directory_path(const char* path);
char* my_fgets(char* str, int num, FILE* stream);

// Lex options
int get_initial_lex_token_accumulator_size();
#endif