/**
 * GosiUML SVG Blueprint Generator
 * Generates state machine visualization
 */

#include <stdio.h>
#include <stdlib.h>
#include "gosiuml.h"
#include "phenomemory_platform.h"

// External SVG generation function
extern void generate_svg_from_tokens(PhenoToken* tokens, int count, const char* output_file);

int main(int argc, char* argv[]) {
    const char* output_file = (argc > 1) ? argv[1] : "gosiuml_blueprint.svg";
    
    printf("=== GosiUML SVG Blueprint Generator ===\n");
    printf("Generating: %s\n", output_file);
    
    // Create sample tokens for visualization
    PhenoToken* tokens[7];
    
    // Create tokens representing each state
    tokens[0] = pheno_token_alloc(512);
    if (tokens[0]) {
        tokens[0]->token_id = 0x10000001;
        snprintf(tokens[0]->sentinel, 16, "NIL");
        tokens[0]->memory_zone = 0;
    }
    
    tokens[1] = pheno_token_alloc(512);
    if (tokens[1]) {
        tokens[1]->token_id = 0x10000002;
        snprintf(tokens[1]->sentinel, 16, "ALLOCATED");
        tokens[1]->memory_zone = 1;
    }
    
    tokens[2] = pheno_token_alloc(512);
    if (tokens[2]) {
        tokens[2]->token_id = 0x10000003;
        snprintf(tokens[2]->sentinel, 16, "LOCKED");
        tokens[2]->memory_zone = 2;
    }
    
    tokens[3] = pheno_token_alloc(512);
    if (tokens[3]) {
        tokens[3]->token_id = 0x10000004;
        snprintf(tokens[3]->sentinel, 16, "ACTIVE");
        tokens[3]->memory_zone = 3;
    }
    
    tokens[4] = pheno_token_alloc(512);
    if (tokens[4]) {
        tokens[4]->token_id = 0x10000005;
        snprintf(tokens[4]->sentinel, 16, "DEGRADED");
        tokens[4]->memory_zone = 4;
    }
    
    tokens[5] = pheno_token_alloc(512);
    if (tokens[5]) {
        tokens[5]->token_id = 0x10000006;
        snprintf(tokens[5]->sentinel, 16, "SHARED");
        tokens[5]->memory_zone = 5;
    }
    
    tokens[6] = pheno_token_alloc(512);
    if (tokens[6]) {
        tokens[6]->token_id = 0x10000007;
        snprintf(tokens[6]->sentinel, 16, "FREED");
        tokens[6]->memory_zone = 6;
    }
    
    // Generate the SVG file
    generate_svg_from_tokens(tokens, 7, output_file);
    
    printf("SVG blueprint generated successfully!\n");
    printf("View with: firefox %s\n", output_file);
    
    // Also generate using the simple function
    int result = generate_svg("state_diagram.svg");
    if (result == 0) {
        printf("Additional state diagram generated: state_diagram.svg\n");
    }
    
    // Clean up
    for (int i = 0; i < 7; i++) {
        if (tokens[i]) {
            pheno_token_free(tokens[i]);
        }
    }
    
    // Cleanup memory pool
    extern void pheno_memory_cleanup(void);
    pheno_memory_cleanup();
    
    return 0;
}
