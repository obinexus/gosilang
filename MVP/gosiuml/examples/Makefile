# Makefile for GosiUML Examples
# Build and run library usage examples

CC = gcc
CFLAGS = -Wall -O2 -I../include
LDFLAGS = -L../lib -lgosiuml -pthread -lm

# Use static or shared library
LINK_MODE ?= static

ifeq ($(LINK_MODE),shared)
    RUN_PREFIX = LD_LIBRARY_PATH=../lib
else
    LDFLAGS += -static
endif

EXAMPLES = example test_states test_tokens test_relations

all: $(EXAMPLES)

example: example.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Built: $@"
	@echo "Run with: $(RUN_PREFIX) ./$@"

test_states: test_states.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Built: $@"

test_tokens: test_tokens.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Built: $@"

test_relations: test_relations.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "Built: $@"

run: example
	$(RUN_PREFIX) ./example

run-all: $(EXAMPLES)
	@echo "=== Running All Examples ==="
	@for prog in $(EXAMPLES); do \
		echo "\nRunning $$prog..."; \
		$(RUN_PREFIX) ./$$prog || exit 1; \
	done

clean:
	rm -f $(EXAMPLES) *.o

help:
	@echo "GosiUML Examples Makefile"
	@echo "Usage:"
	@echo "  make         - Build all examples"
	@echo "  make run     - Build and run main example"
	@echo "  make run-all - Build and run all examples"
	@echo "  make clean   - Remove built files"
	@echo ""
	@echo "Options:"
	@echo "  LINK_MODE=static  - Use static linking (default)"
	@echo "  LINK_MODE=shared  - Use shared library"

.PHONY: all run run-all clean help
