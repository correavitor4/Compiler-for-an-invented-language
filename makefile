# Compiler and flags
CC = gcc
# Use -g for debugging, -Wall for warnings.
# -Iinclude tells gcc to look for headers in the 'include' directory.
# -MMD -MP generates dependency files (.d) for header tracking
CFLAGS = -g -Wall -std=c11 -Iinclude -MMD -MP
# Add linker flags if needed, e.g., -lm for math library
LDFLAGS =

MAIN_MODULE = core

# --- BUILD SPECIFIC FOLDERS ---
# List the names of the folders inside 'src' that you want to build.
# For example, to build only 'src/core' and 'src/utils', set:

# WARNING : Must be in order of dependencies!

# Ex: SRC_MODULES = core utils
SRC_MODULES = memory reader core

# Project name and directories
EXEC = $(MAIN_MODULE)
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
SRC_DIR = src
INCLUDE_DIR = include

# Find all .c files within the specified module directories
MODULE_PATHS = $(patsubst %,$(SRC_DIR)/%,$(SRC_MODULES))
SOURCES = $(foreach dir,$(MODULE_PATHS),$(shell find $(dir) -name '*.c'))

# Create object file paths in the BUILD_DIR, preserving the subdirectory structure
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
# Create dependency file paths (.d) in the BUILD_DIR
DEPS = $(OBJECTS:.o=.d)

# The default target, which builds the executable in the BIN_DIR
all: $(BIN_DIR)/$(EXEC)

# Rule to link the executable
$(BIN_DIR)/$(EXEC): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Linking complete. Executable '$(EXEC)' is at $(BIN_DIR)/$(EXEC)"

# Rule to compile source files into object files in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< into $@"

# Clean up build artifacts
clean:
	@echo "Cleaning up build files..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Declare non-file targets
.PHONY: all clean

# Include the dependency files. The '-' prevents errors if the files don't exist yet.
-include $(DEPS)
