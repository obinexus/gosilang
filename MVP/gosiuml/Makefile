# GosiUML Phenomenological Memory Platform Makefile
# OBINexus Project - MVP Build System

# Compiler and flags
CC = gcc
CFLAGS = -Wall -O2 -I./include -fPIC -pthread
LDFLAGS = -pthread -lm
DEBUG_FLAGS = -g -DDEBUG -fsanitize=address
RELEASE_FLAGS = -O3 -march=native -DNDEBUG

# Directories
SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
CLI_DIR = $(SRC_DIR)/cli
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include
LIB_DIR = lib
TEMPLATE_DIR = templates
DOC_DIR = docs

# Source files
CORE_SRCS = $(CORE_DIR)/pheno_memory.c \
            $(CORE_DIR)/pheno_state_machine.c \
            $(CORE_DIR)/pheno_relation.c \
            $(CORE_DIR)/token_parser.c \
            $(CORE_DIR)/svg_generator.c

CLI_SRCS = $(CLI_DIR)/cli_parser.c \
           $(CLI_DIR)/main.c

MAIN_SRC = $(SRC_DIR)/main.c

# Object files
CORE_OBJS = $(patsubst $(CORE_DIR)/%.c,$(BUILD_DIR)/%.o,$(CORE_SRCS))
CLI_OBJS = $(patsubst $(CLI_DIR)/%.c,$(BUILD_DIR)/%.o,$(CLI_SRCS))
MAIN_OBJ = $(BUILD_DIR)/main.o

# All object files
ALL_OBJS = $(CORE_OBJS) $(CLI_OBJS) $(MAIN_OBJ)

# Target executables
GOSIUML_BIN = $(BIN_DIR)/gosiuml
GOSIUML_CLI = $(BIN_DIR)/gosiuml-cli
GOSIUML_TEST = $(BIN_DIR)/gosiuml-test

# Library targets
STATIC_LIB = $(LIB_DIR)/libgosiuml.a
SHARED_LIB = $(LIB_DIR)/libgosiuml.so

# Default target
all: directories $(GOSIUML_BIN) $(GOSIUML_CLI) $(STATIC_LIB) $(SHARED_LIB)
	@echo "=== Build Complete ==="
	@echo "Binaries in: $(BIN_DIR)/"
	@echo "Libraries in: $(LIB_DIR)/"
	@ls -la $(BIN_DIR)/
	@ls -la $(LIB_DIR)/ 2>/dev/null || true

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(DOC_DIR)

# Main gosiuml executable (test driver)
$(GOSIUML_BIN): $(BUILD_DIR)/main.o $(BUILD_DIR)/pheno_memory.o $(BUILD_DIR)/pheno_state_machine.o
	@echo "Linking $@..."
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Built: $@"

# CLI tool
$(GOSIUML_CLI): $(CLI_OBJS) $(CORE_OBJS)
	@echo "Linking $@..."
	$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Built: $@"

# Static library
$(STATIC_LIB): $(CORE_OBJS)
	@echo "Creating static library $@..."
	ar rcs $@ $^
	@echo "Built: $@"

# Shared library
$(SHARED_LIB): $(CORE_OBJS)
	@echo "Creating shared library $@..."
	$(CC) -shared -o $@ $^ $(LDFLAGS)
	@echo "Built: $@"

# Pattern rules for object files
$(BUILD_DIR)/%.o: $(CORE_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(CLI_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Specific rules for missing source files (create stubs)
$(CORE_DIR)/pheno_relation.c:
	@echo "Creating stub: $@"
	@mkdir -p $(CORE_DIR)
	@echo '#include "phenomemory_platform.h"' > $@
	@echo '// Stub for pheno_relation' >> $@

$(CORE_DIR)/token_parser.c:
	@echo "Creating stub: $@"
	@mkdir -p $(CORE_DIR)
	@echo '#include <stdio.h>' > $@
	@echo '#include "phenomemory_platform.h"' >> $@
	@echo 'int parse_token_file(const char* filename) {' >> $@
	@echo '    printf("Parsing token file: %s\\n", filename);' >> $@
	@echo '    return 0;' >> $@
	@echo '}' >> $@

$(CORE_DIR)/svg_generator.c:
	@echo "Creating stub: $@"
	@mkdir -p $(CORE_DIR)
	@echo '#include <stdio.h>' > $@
	@echo '#include "phenomemory_platform.h"' >> $@
	@echo 'int generate_svg(const char* output_file) {' >> $@
	@echo '    printf("Generating SVG: %s\\n", output_file);' >> $@
	@echo '    return 0;' >> $@
	@echo '}' >> $@

$(CLI_DIR)/cli_parser.c:
	@echo "Creating stub: $@"
	@mkdir -p $(CLI_DIR)
	@echo '#include <stdio.h>' > $@
	@echo '#include <getopt.h>' >> $@
	@echo 'int parse_cli_args(int argc, char* argv[]) {' >> $@
	@echo '    return 0;' >> $@
	@echo '}' >> $@

$(CLI_DIR)/main.c:
	@echo "Creating stub: $@"
	@mkdir -p $(CLI_DIR)
	@echo '#include <stdio.h>' > $@
	@echo 'int parse_cli_args(int argc, char* argv[]);' >> $@
	@echo 'int main(int argc, char* argv[]) {' >> $@
	@echo '    printf("GosiUML CLI v1.0.0\\n");' >> $@
	@echo '    return parse_cli_args(argc, argv);' >> $@
	@echo '}' >> $@

# Ensure headers exist
$(INCLUDE_DIR)/cli_parser.h:
	@echo "Creating header: $@"
	@echo '#ifndef CLI_PARSER_H' > $@
	@echo '#define CLI_PARSER_H' >> $@
	@echo 'int parse_cli_args(int argc, char* argv[]);' >> $@
	@echo '#endif' >> $@

$(INCLUDE_DIR)/gosiuml.h:
	@echo "Creating header: $@"
	@echo '#ifndef GOSIUML_H' > $@
	@echo '#define GOSIUML_H' >> $@
	@echo '#include "phenomemory_platform.h"' >> $@
	@echo '#endif' >> $@

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# Release build
release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

# Run tests
test: $(GOSIUML_BIN)
	@echo "=== Running Tests ==="
	$(GOSIUML_BIN) -t

# Run basic test
run: $(GOSIUML_BIN)
	@echo "=== Running Basic Test ==="
	$(GOSIUML_BIN) -b

# Run stress test
stress: $(GOSIUML_BIN)
	@echo "=== Running Stress Test ==="
	$(GOSIUML_BIN) -s 1000

# Memory check with valgrind
memcheck: debug
	valgrind --leak-check=full --show-leak-kinds=all $(GOSIUML_BIN) -b

# Generate documentation
docs:
	@echo "Generating documentation..."
	@mkdir -p $(DOC_DIR)
	@echo "# GosiUML Documentation" > $(DOC_DIR)/README.md
	@echo "## State Machine States" >> $(DOC_DIR)/README.md
	@echo "- NIL: Initial state" >> $(DOC_DIR)/README.md
	@echo "- ALLOCATED: Memory allocated" >> $(DOC_DIR)/README.md
	@echo "- LOCKED: Exclusive access" >> $(DOC_DIR)/README.md
	@echo "- ACTIVE: Processing" >> $(DOC_DIR)/README.md
	@echo "- DEGRADED: Error recovery" >> $(DOC_DIR)/README.md
	@echo "- SHARED: Multi-reference" >> $(DOC_DIR)/README.md
	@echo "- FREED: Released" >> $(DOC_DIR)/README.md

# Install targets
install: all
	@echo "Installing to /usr/local..."
	@install -d /usr/local/bin
	@install -d /usr/local/lib
	@install -d /usr/local/include/gosiuml
	@install -m 755 $(GOSIUML_BIN) /usr/local/bin/
	@install -m 755 $(GOSIUML_CLI) /usr/local/bin/
	@install -m 644 $(STATIC_LIB) /usr/local/lib/
	@install -m 755 $(SHARED_LIB) /usr/local/lib/
	@install -m 644 $(INCLUDE_DIR)/*.h /usr/local/include/gosiuml/
	@ldconfig 2>/dev/null || true
	@echo "Installation complete!"

# Uninstall
uninstall:
	@echo "Removing installed files..."
	@rm -f /usr/local/bin/gosiuml
	@rm -f /usr/local/bin/gosiuml-cli
	@rm -f /usr/local/lib/libgosiuml.*
	@rm -rf /usr/local/include/gosiuml
	@ldconfig 2>/dev/null || true
	@echo "Uninstall complete!"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(BUILD_DIR)/*.o
	rm -rf $(BIN_DIR)/*
	rm -rf $(LIB_DIR)/*
	@echo "Clean complete!"

# Deep clean (including generated files)
distclean: clean
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(DOC_DIR)
	@echo "Distribution clean complete!"

# Create source archive
dist:
	@echo "Creating source archive..."
	tar czf gosiuml-1.0.0.tar.gz \
		Makefile \
		$(SRC_DIR) \
		$(INCLUDE_DIR) \
		$(TEMPLATE_DIR) \
		README.md 2>/dev/null || true
	@echo "Archive created: gosiuml-1.0.0.tar.gz"

# Show help
help:
	@echo "GosiUML Makefile Targets:"
	@echo "  all       - Build all targets (default)"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized release"
	@echo "  test      - Run test suite"
	@echo "  run       - Run basic test"
	@echo "  stress    - Run stress test"
	@echo "  memcheck  - Run with valgrind"
	@echo "  docs      - Generate documentation"
	@echo "  install   - Install to system"
	@echo "  uninstall - Remove from system"
	@echo "  clean     - Remove build artifacts"
	@echo "  distclean - Complete cleanup"
	@echo "  dist      - Create source archive"
	@echo "  help      - Show this help"

# Phony targets
.PHONY: all directories debug release test run stress memcheck docs \
        install uninstall clean distclean dist help

# Print configuration
info:
	@echo "=== Build Configuration ==="
	@echo "CC:        $(CC)"
	@echo "CFLAGS:    $(CFLAGS)"
	@echo "LDFLAGS:   $(LDFLAGS)"
	@echo "BUILD_DIR: $(BUILD_DIR)"
	@echo "BIN_DIR:   $(BIN_DIR)"
	@echo "LIB_DIR:   $(LIB_DIR)"
	@echo "=========================="

# Dependency generation
-include $(ALL_OBJS:.o=.d)

%.d: %.c
	@$(CC) -MM $(CFLAGS) $< > $@
