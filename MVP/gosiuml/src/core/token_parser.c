#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gosiuml.h"
#include "phenomemory_platform.h"

// Parse token file and allocate tokens
int parse_token_file(const char* filename) {
    printf("[PARSER] Parsing token file: %s\n", filename);
    
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("[PARSER] Could not open file: %s\n", filename);
        return -1;
    }
    
    char line[256];
    int token_count = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        // Skip comments and empty lines
        if (line[0] == '#' || line[0] == '\n') continue;
        
        // Parse token definitions
        if (strstr(line, "TOKEN:")) {
            uint32_t id;
            char type[32], zone[16];
            
            if (sscanf(line, "TOKEN: 0x%x %s %s", &id, type, zone) == 3) {
                token_count++;
                printf("[PARSER] Found token: ID=0x%08X TYPE=%s ZONE=%s\n", 
                       id, type, zone);
                
                // Create actual token using pheno_token_alloc
                PhenoToken* token = pheno_token_alloc(1024);
                if (token) {
                    token->token_id = id;
                    strncpy(token->sentinel, type, 15);
                    token->sentinel[15] = '\0';  // Ensure null termination
                    token->memory_zone = atoi(zone);
                    
                    // Process the token
                    printf("[PARSER] Allocated token 0x%08X in zone %u\n",
                           token->token_id, token->memory_zone);
                }
            }
        }
        
        // Parse relation definitions
        if (strstr(line, "RELATION:")) {
            uint32_t src_id, dst_id;
            char rel_type[32];
            
            if (sscanf(line, "RELATION: 0x%x -> 0x%x : %s", 
                      &src_id, &dst_id, rel_type) == 3) {
                printf("[PARSER] Found relation: 0x%08X -> 0x%08X (%s)\n",
                       src_id, dst_id, rel_type);
            }
        }
    }
    
    fclose(fp);
    printf("[PARSER] Parsed %d tokens\n", token_count);
    return token_count;
}
