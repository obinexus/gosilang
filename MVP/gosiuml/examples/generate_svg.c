#include <stdio.h>
#include <stdlib.h>
#include "phenomemory_platform.h"

// External function from svg_generator.c
extern void generate_svg_from_tokens(PhenoToken* tokens, int count, const char* output_file);

int main() {
    printf("=== Generating GosiUML SVG Blueprint ===\n");
    
    // Create tokens for all states
    PhenoToken* tokens[7];
    const char* state_names[] = {"NIL", "ALLOCATED", "LOCKED", "ACTIVE", "DEGRADED", "SHARED", "FREED"};
    
    for (int i = 0; i < 7; i++) {
        tokens[i] = pheno_token_alloc(512);
        if (tokens[i]) {
            tokens[i]->token_id = 0x10000001 + i;
            snprintf(tokens[i]->sentinel, 16, "%s", state_names[i]);
            tokens[i]->memory_zone = i;
            printf("Created token: %s (0x%08X) in zone %d\n", 
                   state_names[i], tokens[i]->token_id, i);
        }
    }
    
    // Generate the SVG file
    const char* svg_file = "gosiuml_blueprint.svg";
    generate_svg_from_tokens(tokens, 7, svg_file);
    printf("\nSVG blueprint generated: %s\n", svg_file);
    printf("View with: firefox %s\n", svg_file);
    
    // Clean up
    for (int i = 0; i < 7; i++) {
        if (tokens[i]) pheno_token_free(tokens[i]);
    }
    
    extern void pheno_memory_cleanup(void);
    pheno_memory_cleanup();
    
    return 0;
}
