# Compiler and flags
CC = gcc

# Unity test framework path
UNITY_PATH = lib/unity

# --- DIRECTORIES ---
EXEC = $(MAIN_MODULE)
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test/unit
TEST_BIN_DIR = $(BUILD_DIR)/bin/test

# --- COMPILER FLAGS ---
CFLAGS = -g -Wall -std=c11 -I$(INCLUDE_DIR) -I$(SRC_DIR) -I$(UNITY_PATH)/src -MMD -MP
CFLAGS_TEST = $(CFLAGS) -DTESTING

LDFLAGS =

# --- PROJECT MODULES ---
MAIN_MODULE = core
SRC_MODULES = memory reader config core tokens
TEST_MODULES = memory reader config tokens tokenstryparsemain # Adicione 'options' se tiver testes para ele

# --- SOURCES & OBJECTS ---

# Main source files and objects
MODULE_PATHS = $(patsubst %,$(SRC_DIR)/%,$(SRC_MODULES))
SOURCES = $(foreach dir,$(MODULE_PATHS),$(shell find $(dir) -name '*.c'))
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)

# Test source files and objects
TEST_LIB_PATHS = $(patsubst %,$(SRC_DIR)/%,$(TEST_MODULES))
TEST_LIB_SOURCES = $(foreach dir,$(TEST_LIB_PATHS),$(shell find $(dir) -name '*.c'))
TEST_LIB_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(TEST_BIN_DIR)/%.o,$(TEST_LIB_SOURCES))

TEST_RUNNER_SRCS = $(patsubst %,$(TEST_DIR)/test_%.c,$(TEST_MODULES))
TEST_RUNNER_OBJS = $(patsubst $(TEST_DIR)/%.c,$(TEST_BIN_DIR)/%.o,$(TEST_RUNNER_SRCS))

UNITY_SRC = $(UNITY_PATH)/src/unity.c
UNITY_OBJ = $(TEST_BIN_DIR)/unity.o

TEST_ALL_OBJECTS = $(TEST_RUNNER_OBJS) $(TEST_LIB_OBJECTS)
TEST_DEPS = $(TEST_ALL_OBJECTS:.o=.d) $(UNITY_OBJ:.o=.d)

# --- DEFAULT TARGETS ---

all: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Linking complete. Executable '$(EXEC)' is at $@"

# --- TEST TARGETS ---

TEST_EXECS = $(patsubst %,$(TEST_BIN_DIR)/test_%,$(TEST_MODULES))

test: $(TEST_EXECS)
	@echo "--- Running All Tests ---"
	@echo "Test executables: $(TEST_EXECS)"
	@$(foreach exec,$(TEST_EXECS), echo "▶️  Running $(exec)"; $(exec) || exit 1;)
	@echo "✅ --- All Tests Finished Successfully ---"

# --- COMPILATION RULES ---

# Compile main sources
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< into $@"

# Compile test library sources with TESTING flag
$(TEST_BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS_TEST) -c $< -o $@
	@echo "Compiled Test Lib $< into $@"

# Compile test runner sources with TESTING flag
$(TEST_BIN_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS_TEST) -c $< -o $@
	@echo "Compiled Test Runner $< into $@"

# Compile Unity framework
$(UNITY_OBJ): $(UNITY_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled Unity Framework $< into $@"

# --- LINKING TEST EXECUTABLES ---

$(TEST_BIN_DIR)/test_%: $(TEST_BIN_DIR)/test_%.o $(TEST_LIB_OBJECTS) $(UNITY_OBJ)
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Linked test executable $@"

# --- HOUSEKEEPING ---

clean:
	@echo "Cleaning up build files..."
	rm -rf $(BUILD_DIR)

.PHONY: all clean test

# Include dependency files
-include $(DEPS) $(TEST_DEPS)
