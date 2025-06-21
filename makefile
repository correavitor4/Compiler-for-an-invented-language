# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11

# Diretórios
SRC_DIR = .
CONFIG_DIR = config
READER_DIR = reader
TESTS_DIR = tests
UNITY_DIR = $(TESTS_DIR)/libs/unity/src
UNIT_TESTS_DIR = $(TESTS_DIR)/unit/options

# Fontes principais do programa
SRC_FILES = $(SRC_DIR)/main.c \
            $(CONFIG_DIR)/options.c \
            $(READER_DIR)/reader.c

# Arquivos de teste unitário
UNIT_TEST_FILES = $(UNIT_TESTS_DIR)/options_unit_tests.c

# Arquivo Unity
UNITY_SRC = $(UNITY_DIR)/unity.c

# Executáveis
MAIN_EXE = main
TEST_EXE = test_options

.PHONY: all build test clean

all: build

build: $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(MAIN_EXE) $(SRC_FILES)

test: $(UNIT_TEST_FILES) $(CONFIG_DIR)/options.c $(UNITY_SRC)
	$(CC) $(CFLAGS) -DTESTING -I$(UNITY_DIR) -I$(CONFIG_DIR) -o $(TEST_EXE) $(UNIT_TEST_FILES) $(CONFIG_DIR)/options.c $(UNITY_SRC)
	./$(TEST_EXE)

clean:
	rm -f $(MAIN_EXE) $(TEST_EXE)
