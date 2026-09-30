//
// Created by sofiane on 16/09/2026.
//

#ifndef MATEMATIX_INSTRUCTIONS_BLOCK_H
#define MATEMATIX_INSTRUCTIONS_BLOCK_H

#include "instruction.h"


typedef enum instruction_block_type {
    NORMAL_INSTRUCTION,
    FALSE_INSTRUCTION_CONST

} instruction_block_type;
typedef struct instruction_block {
    struct instruction_block *previous;
    struct instruction_block *next;
    struct instruction_block *jump;
    instruction_block_type type;
    instruction_array_list * instructions;
} instruction_block;

instruction_block *new_instructions_block();

long hash_instruction_block_address(instruction_block *ptr);

void print_instructions_block_readable(instruction_block block, size_t) ;

bool equals_instruction_block_address(instruction_block *p1, instruction_block *p2);

void print_instruction_block(instruction_block block);

void print_instruction_block_recursive(instruction_block block);

instruction_block *last_instruction_block_from_instruction_block(instruction_block *current);

instruction_block *new_boolean_block_push(bool t);


#endif //MATEMATIX_INSTRUCTIONS_BLOCK_H
