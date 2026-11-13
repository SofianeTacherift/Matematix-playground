//
// Created by sofiane on 16/09/2026.
//

#ifndef MATEMATIX_INSTRUCTIONS_BLOCK_H
#define MATEMATIX_INSTRUCTIONS_BLOCK_H

#include "instruction.h"


typedef enum InstructionBlockType {
    NORMAL_INSTRUCTION,
    FALSE_INSTRUCTION_CONST
} InstructionBlockType;

typedef struct InstructionBlock {
    struct InstructionBlock* next;
    struct InstructionBlock* jump;
    InstructionBlockType type;
    InstructionArrayList* instructions;
} InstructionBlock;

InstructionBlock* InstructionBlock_new();
void InstructionBlock_free_recursive(InstructionBlock* block);
long InstructionBlock_hash_address(const InstructionBlock* ptr);
void InstructionBlock_print_readable(InstructionBlock* block, size_t);
bool InstructionBlock_equals_address(const InstructionBlock* p1, const InstructionBlock* p2);
void InstructionBlock_print(InstructionBlock* block);
void InstructionBlock_print_recursive(InstructionBlock* block);
InstructionBlock* last_instruction_block_from_instruction_block(InstructionBlock* current);
InstructionBlock* InstructionBlock_new_boolean_push(bool t);
void InstructionBlock_save_recursive(const InstructionBlock* block, const FILE* file);

#endif //MATEMATIX_INSTRUCTIONS_BLOCK_H
