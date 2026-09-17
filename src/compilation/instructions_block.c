//
// Created by sofiane on 17/09/2026.
//

#include "instructions_block.h"

instructions_block *new_instructions_block() {
    instructions_block *res=calloc(1, sizeof(instructions_block));
    res->instructions=new_instruction_array_list();
    return res;
}

void print_instruction_block(instructions_block block) {
    printf("block [ size : %d - instructions;\n", block.instructions->size);
    for (size_t i=0; i<block.instructions->size; i++) {
        print_instruction_readable(block.instructions->elements[i]);
    }
    printf("]\n");
}