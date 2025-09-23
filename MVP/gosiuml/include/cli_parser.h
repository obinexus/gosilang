#ifndef CLI_PARSER_H
#define CLI_PARSER_H

// Forward declarations
typedef struct PhenoToken PhenoToken;
typedef struct GosiUMLContext GosiUMLContext;

// CLI-specific functions
int parse_cli_config(const char* config_file);
void print_token_info(PhenoToken* token);
void display_state_diagram(GosiUMLContext* ctx);

#endif // CLI_PARSER_H
