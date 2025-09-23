# gosiuml/Makefile - Library and CLI build system
CC = gcc
AR = ar
CFLAGS = -Wall -O2 -I./include -fPIC -pthread
LDFLAGS = -L./lib -lgosiuml -lm -pthread
ARFLAGS = rcs

# Directories
SRC_CORE = src/core
SRC_CLI = src/cli
OBJ_DIR = build
LIB_DIR = lib
BIN_DIR = bin
INCLUDE_DIR = include

# Core library sources
CORE_SRCS = $(SRC_CORE)/token_parser.c \
            $(SRC_CORE)/svg_generator.c \
            $(SRC_CORE)/pheno_state_machine.c \
            $(SRC_CORE)/pheno_memory.c \
            $(SRC_CORE)/pheno_relation.c

# CLI sources
CLI_SRCS = $(SRC_CLI)/main.c \
           $(SRC_CLI)/cli_parser.c

# Object files
CORE_OBJS = $(CORE_SRCS:$(SRC_CORE)/%.c=$(OBJ_DIR)/%.o)
CLI_OBJS = $(CLI_SRCS:$(SRC_CLI)/%.c=$(OBJ_DIR)/%.o)

# Targets
STATIC_LIB = $(LIB_DIR)/libgosiuml.a
SHARED_LIB = $(LIB_DIR)/libgosiuml.so
CLI_TARGET = $(BIN_DIR)/gosiuml

# Default target
all: dirs $(STATIC_LIB) $(SHARED_LIB) $(CLI_TARGET)

# Create necessary directories
dirs:
	@mkdir -p $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)

# Build static library
$(STATIC_LIB): $(CORE_OBJS)
	@echo "Building static library..."
	$(AR) $(ARFLAGS) $@ $^
	@echo "Static library built: $@"

# Build shared library
$(SHARED_LIB): $(CORE_OBJS)
	@echo "Building shared library..."
	$(CC) -shared -o $@ $^ -lm
	@echo "Shared library built: $@"

# Build CLI executable
$(CLI_TARGET): $(CLI_OBJS) $(STATIC_LIB)
	@echo "Building CLI executable..."
	$(CC) -o $@ $(CLI_OBJS) $(LDFLAGS)
	@echo "CLI built: $@"

# Compile core library objects
$(OBJ_DIR)/%.o: $(SRC_CORE)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Compile CLI objects
$(OBJ_DIR)/%.o: $(SRC_CLI)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Install library (system-wide or local)
install: $(STATIC_LIB) $(SHARED_LIB)
	@echo "Installing libraries..."
	install -d /usr/local/lib
	install -m 644 $(STATIC_LIB) /usr/local/lib/
	install -m 755 $(SHARED_LIB) /usr/local/lib/
	install -d /usr/local/include/gosiuml
	install -m 644 $(INCLUDE_DIR)/*.h /usr/local/include/gosiuml/
	ldconfig
	@echo "Installation complete"

# Local install (for development)
install-local: $(STATIC_LIB) $(SHARED_LIB)
	@echo "Setting up local installation..."
	@echo "export LD_LIBRARY_PATH=$(PWD)/lib:\$$LD_LIBRARY_PATH" > setup.sh
	@echo "export LIBRARY_PATH=$(PWD)/lib:\$$LIBRARY_PATH" >> setup.sh
	@echo "export C_INCLUDE_PATH=$(PWD)/include:\$$C_INCLUDE_PATH" >> setup.sh
	chmod +x setup.sh
	@echo "Run 'source setup.sh' to use the library locally"

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(BIN_DIR)
	rm -f *.svg *.xml setup.sh

# Run tests
test: $(CLI_TARGET)
	@echo "Running tests..."
	$(CLI_TARGET) config/token_file.txt test_output.svg

# Build examples
examples: $(STATIC_LIB)
	@echo "Building examples..."
	$(CC) examples/simple_usage.c -o examples/simple_usage $(CFLAGS) $(LDFLAGS)

.PHONY: all dirs clean install install-local test examples
