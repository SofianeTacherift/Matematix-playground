//
// Created by sofiane on 17/09/2026.
//

#include "instruction_block.h"

instruction_block *new_instructions_block() {
    instruction_block *res=calloc(1, sizeof(instruction_block));
    res->instructions=new_instruction_array_list();
    return res;
}

long hash_instruction_block_address(instruction_block *ptr) {
    return (long) ptr;
}

bool equals_instruction_block_address(instruction_block *p1, instruction_block *p2) {
    return p1==p2;
}



void print_instruction_block(instruction_block *block) {
    if (block==NULL) {
        printf("NULL\n");
        return;
    }
    printf("block [ size : %zu address : %p - jump_address : %p - instructions:\n", block->instructions->size, (void*) block, (void*) block->jump);
    for (size_t i=0; i<block->instructions->size; i++) {
        print_instruction_readable(block->instructions->elements[i]);
    }
    printf("\n");
}



void print_instruction_block_recursive(instruction_block *block) {
    print_instruction_block(block);

    if ( block!=NULL && block->next!=NULL) {
        print_instruction_block_recursive( block->next);
    }
}



void print_instruction_block_readable(instruction_block *block, size_t instruction_index) {
    if (block==NULL) {
        printf("NULL\n");
        return;
    }

    for (size_t i=0; i<block->instructions->size; i++) {
        printf("%zu ", instruction_index);
        print_instruction_readable(block->instructions->elements[i]);
        instruction_index++;
    }
    if (block->next!=NULL) {
        print_instruction_block_readable(block->next, instruction_index);
    }

}

instruction_block *last_instruction_block_from_instruction_block(instruction_block *start) {
    instruction_block *current=start;
    while (current->next!=NULL) {
        current=current->next;
    }
    return current;
}


instruction_block *new_boolean_block_push(bool t) {
    instruction_block *result=new_instructions_block();
    add_instruction(result->instructions , (instruction) {.type = ICONST_INSTRUCTION , .operand1 = t ? 1 : 0} );
    return result;
}

void free_instruction_block_recursive(instruction_block *block) {
    if (block==NULL) {
        return;
    }

    free_instruction_block_recursive(block->next);

    free_instruction_array_list(block->instructions);
    free(block);

}

void save_instruction_block_recursive(const instruction_block *block, const FILE *file) {
    if (block==NULL || file==NULL) return;
    save_instruction_list(block->instructions->elements, block->instructions->size, file);
    save_instruction_block_recursive(block->next, file);

}