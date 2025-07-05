# Compiler and flags
CC = gcc

# Define the path to the Unity test framework
# Ensure that the Unity framework is available in the specified path.
UNITY_PATH = lib/unity

# --- DIRECTORIES ---
# Project name and directories
EXEC = $(MAIN_MODULE)
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test/unit

# Use -g for debugging, -Wall for warnings.
# -Iinclude tells gcc to look for headers in the 'include' directory.
# -MMD -MP generates dependency files (.d) for header tracking
CFLAGS = -g -Wall -std=c11 -I$(INCLUDE_DIR) -I$(SRC_DIR) -I$(UNITY_PATH)/src -MMD -MP
# Add linker flags if needed, e.g., -lm for math library
LDFLAGS =

# --- PROJECT MODULES ---
# Define the main module, which determines the final executable name.
MAIN_MODULE = core

# List the names of the folders inside 'src' that you want to build.
# WARNING: Must be in order of dependencies!
SRC_MODULES = memory reader config core

# List the names of the 'src' folders that have corresponding tests.
# An executable 'test_module' will be generated for each entry.
TEST_MODULES = memory reader config

# --- MAIN BUILD TARGETS ---
# Find all .c files within the specified module directories for the main build
MODULE_PATHS = $(patsubst %,$(SRC_DIR)/%,$(SRC_MODULES))
SOURCES = $(foreach dir,$(MODULE_PATHS),$(shell find $(dir) -name '*.c'))

# Create object file paths in the BUILD_DIR, preserving the subdirectory structure
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
# Create dependency file paths (.d) for the main build
DEPS = $(OBJECTS:.o=.d)

# Default target: builds the main executable
all: $(BIN_DIR)/$(EXEC)

# Rule to link the main executable
$(BIN_DIR)/$(EXEC): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Linking complete. Executable '$(EXEC)' is at $(BIN_DIR)/$(EXEC)"

# --- TEST BUILD TARGETS ---
TEST_BIN_DIR = $(BUILD_DIR)/bin/test

# --- Unity Framework Objects ---
UNITY_SRC = $(UNITY_PATH)/src/unity.c
UNITY_OBJ = $(TEST_BIN_DIR)/unity.o

# --- Define Test Executables ---
# Create a list of all test executables based on TEST_MODULES
# e.g., build/bin/test/test_memory, build/bin/test/test_options, ...
TEST_EXECS = $(patsubst %,$(TEST_BIN_DIR)/test_%,$(TEST_MODULES))

# --- Sources & Objects for Test Libraries ---
# Find all .c files for the modules required by the tests (e.g., memory.c)
TEST_LIB_PATHS = $(patsubst %,$(SRC_DIR)/%,$(TEST_MODULES))
TEST_LIB_SOURCES = $(foreach dir,$(TEST_LIB_PATHS),$(shell find $(dir) -name '*.c'))
# Create object file paths for the test libraries in the test build directory
TEST_LIB_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(TEST_BIN_DIR)/%.o,$(TEST_LIB_SOURCES))

# --- Sources & Objects for Test Runners ---
# Assumes a test runner file exists for each module, e.g., test/unit/test_memory.c
TEST_RUNNER_SRCS = $(patsubst %,$(TEST_DIR)/test_%.c,$(TEST_MODULES))
TEST_RUNNER_OBJS = $(patsubst $(TEST_DIR)/%.c,$(TEST_BIN_DIR)/%.o,$(TEST_RUNNER_SRCS))

# --- Combined Dependencies ---
# List all test-related object files to generate dependencies for them
TEST_ALL_OBJECTS = $(TEST_RUNNER_OBJS) $(TEST_LIB_OBJECTS)
# MODIFIED: Add Unity object dependencies
TEST_DEPS = $(TEST_ALL_OBJECTS:.o=.d) $(UNITY_OBJ:.o=.d)

# --- Main Test Target ---
# Builds and runs all executables defined in TEST_EXECS
test: $(TEST_EXECS)
	@echo "--- Running All Tests ---"
	@echo "Test executables: $(TEST_EXECS)"
	@$(foreach exec,$(TEST_EXECS), echo "▶️  Running $$exec"; ./$(exec) || exit 1;)
	@echo "✅ --- All Tests Finished Successfully ---"

# --- Generic Rule to Link Any Test Executable ---
# This pattern rule links a test runner object (e.g., test_memory.o) with ALL compiled library objects.
# $^ refers to all prerequisites.
# MODIFIED: Add $(UNITY_OBJ) as a dependency for linking
$(TEST_BIN_DIR)/test_%: $(TEST_BIN_DIR)/test_%.o $(TEST_LIB_OBJECTS) $(UNITY_OBJ)
	@mkdir -p $(@D)
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Linking test executable '$@'"

# --- GENERIC COMPILATION RULES ---
# Rule to compile 'src' files for the main build
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< into $@"

# Rule to compile 'src' files for the test build
$(TEST_BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled Test Lib $< into $@"

# Rule to compile test runner files from 'test/unit'
$(TEST_BIN_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled Test Runner $< into $@"

# --- ADDED: Rule to compile the Unity framework source ---
$(UNITY_OBJ): $(UNITY_SRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled Test Framework $< into $@"

# --- HOUSEKEEPING ---
# Clean up all build artifacts
clean:
	@echo "Cleaning up build files..."
	rm -rf $(BUILD_DIR)

# Declare non-file targets
.PHONY: all clean test

# Include all generated dependency files.
-include $(DEPS) $(TEST_DEPS)