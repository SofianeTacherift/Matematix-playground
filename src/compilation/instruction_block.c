//
// Created by sofiane on 17/09/2026.
//

#include "instruction_block.h"

InstructionBlock* InstructionBlock_new() {
    InstructionBlock* res = calloc(1, sizeof(InstructionBlock));
    res->instructions = InstructionArrayList_new();
    return res;
}

long InstructionBlock_hash_address(const InstructionBlock* ptr) {
    return (long)ptr;
}

bool InstructionBlock_equals_address(const InstructionBlock* p1, const InstructionBlock* p2) {
    return p1 == p2;
}


void InstructionBlock_print(InstructionBlock* block) {
    if (block == NULL) {
        printf("NULL\n");
        return;
    }
    printf("block [ size : %zu address : %p - jump_address : %p - instructions:\n", block->instructions->size,
           (void*)block, (void*)block->jump);
    for (size_t i = 0; i < block->instructions->size; i++) {
        Instruction_print_readable(block->instructions->elements[i]);
    }
    printf("\n");
}


void InstructionBlock_print_recursive(InstructionBlock* block) {
    InstructionBlock_print(block);

    if (block != NULL && block->next != NULL) {
        InstructionBlock_print_recursive(block->next);
    }
}


void InstructionBlock_print_readable(InstructionBlock* block, size_t instruction_index) {
    if (block == NULL) {
        printf("NULL\n");
        return;
    }
    for (size_t i = 0; i < block->instructions->size; i++) {
        printf("%zu ", instruction_index);
        Instruction_print_readable(block->instructions->elements[i]);
        instruction_index++;
    }
    if (block->next != NULL) {
        InstructionBlock_print_readable(block->next, instruction_index);
    }
}

InstructionBlock* last_instruction_block_from_instruction_block(InstructionBlock* start) {
    InstructionBlock* current = start;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}


InstructionBlock* InstructionBlock_new_boolean_push(bool t) {
    InstructionBlock* result = InstructionBlock_new();
    InstructionArrayList_add(result->instructions, (Instruction){.type = ICONST_INSTRUCTION, .operand1 = t ? 1 : 0});
    return result;
}

void InstructionBlock_free_recursive(InstructionBlock* block) {
    if (block == NULL) {
        return;
    }
    InstructionBlock_free_recursive(block->next);
    InstructionArrayList_free(block->instructions);
    free(block);
}

void InstructionBlock_save_recursive(const InstructionBlock* block, const FILE* file) {
    if (block == NULL || file == NULL) return;
    Instruction_save_array(block->instructions->elements, block->instructions->size, file);
    InstructionBlock_save_recursive(block->next, file);
}
