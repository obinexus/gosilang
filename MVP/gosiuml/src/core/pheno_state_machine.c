#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "phenomemory_platform.h"

// State name lookup
const char* get_state_name(PhenoState state) {
    static const char* state_names[] = {
        "NIL", "ALLOCATED", "LOCKED", "ACTIVE",
        "DEGRADED", "SHARED", "FREED"
    };
    if (state >= 0 && state <= STATE_FREED) {
        return state_names[state];
    }
    return "UNKNOWN";
}

// Event name lookup
const char* get_event_name(PhenoEvent event) {
    static const char* event_names[] = {
        "ALLOC", "LOCK", "UNLOCK", "VALIDATE",
        "DEGRADE", "RECOVER", "SHARE", "FREE"
    };
    if (event >= 0 && event <= EVENT_FREE) {
        return event_names[event];
    }
    return "UNKNOWN";
}

// Create state machine
StateMachine* create_state_machine(void) {
    StateMachine* sm = (StateMachine*)calloc(1, sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->current_state = STATE_NIL;
    sm->current_substate = SUBSTATE_NONE;
    sm->retry_count = 0;
    sm->confidence_score = 1.0f;
    sm->is_initialized = false;
    
    pthread_mutex_init(&sm->mutex, NULL);
    pthread_spin_init(&sm->spinlock, PTHREAD_PROCESS_PRIVATE);
    
    return sm;
}

// Initialize state machine
bool initialize_state_machine(StateMachine* sm) {
    if (!sm) return false;
    
    sm->token = pheno_token_alloc(4096);  // Default size
    if (!sm->token) return false;
    
    sm->is_initialized = true;
    return true;
}

// Destroy state machine
void destroy_state_machine(StateMachine* sm) {
    if (!sm) return;
    
    if (sm->token) {
        pheno_token_free(sm->token);
    }
    
    pthread_mutex_destroy(&sm->mutex);
    pthread_spin_destroy(&sm->spinlock);
    free(sm);
}

// Transition: NIL -> ALLOCATED
static bool transition_nil_to_allocated(StateMachine* sm) {
    if (!memory_available()) return false;
    
    sm->token = pheno_token_alloc(4096);
    if (!sm->token) return false;
    
    assign_token_id(sm->token);
    set_flag(&sm->token->mem_flags, FLAG_ALLOCATED_BIT);
    sm->current_state = STATE_ALLOCATED;
    
    printf("[TRANSITION] NIL -> ALLOCATED (token_id: 0x%08X)\n", 
           sm->token->token_id);
    return true;
}

// Transition: ALLOCATED -> LOCKED
static bool transition_allocated_to_locked(StateMachine* sm) {
    if (!sm->token) return false;
    
    // Atomically set the locked flag
    if (test_and_set_flag(&sm->token->mem_flags, FLAG_LOCKED_BIT)) {
        return false;  // Already locked
    }
    
    pthread_spin_lock(&sm->spinlock);
    sm->token->thread_owner = pthread_self();
    sm->current_state = STATE_LOCKED;
    
    printf("[TRANSITION] ALLOCATED -> LOCKED (thread: %lu)\n",
           (unsigned long)sm->token->thread_owner);
    return true;
}

// Transition: LOCKED -> ACTIVE
static bool transition_locked_to_active(StateMachine* sm) {
    if (!verify_geometric_proof(sm->token)) return false;
    
    set_flag(&sm->token->mem_flags, FLAG_COHERENT_BIT);
    set_flag(&sm->token->mem_flags, FLAG_PROCESSING_BIT);
    sm->current_state = STATE_ACTIVE;
    sm->current_substate = SUBSTATE_READING;
    
    printf("[TRANSITION] LOCKED -> ACTIVE\n");
    return true;
}

// Transition: ACTIVE -> DEGRADED
static bool transition_active_to_degraded(StateMachine* sm) {
    float degradation_score = (float)sm->retry_count / 100.0f;
    
    if (degradation_score <= 0.6f) return false;
    
    clear_flag(&sm->token->mem_flags, FLAG_COHERENT_BIT);
    sm->current_state = STATE_DEGRADED;
    initiate_recovery(sm);
    
    printf("[TRANSITION] ACTIVE -> DEGRADED (score: %.2f)\n", 
           degradation_score);
    return true;
}

// Transition: DEGRADED -> ACTIVE
static bool transition_degraded_to_active(StateMachine* sm) {
    if (!verify_integrity(sm)) return false;
    
    reset_degradation_metrics(sm);
    set_flag(&sm->token->mem_flags, FLAG_COHERENT_BIT);
    sm->current_state = STATE_ACTIVE;
    
    printf("[TRANSITION] DEGRADED -> ACTIVE (recovered)\n");
    return true;
}

// Transition: DEGRADED -> FREED
static bool transition_degraded_to_freed(StateMachine* sm) {
    if (sm->retry_count < 63) return false;
    
    cleanup_resources(sm);
    clear_flag(&sm->token->mem_flags, FLAG_ALLOCATED_BIT);
    sm->current_state = STATE_FREED;
    
    printf("[TRANSITION] DEGRADED -> FREED (max retries)\n");
    return true;
}

// Transition: ACTIVE -> SHARED
static bool transition_active_to_shared(StateMachine* sm) {
    increment_ref_count(&sm->token->mem_flags);
    set_flag(&sm->token->mem_flags, FLAG_SHARED_BIT);
    sm->current_state = STATE_SHARED;
    
    printf("[TRANSITION] ACTIVE -> SHARED (ref_count: %u)\n",
           get_ref_count(&sm->token->mem_flags));
    return true;
}

// Transition: ANY -> FREED
static bool transition_to_freed(StateMachine* sm) {
    cleanup_resources(sm);
    
    if (sm->token) {
        pheno_token_free(sm->token);
        sm->token = NULL;
    }
    
    sm->current_state = STATE_FREED;
    printf("[TRANSITION] %s -> FREED\n", 
           get_state_name(sm->current_state));
    return true;
}

// Main state machine step function
void step_state_machine(StateMachine* sm, PhenoEvent event) {
    if (!sm || !sm->is_initialized) return;
    
    pthread_mutex_lock(&sm->mutex);
    
    bool transition_success = false;
    PhenoState old_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_NIL:
            if (event == EVENT_ALLOC) {
                transition_success = transition_nil_to_allocated(sm);
            }
            break;
            
        case STATE_ALLOCATED:
            if (event == EVENT_LOCK) {
                transition_success = transition_allocated_to_locked(sm);
            } else if (event == EVENT_FREE) {
                transition_success = transition_to_freed(sm);
            }
            break;
            
        case STATE_LOCKED:
            if (event == EVENT_VALIDATE) {
                transition_success = transition_locked_to_active(sm);
            } else if (event == EVENT_UNLOCK) {
                clear_flag(&sm->token->mem_flags, FLAG_LOCKED_BIT);
                pthread_spin_unlock(&sm->spinlock);
                sm->current_state = STATE_ALLOCATED;
                transition_success = true;
            }
            break;
            
        case STATE_ACTIVE:
            if (event == EVENT_DEGRADE) {
                transition_success = transition_active_to_degraded(sm);
            } else if (event == EVENT_SHARE) {
                transition_success = transition_active_to_shared(sm);
            } else if (event == EVENT_FREE) {
                transition_success = transition_to_freed(sm);
            }
            break;
            
        case STATE_DEGRADED:
            if (event == EVENT_RECOVER) {
                transition_success = transition_degraded_to_active(sm);
            } else if (event == EVENT_FREE) {
                transition_success = transition_degraded_to_freed(sm);
            }
            sm->retry_count++;
            break;
            
        case STATE_SHARED:
            if (event == EVENT_FREE) {
                uint32_t refs = decrement_ref_count(&sm->token->mem_flags);
                if (refs == 0) {
                    transition_success = transition_to_freed(sm);
                }
            }
            break;
            
        case STATE_FREED:
            // Terminal state
            break;
    }
    
    if (transition_success) {
        printf("[STATE_MACHINE] %s + %s -> %s\n",
               get_state_name(old_state),
               get_event_name(event),
               get_state_name(sm->current_state));
    }
    
    pthread_mutex_unlock(&sm->mutex);
}

// Placeholder implementations for utility functions
bool memory_available(void) {
    // Check available memory
    return true;
}

void assign_token_id(PhenoToken* token) {
    static atomic_uint32_t next_id = ATOMIC_VAR_INIT(0x10000000);
    token->token_id = atomic_fetch_add(&next_id, 1);
    snprintf(token->sentinel, 16, "PHENO_%08X", token->token_id);
}

bool verify_geometric_proof(PhenoToken* token) {
    // Implement geometric proof verification
    return token && test_flag(&token->mem_flags, FLAG_ALLOCATED_BIT);
}

bool verify_integrity(StateMachine* sm) {
    // Implement integrity verification
    return sm && sm->token && sm->confidence_score > 0.3f;
}

void initiate_recovery(StateMachine* sm) {
    printf("[RECOVERY] Initiating recovery process...\n");
    sm->confidence_score *= 0.9f;
}

void attempt_hitl_recovery(StateMachine* sm) {
    printf("[HITL] Human-in-the-loop recovery attempt %u/63\n", 
           sm->retry_count);
}

void cleanup_resources(StateMachine* sm) {
    printf("[CLEANUP] Releasing resources...\n");
    if (sm->token) {
        clear_flag(&sm->token->mem_flags, FLAG_ALLOCATED_BIT);
        clear_flag(&sm->token->mem_flags, FLAG_LOCKED_BIT);
        clear_flag(&sm->token->mem_flags, FLAG_PROCESSING_BIT);
    }
}

void reset_degradation_metrics(StateMachine* sm) {
    sm->retry_count = 0;
    sm->confidence_score = 1.0f;
    atomic_store(&sm->token->mem_flags.degradation_metrics, 0);
}

void process_token_operations(StateMachine* sm) {
    if (!sm || !sm->token) return;
    
    switch (sm->current_substate) {
        case SUBSTATE_READING:
            printf("[PROCESS] Reading token data...\n");
            break;
        case SUBSTATE_WRITING:
            printf("[PROCESS] Writing token data...\n");
            break;
        case SUBSTATE_TRANSFORMING:
            printf("[PROCESS] Transforming token data...\n");
            break;
        default:
            break;
    }
}
