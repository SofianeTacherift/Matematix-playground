//
// Created by sofiane on 16/09/2026.
//

#ifndef MATEMATIX_INSTRUCTIONS_BLOCK_H
#define MATEMATIX_INSTRUCTIONS_BLOCK_H

#include "instructions.h"


typedef struct instructions_block {
    struct instructions_block *previous;
    struct instructions_block *next;
    struct instructions_block *jump;
    instruction_array_list * instructions;
} instructions_block;

instructions_block *new_instructions_block();

void print_instruction_block(instructions_block block);


#endif //MATEMATIX_INSTRUCTIONS_BLOCK_H
