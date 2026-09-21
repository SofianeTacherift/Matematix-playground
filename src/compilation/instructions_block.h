//
// Created by sofiane on 16/09/2026.
//

#ifndef MATEMATIX_INSTRUCTIONS_BLOCK_H
#define MATEMATIX_INSTRUCTIONS_BLOCK_H

#include "instructions.h"


typedef enum instructions_block_type {
    NORMAL_INSTRUCTION,
    FALSE_INSTRUCTION_CONST

} instructions_block_type;
typedef struct instructions_block {
    struct instructions_block *previous;
    struct instructions_block *next;
    struct instructions_block *jump;
    instructions_block_type type;
    instruction_array_list * instructions;
} instructions_block;

instructions_block *new_instructions_block();

long hash_instruction_block_address(instructions_block *ptr);

void print_instructions_block_readable(instructions_block block, size_t) ;

bool equals_instruction_block_address(instructions_block *p1, instructions_block *p2);

void print_instruction_block(instructions_block block);

void print_instruction_block_recursive(instructions_block block);

instructions_block *last_instruction_block_from_instruction(instructions_block *current);

instructions_block *new_boolean_block_push(bool t);


#endif //MATEMATIX_INSTRUCTIONS_BLOCK_H
