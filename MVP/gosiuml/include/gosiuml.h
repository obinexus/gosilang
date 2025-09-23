#ifndef GOSIUML_H
#define GOSIUML_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// Version macros
#define GOSIUML_VERSION_MAJOR 1
#define GOSIUML_VERSION_MINOR 0
#define GOSIUML_VERSION_PATCH 0

// Output formats
typedef enum {
    FORMAT_SVG = 0,
    FORMAT_XML = 1,
    FORMAT_JSON = 2
} GosiUMLFormat;

// Options
typedef enum {
    GOSIUML_OPT_VERBOSE = 1,
    GOSIUML_OPT_SHOW_BITFIELDS = 2,
    GOSIUML_OPT_STATE_MACHINE = 3,
    GOSIUML_OPT_MEMORY_TRACKING = 4
} GosiUMLOption;

// Forward declaration of context structure
typedef struct GosiUMLContext GosiUMLContext;

// Include platform definitions (contains PhenoToken, PhenoState, etc)
#include "phenomemory_platform.h"

// Function prototypes
int gosiuml_init(void);
void gosiuml_cleanup(void);
const char* gosiuml_version(void);
GosiUMLContext* gosiuml_create_context(void);
void gosiuml_free_context(GosiUMLContext* ctx);
int gosiuml_set_option(GosiUMLContext* ctx, GosiUMLOption option, int value);
PhenoToken* gosiuml_parse_file(const char* filename, int* count);
PhenoToken* gosiuml_create_token(uint8_t type, const char* name);
void gosiuml_free_token(PhenoToken* token);
void gosiuml_free_tokens(PhenoToken* tokens, int count);
int gosiuml_process_token(GosiUMLContext* ctx, PhenoToken* token);
int gosiuml_generate_svg(GosiUMLContext* ctx, PhenoToken* tokens, int count, const char* output_file);
int gosiuml_generate_xml(GosiUMLContext* ctx, PhenoToken* tokens, int count, const char* output_file);
int gosiuml_generate_json(GosiUMLContext* ctx, PhenoToken* tokens, int count, const char* output_file);
int gosiuml_get_state(PhenoToken* token);
int gosiuml_transition(PhenoToken* token, int new_state);
int gosiuml_test_state_machine(GosiUMLContext* ctx);
int gosiuml_test_bitfields(void);
int gosiuml_run_tests(void);
const char* gosiuml_get_error(void);
void gosiuml_set_debug(bool enable);

// Export parse_token_file function
int parse_token_file(const char* filename);
int generate_svg(const char* output_file);

#endif // GOSIUML_H

