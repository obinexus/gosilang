#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "phenomemory_platform.h"

// External functions
void pheno_memory_stats(void);
void pheno_memory_cleanup(void);

// Test scenarios
void test_basic_transitions(void) {
    printf("\n=== Testing Basic State Transitions ===\n");
    
    StateMachine* sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return;
    }
    
    initialize_state_machine(sm);
    
    // NIL -> ALLOCATED
    step_state_machine(sm, EVENT_ALLOC);
    
    // ALLOCATED -> LOCKED
    step_state_machine(sm, EVENT_LOCK);
    
    // LOCKED -> ACTIVE
    step_state_machine(sm, EVENT_VALIDATE);
    
    // Test substates
    process_token_operations(sm);
    
    // ACTIVE -> SHARED
    step_state_machine(sm, EVENT_SHARE);
    
    // SHARED -> FREED
    step_state_machine(sm, EVENT_FREE);
    
    destroy_state_machine(sm);
}

void test_degradation_recovery(void) {
    printf("\n=== Testing Degradation and Recovery ===\n");
    
    StateMachine* sm = create_state_machine();
    initialize_state_machine(sm);
    
    // Get to ACTIVE state
    step_state_machine(sm, EVENT_ALLOC);
    step_state_machine(sm, EVENT_LOCK);
    step_state_machine(sm, EVENT_VALIDATE);
    
    // Force degradation
    sm->retry_count = 61; // Above threshold
    step_state_machine(sm, EVENT_DEGRADE);
    
    // Attempt recovery
    attempt_hitl_recovery(sm);
    step_state_machine(sm, EVENT_RECOVER);
    
    // Clean up
    step_state_machine(sm, EVENT_FREE);
    destroy_state_machine(sm);
}

void test_concurrent_access(void) {
    printf("\n=== Testing Concurrent Token Access ===\n");
    
    PhenoToken* token1 = pheno_token_alloc(1024);
    PhenoToken* token2 = pheno_token_alloc(2048);
    
    if (token1 && token2) {
        // Test locking
        if (pheno_token_lock(token1)) {
            printf("Token 1 locked successfully\n");
            
            // Try to lock again (should fail)
            if (!pheno_token_lock(token1)) {
                printf("Double lock prevented (expected)\n");
            }
            
            pheno_token_unlock(token1);
        }
        
        // Validate tokens
        pheno_token_validate(token1);
        pheno_token_validate(token2);
        
        // Test reference counting
        increment_ref_count(&token2->mem_flags);
        printf("Token 2 ref count: %u\n", get_ref_count(&token2->mem_flags));
        
        decrement_ref_count(&token2->mem_flags);
        printf("Token 2 ref count after decrement: %u\n", 
               get_ref_count(&token2->mem_flags));
        
        pheno_token_free(token1);
        pheno_token_free(token2);
    }
}

void test_memory_zones(void) {
    printf("\n=== Testing Memory Zone Allocation ===\n");
    
    PhenoToken* tokens[8];
    
    // Allocate tokens across different zones
    for (int i = 0; i < 8; i++) {
        tokens[i] = pheno_token_alloc(512 * (i + 1));
        if (tokens[i]) {
            printf("Token %d: zone=%u, size=%zu\n",
                   i, tokens[i]->memory_zone, tokens[i]->data_size);
        }
    }
    
    // Show memory statistics
    pheno_memory_stats();
    
    // Clean up
    for (int i = 0; i < 8; i++) {
        if (tokens[i]) {
            pheno_token_free(tokens[i]);
        }
    }
}

void run_stress_test(int iterations) {
    printf("\n=== Running Stress Test (%d iterations) ===\n", iterations);
    
    clock_t start = clock();
    int success_count = 0;
    int failure_count = 0;
    
    for (int i = 0; i < iterations; i++) {
        StateMachine* sm = create_state_machine();
        if (!sm) {
            failure_count++;
            continue;
        }
        
        initialize_state_machine(sm);
        
        // Random state transitions
        int num_events = rand() % 10 + 1;
        for (int j = 0; j < num_events; j++) {
            PhenoEvent event = (PhenoEvent)(rand() % 8);
            step_state_machine(sm, event);
        }
        
        destroy_state_machine(sm);
        success_count++;
        
        if ((i + 1) % 100 == 0) {
            printf("Progress: %d/%d\r", i + 1, iterations);
            fflush(stdout);
        }
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("\nStress Test Results:\n");
    printf("  Successful: %d\n", success_count);
    printf("  Failed:     %d\n", failure_count);
    printf("  Time:       %.3f seconds\n", cpu_time);
    printf("  Rate:       %.1f ops/sec\n", iterations / cpu_time);
}

void print_usage(const char* prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  -t      Run all tests\n");
    printf("  -b      Test basic transitions\n");
    printf("  -d      Test degradation/recovery\n");
    printf("  -c      Test concurrent access\n");
    printf("  -z      Test memory zones\n");
    printf("  -s <n>  Run stress test with n iterations\n");
    printf("  -m      Show memory statistics\n");
    printf("  -h      Show this help\n");
}

int main(int argc, char* argv[]) {
    printf("===========================================\n");
    printf("   GosiUML Phenomenological Memory Test   \n");
    printf("   OBINexus Platform v1.0.0              \n");
    printf("===========================================\n");
    
    // Seed random number generator
    srand(time(NULL));
    
    if (argc < 2) {
        print_usage(argv[0]);
        return 0;
    }
    
    int opt;
    while ((opt = getopt(argc, argv, "tbdczs:mh")) != -1) {
        switch (opt) {
            case 't':
                // Run all tests
                test_basic_transitions();
                test_degradation_recovery();
                test_concurrent_access();
                test_memory_zones();
                run_stress_test(100);
                break;
                
            case 'b':
                test_basic_transitions();
                break;
                
            case 'd':
                test_degradation_recovery();
                break;
                
            case 'c':
                test_concurrent_access();
                break;
                
            case 'z':
                test_memory_zones();
                break;
                
            case 's':
                run_stress_test(atoi(optarg));
                break;
                
            case 'm':
                pheno_memory_stats();
                break;
                
            case 'h':
            default:
                print_usage(argv[0]);
                return opt != 'h';
        }
    }
    
    // Final cleanup
    pheno_memory_cleanup();
    
    printf("\n=== Test Suite Complete ===\n");
    return 0;
}
