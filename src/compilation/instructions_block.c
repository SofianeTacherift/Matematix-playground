//
// Created by sofiane on 17/09/2026.
//

#include "instructions_block.h"

instructions_block *new_instructions_block() {
    instructions_block *res=calloc(1, sizeof(instructions_block));
    res->instructions=new_instruction_array_list();
    return res;
}

long hash_instruction_block_address(instructions_block *ptr) {
    return (long) ptr;
}

bool equals_instruction_block_address(instructions_block *p1, instructions_block *p2) {
    return p1==p2;
}



void print_instruction_block(instructions_block block) {
    printf("block [ size : %d address : %p - jump_address : %p - instructions:\n", block.instructions->size, &block, (void*) block.jump);
    for (size_t i=0; i<block.instructions->size; i++) {
        print_instruction_readable(block.instructions->elements[i]);
    }
    printf("\n");
}



void print_instruction_block_recursive(instructions_block block) {
    print_instruction_block(block);

    if (block.next!=NULL) {
        print_instruction_block_recursive( *block.next);
    }
}



void print_instructions_block_readable(instructions_block block, size_t instruction_index) {
    for (size_t i=0; i<block.instructions->size; i++) {
        printf("%zu ", instruction_index);
        print_instruction_readable(block.instructions->elements[i]);
        instruction_index++;
    }
    if (block.next!=NULL) {
        print_instructions_block_readable(*block.next, instruction_index);
    }

}

instructions_block *last_instruction_block_from_instruction(instructions_block *start) {
    instructions_block *current=start;
    while (current->next!=NULL) {
        current=current->next;
    }
    return current;
}


instructions_block *new_boolean_block_push(bool t) {
    instructions_block *result=new_instructions_block();
    add_instruction(result->instructions , (instruction) {.type = ICONST_INSTRUCTION , .operand1 = t ? 1 : 0} );
    if (!t) {
        result->type=FALSE_INSTRUCTION_CONST;
    }
    return result;
}