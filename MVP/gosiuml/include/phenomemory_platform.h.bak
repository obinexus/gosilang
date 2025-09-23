#ifndef PHENOMEMORY_PLATFORM_H
#define PHENOMEMORY_PLATFORM_H

#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <pthread.h>

// Forward declarations
typedef struct PhenoToken PhenoToken;
typedef struct StateMachine StateMachine;

// State enumeration - single definition
typedef enum {
    STATE_NIL,
    STATE_ALLOCATED,
    STATE_LOCKED,
    STATE_ACTIVE,
    STATE_DEGRADED,
    STATE_SHARED,
    STATE_FREED
} PhenoState;

// Event types for state machine
typedef enum {
    EVENT_ALLOC,
    EVENT_LOCK,
    EVENT_UNLOCK,
    EVENT_VALIDATE,
    EVENT_DEGRADE,
    EVENT_RECOVER,
    EVENT_SHARE,
    EVENT_FREE
} PhenoEvent;

// Substates for ACTIVE state
typedef enum {
    SUBSTATE_NONE,
    SUBSTATE_READING,
    SUBSTATE_WRITING,
    SUBSTATE_TRANSFORMING
} PhenoSubstate;

// Memory zones
#define MAX_MEMORY_ZONES 16
#define ZONE_MASK 0x0F

// Bitfield positions for atomic flags
#define FLAG_NIL_BIT        0
#define FLAG_ALLOCATED_BIT  1
#define FLAG_LOCKED_BIT     2
#define FLAG_DIRTY_BIT      3
#define FLAG_COHERENT_BIT   4
#define FLAG_PROCESSING_BIT 5
#define FLAG_SHARED_BIT     6

// Reference count position and mask
#define REF_COUNT_SHIFT 8
#define REF_COUNT_MASK  0xFF00

// Degradation score position
#define DEGRADATION_SHIFT 16
#define DEGRADATION_MASK  0x3FF0000

// Memory flags using atomic for thread safety
typedef struct {
    atomic_uint32_t flags;  // Combined flags for atomic operations
    atomic_uint32_t ref_count;
    atomic_uint32_t degradation_metrics;
} MemFlags;

// Pheno Token structure
struct PhenoToken {
    uint32_t token_id;
    char sentinel[16];  // "PHENO_NIL", etc.
    uint8_t memory_zone;
    MemFlags mem_flags;
    pthread_t thread_owner;
    void* data_ptr;
    size_t data_size;
};

// State Machine structure
struct StateMachine {
    PhenoState current_state;
    PhenoSubstate current_substate;
    PhenoToken* token;
    pthread_mutex_t mutex;
    pthread_spinlock_t spinlock;
    uint32_t retry_count;
    float confidence_score;
    bool is_initialized;
};

// Transition function type
typedef bool (*TransitionFunc)(StateMachine*, PhenoEvent);

// Atomic flag operations (inline for performance)
static inline void set_flag(MemFlags* flags, int bit) {
    atomic_fetch_or(&flags->flags, (1U << bit));
}

static inline void clear_flag(MemFlags* flags, int bit) {
    atomic_fetch_and(&flags->flags, ~(1U << bit));
}

static inline bool test_flag(MemFlags* flags, int bit) {
    return (atomic_load(&flags->flags) & (1U << bit)) != 0;
}

static inline bool test_and_set_flag(MemFlags* flags, int bit) {
    uint32_t old_val = atomic_fetch_or(&flags->flags, (1U << bit));
    return (old_val & (1U << bit)) == 0;
}

// Reference count operations
static inline void increment_ref_count(MemFlags* flags) {
    atomic_fetch_add(&flags->ref_count, 1);
}

static inline uint32_t decrement_ref_count(MemFlags* flags) {
    return atomic_fetch_sub(&flags->ref_count, 1) - 1;
}

static inline uint32_t get_ref_count(MemFlags* flags) {
    return atomic_load(&flags->ref_count);
}

// Function declarations
StateMachine* create_state_machine(void);
void destroy_state_machine(StateMachine* sm);
bool initialize_state_machine(StateMachine* sm);
void step_state_machine(StateMachine* sm, PhenoEvent event);
const char* get_state_name(PhenoState state);
const char* get_event_name(PhenoEvent event);

// Token operations
PhenoToken* pheno_token_alloc(uint32_t size);
void pheno_token_free(PhenoToken* token);
bool pheno_token_lock(PhenoToken* token);
void pheno_token_unlock(PhenoToken* token);
bool pheno_token_validate(PhenoToken* token);

// Verification and recovery
bool verify_geometric_proof(PhenoToken* token);
bool verify_integrity(StateMachine* sm);
void initiate_recovery(StateMachine* sm);
void attempt_hitl_recovery(StateMachine* sm);
void cleanup_resources(StateMachine* sm);
void reset_degradation_metrics(StateMachine* sm);

// Utility functions
bool memory_available(void);
void assign_token_id(PhenoToken* token);
void process_token_operations(StateMachine* sm);

#endif // PHENOMEMORY_PLATFORM_H
