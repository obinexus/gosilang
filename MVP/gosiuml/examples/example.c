/**
 * GosiUML Library Usage Example
 * OBINexus Phenomenological Memory Platform
 * 
 * Compile: gcc -o example example.c -L./lib -lgosiuml -I./include -pthread
 * Run: LD_LIBRARY_PATH=./lib ./example
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gosiuml.h"
#include "phenomemory_platform.h"

// Example 1: Basic State Machine Usage
void example_state_machine() {
    printf("\n=== Example 1: State Machine ===\n");
    
    // Create and initialize state machine
    StateMachine* sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return;
    }
    
    initialize_state_machine(sm);
    
    // Perform state transitions
    printf("Initial state: %s\n", get_state_name(sm->current_state));
    
    // NIL -> ALLOCATED
    step_state_machine(sm, EVENT_ALLOC);
    printf("After ALLOC: %s\n", get_state_name(sm->current_state));
    
    // ALLOCATED -> LOCKED
    step_state_machine(sm, EVENT_LOCK);
    printf("After LOCK: %s\n", get_state_name(sm->current_state));
    
    // LOCKED -> ACTIVE
    step_state_machine(sm, EVENT_VALIDATE);
    printf("After VALIDATE: %s\n", get_state_name(sm->current_state));
    
    // Clean up
    destroy_state_machine(sm);
}

// Example 2: Token Allocation and Management
void example_token_management() {
    printf("\n=== Example 2: Token Management ===\n");
    
    // Allocate phenomenological tokens
    PhenoToken* token1 = pheno_token_alloc(1024);
    PhenoToken* token2 = pheno_token_alloc(2048);
    
    if (token1 && token2) {
        printf("Token 1: ID=0x%08X, Zone=%u, Size=%zu\n",
               token1->token_id, token1->memory_zone, token1->data_size);
        printf("Token 2: ID=0x%08X, Zone=%u, Size=%zu\n",
               token2->token_id, token2->memory_zone, token2->data_size);
        
        // Test locking mechanism
        if (pheno_token_lock(token1)) {
            printf("Successfully locked token 1\n");
            
            // Perform operations on locked token
            set_flag(&token1->mem_flags, FLAG_DIRTY_BIT);
            printf("Token 1 dirty flag set\n");
            
            pheno_token_unlock(token1);
            printf("Token 1 unlocked\n");
        }
        
        // Test reference counting
        increment_ref_count(&token2->mem_flags);
        printf("Token 2 ref count: %u\n", get_ref_count(&token2->mem_flags));
        
        // Validate tokens
        if (pheno_token_validate(token1)) {
            printf("Token 1 validation: PASSED\n");
        }
        
        // Clean up
        pheno_token_free(token1);
        pheno_token_free(token2);
    }
}

// Example 3: Relation Mapping
void example_relation_mapping() {
    printf("\n=== Example 3: Relation Mapping ===\n");
    
    PhenoRelation rel1 = {0};
    PhenoRelation rel2 = {0};
    
    // Initialize first relation
    rel1.subject_id = 0x01;
    rel1.subject_type = 0x10;
    rel1.class_id = 0x02;
    rel1.instance_id = 0x03;
    
    // Initialize second relation
    rel2.subject_id = 0x04;
    rel2.subject_type = 0x20;
    rel2.class_id = 0x05;
    rel2.instance_id = 0x06;
    
    printf("Before mapping:\n");
    printf("  Rel1: subject=%02X, class=%02X, instance=%02X\n",
           rel1.subject_id, rel1.class_id, rel1.instance_id);
    printf("  Rel2: subject=%02X, class=%02X, instance=%02X\n",
           rel2.subject_id, rel2.class_id, rel2.instance_id);
    
    // Apply object-to-object mapping
    map_obj_to_obj(&rel1, &rel2);
    
    printf("After obj-to-obj mapping:\n");
    printf("  Rel2: subject=%02X, class=%02X, instance=%02X\n",
           rel2.subject_id, rel2.class_id, rel2.instance_id);
    
    // Apply person-to-person model
    apply_person_model(&rel1, 0xAA, 0x55);
    printf("After person model (AA->55):\n");
    printf("  Person: id=%02X, role=%02X, auth=%02X, state=%02X\n",
           rel1.person_id, rel1.person_role, 
           rel1.person_auth, rel1.person_state);
}

// Example 4: Parse Configuration File
void example_parse_config() {
    printf("\n=== Example 4: Configuration Parsing ===\n");
    
    // Create test configuration file
    FILE* fp = fopen("test_tokens.txt", "w");
    if (fp) {
        fprintf(fp, "# GosiUML Test Configuration\n");
        fprintf(fp, "TOKEN: 0x10000001 PHENO_NIL 0\n");
        fprintf(fp, "TOKEN: 0x10000002 ALLOCATED 1\n");
        fprintf(fp, "TOKEN: 0x10000003 LOCKED 2\n");
        fprintf(fp, "RELATION: 0x10000001 -> 0x10000002 : alloc\n");
        fprintf(fp, "RELATION: 0x10000002 -> 0x10000003 : lock\n");
        fclose(fp);
        
        // Parse the configuration
        int count = parse_token_file("test_tokens.txt");
        printf("Parsed %d tokens from configuration\n", count);
        
        // Clean up test file
        unlink("test_tokens.txt");
    }
}

// Example 5: Memory Statistics
void example_memory_stats() {
    printf("\n=== Example 5: Memory Statistics ===\n");
    
    // Allocate several tokens to populate memory zones
    PhenoToken* tokens[5];
    for (int i = 0; i < 5; i++) {
        tokens[i] = pheno_token_alloc(512 * (i + 1));
        if (tokens[i]) {
            printf("Allocated token %d: zone=%u, size=%zu\n",
                   i, tokens[i]->memory_zone, tokens[i]->data_size);
        }
    }
    
    // Show memory statistics
    extern void pheno_memory_stats(void);
    pheno_memory_stats();
    
    // Clean up
    for (int i = 0; i < 5; i++) {
        if (tokens[i]) {
            pheno_token_free(tokens[i]);
        }
    }
}

int main(int argc, char* argv[]) {
    printf("================================================\n");
    printf("   GosiUML Library Examples                    \n");
    printf("   OBINexus Phenomenological Memory Platform   \n");
    printf("================================================\n");
    
    // Run examples
    example_state_machine();
    example_token_management();
    example_relation_mapping();
    example_parse_config();
    example_memory_stats();
    
    // Final cleanup
    extern void pheno_memory_cleanup(void);
    pheno_memory_cleanup();
    
    printf("\n=== All Examples Complete ===\n");
    return 0;
}
