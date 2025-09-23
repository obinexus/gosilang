#include "phenomemory_platform.h"

// Object-to-Object mapping function
void map_obj_to_obj(PhenoRelation* src, PhenoRelation* dst) {
    // XOR for differential mapping
    dst->subject_id = src->subject_id ^ dst->subject_id;
    dst->class_id = src->class_id ^ dst->class_id;
    
    // Bit-level merging
    dst->instance_state |= src->instance_state;
    dst->person_state = ROTATE_LEFT(src->person_state, 2);
}

// Person-to-Person model implementation
void apply_person_model(PhenoRelation* rel, uint8_t person_a, uint8_t person_b) {
    // Set person IDs using bit operations
    rel->person_id = person_a;
    rel->person_role = person_b;
    
    // Calculate authority level using bit counting
    uint8_t xor_val = person_a ^ person_b;
    rel->person_auth = 0;
    while (xor_val) {
        rel->person_auth += xor_val & 1;
        xor_val >>= 1;
    }
    
    // Set state flags
    rel->person_state = 0;
    if (person_a & 0x01) BIT_SET(rel->person_state, 0); // Active
    if (person_b & 0x02) BIT_SET(rel->person_state, 1); // Connected
    if ((person_a ^ person_b) & 0x04) BIT_SET(rel->person_state, 2); // Differential
}
