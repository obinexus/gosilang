#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include "phenomemory_platform.h"

// Global memory pool for phenomenological tokens
typedef struct {
    void* base_addr;
    size_t total_size;
    size_t used_size;
    atomic_uint32_t active_tokens;
    pthread_mutex_t pool_mutex;
} MemoryPool;

static MemoryPool g_pool = {0};

// Initialize memory pool
static void init_memory_pool(void) {
    static atomic_bool initialized = ATOMIC_VAR_INIT(false);
    
    if (atomic_exchange(&initialized, true)) return;
    
    g_pool.total_size = 16 * 1024 * 1024; // 16MB pool
    g_pool.base_addr = mmap(NULL, g_pool.total_size,
                            PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS,
                            -1, 0);
    
    if (g_pool.base_addr == MAP_FAILED) {
        perror("mmap failed");
        g_pool.base_addr = malloc(g_pool.total_size);
    }
    
    g_pool.used_size = 0;
    atomic_store(&g_pool.active_tokens, 0);
    pthread_mutex_init(&g_pool.pool_mutex, NULL);
}

// Allocate a phenomenological token
PhenoToken* pheno_token_alloc(uint32_t size) {
    init_memory_pool();
    
    pthread_mutex_lock(&g_pool.pool_mutex);
    
    // Check if we have space
    if (g_pool.used_size + sizeof(PhenoToken) + size > g_pool.total_size) {
        pthread_mutex_unlock(&g_pool.pool_mutex);
        return NULL;
    }
    
    // Allocate token structure
    PhenoToken* token = (PhenoToken*)calloc(1, sizeof(PhenoToken));
    if (!token) {
        pthread_mutex_unlock(&g_pool.pool_mutex);
        return NULL;
    }
    
    // Allocate data buffer from pool
    token->data_ptr = (uint8_t*)g_pool.base_addr + g_pool.used_size;
    token->data_size = size;
    g_pool.used_size += size;
    
    // Initialize token
    strncpy(token->sentinel, "PHENO_NIL", 16);
    token->memory_zone = g_pool.used_size / (g_pool.total_size / MAX_MEMORY_ZONES);
    
    // Initialize atomic flags
    atomic_store(&token->mem_flags.flags, 0);
    atomic_store(&token->mem_flags.ref_count, 1);
    atomic_store(&token->mem_flags.degradation_metrics, 0);
    
    // Set allocated flag
    set_flag(&token->mem_flags, FLAG_ALLOCATED_BIT);
    
    atomic_fetch_add(&g_pool.active_tokens, 1);
    
    pthread_mutex_unlock(&g_pool.pool_mutex);
    
    printf("[ALLOC] Token allocated: size=%u, zone=%u, addr=%p\n",
           size, token->memory_zone, token->data_ptr);
    
    return token;
}

// Free a phenomenological token
void pheno_token_free(PhenoToken* token) {
    if (!token) return;
    
    pthread_mutex_lock(&g_pool.pool_mutex);
    
    // Clear sensitive data
    if (token->data_ptr && token->data_size > 0) {
        memset(token->data_ptr, 0, token->data_size);
    }
    
    // Clear flags
    atomic_store(&token->mem_flags.flags, 0);
    atomic_store(&token->mem_flags.ref_count, 0);
    
    uint32_t active = atomic_fetch_sub(&g_pool.active_tokens, 1) - 1;
    
    printf("[FREE] Token freed: id=0x%08X, remaining=%u\n",
           token->token_id, active);
    
    free(token);
    
    pthread_mutex_unlock(&g_pool.pool_mutex);
}

// Lock a token for exclusive access
bool pheno_token_lock(PhenoToken* token) {
    if (!token) return false;
    
    // Try to atomically set the lock bit
    if (test_and_set_flag(&token->mem_flags, FLAG_LOCKED_BIT)) {
        return false; // Already locked
    }
    
    token->thread_owner = pthread_self();
    printf("[LOCK] Token locked by thread %lu\n",
           (unsigned long)token->thread_owner);
    
    return true;
}

// Unlock a token
void pheno_token_unlock(PhenoToken* token) {
    if (!token) return;
    
    // Check if current thread owns the lock
    if (token->thread_owner != pthread_self()) {
        printf("[UNLOCK] Warning: thread %lu trying to unlock token owned by %lu\n",
               (unsigned long)pthread_self(),
               (unsigned long)token->thread_owner);
        return;
    }
    
    clear_flag(&token->mem_flags, FLAG_LOCKED_BIT);
    token->thread_owner = 0;
    
    printf("[UNLOCK] Token unlocked\n");
}

// Validate token integrity
bool pheno_token_validate(PhenoToken* token) {
    if (!token) return false;
    
    // Check sentinel
    if (strncmp(token->sentinel, "PHENO_", 6) != 0) {
        printf("[VALIDATE] Invalid sentinel: %s\n", token->sentinel);
        return false;
    }
    
    // Check memory zone
    if (token->memory_zone >= MAX_MEMORY_ZONES) {
        printf("[VALIDATE] Invalid memory zone: %u\n", token->memory_zone);
        return false;
    }
    
    // Check flags consistency
    uint32_t flags = atomic_load(&token->mem_flags.flags);
    if ((flags & (1 << FLAG_NIL_BIT)) && (flags & (1 << FLAG_ALLOCATED_BIT))) {
        printf("[VALIDATE] Inconsistent flags: NIL and ALLOCATED both set\n");
        return false;
    }
    
    // Check data pointer alignment
    if (token->data_ptr && ((uintptr_t)token->data_ptr & 0x7) != 0) {
        printf("[VALIDATE] Misaligned data pointer: %p\n", token->data_ptr);
        return false;
    }
    
    printf("[VALIDATE] Token valid: id=0x%08X\n", token->token_id);
    return true;
}

// Get memory pool statistics
void pheno_memory_stats(void) {
    init_memory_pool();
    
    pthread_mutex_lock(&g_pool.pool_mutex);
    
    printf("\n=== Phenomenological Memory Statistics ===\n");
    printf("Total Pool Size:  %zu bytes\n", g_pool.total_size);
    printf("Used Pool Size:   %zu bytes (%.1f%%)\n",
           g_pool.used_size,
           (double)g_pool.used_size / g_pool.total_size * 100.0);
    printf("Active Tokens:    %u\n", atomic_load(&g_pool.active_tokens));
    printf("Memory Zones:     %d\n", MAX_MEMORY_ZONES);
    printf("Base Address:     %p\n", g_pool.base_addr);
    printf("==========================================\n\n");
    
    pthread_mutex_unlock(&g_pool.pool_mutex);
}

// Cleanup memory pool (called at exit)
void pheno_memory_cleanup(void) {
    pthread_mutex_lock(&g_pool.pool_mutex);
    
    if (g_pool.base_addr) {
        // Try to unmap, fall back to free if it was malloc'd
        if (munmap(g_pool.base_addr, g_pool.total_size) != 0) {
            free(g_pool.base_addr);
        }
        g_pool.base_addr = NULL;
    }
    
    pthread_mutex_unlock(&g_pool.pool_mutex);
    pthread_mutex_destroy(&g_pool.pool_mutex);
    
    printf("[CLEANUP] Memory pool released\n");
}
