#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include "phenomemory_platform.h"

// Forward declaration
typedef struct GosiUMLContext GosiUMLContext;

// CLI functions
int parse_cli_args(int argc, char* argv[]);
int parse_cli_config(const char* config_file);
void print_token_info(PhenoToken* token);
void display_state_diagram(GosiUMLContext* ctx);

// Context management
GosiUMLContext* create_cli_context(void);
void free_cli_context(GosiUMLContext* ctx);

#endif // CLI_PARSER_H
