//
// Created by sofiane on 06/09/2026.
//


#ifndef MATEMATIX_COMPILATOR_H
#define MATEMATIX_COMPILATOR_H

#include "parser.h"

typedef enum {
    OLOAD,
    OSTORE,
    BINARY_ADD,
    BINARY_MULT,
} instruction_type;

typedef struct {
    instruction_type type;
    long left;
    long right;
} instruction;


ARRAY_LIST(instruction, instruction)


typedef struct compiler {
    int current_index;
    instruction_array_list *instructions;
    parsing_node *head;

} compilator;

#endif //MATEMATIX_COMPILATOR_H
