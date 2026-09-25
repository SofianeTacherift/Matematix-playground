//
// Created by sofiane on 13/09/2026.
//

#ifndef MATEMATIX_INSTRUCTIONS_H
#define MATEMATIX_INSTRUCTIONS_H
#include <stdlib.h>
#include "../data_structures/array_list.h"
#include "../data_structures/parsing_node.h"

typedef enum {
    NONE_INSTRUCTION,
    ICONST_INSTRUCTION,
    DCONST_INSTRUCTION,
    OLOAD_INSTRUCTION,
    OSTORE_INSTRUCTION,
    BINARY_ADD_INSTRUCTION,
    BINARY_SUB_INSTRUCTION,
    BINARY_MULT_INSTRUCTION,
    BINARY_DIVIDE_INSTRUCTION,
    BINARY_POWER_INSTRUCTION,
    UNARY_MINUS_INSTRUCTION,
    GOTO,
    IF_CMPEQ,
    IF_CMPNE,
    IF_CMPGT,
    IF_CMPLT,
    IF_CMPGTE,
    IF_CMPTLTE
} instruction_type;

static char * INSTRUCTION_TYPE_STR[] = {
    "NONE_INSTRUCTION",
     "ICONST",
     "DCONST",
     "OLOAD",
     "OSTORE",
     "BINARY_ADD",
    "BINARY_SUB",
     "BINARY_MULT",
    "BINARY_DIVIDE",
    "BINARY_POWER",
    "UNARY_MINUS",
    "GOTO",
    "IF_CMPEQ",
    "IF_CMPNE",
    "IF_CMPGT",
    "IF_CMPLT",
    "IF_CMPGTE",
    "IF_CMPTLTE"
};


typedef struct {
    instruction_type type;
    size_t operand1;
    size_t operand2;
} instruction;

ARRAY_LIST(instruction, instruction)


// functions

instruction_type binary_arithmetic_node_to_instruction_type(parsing_node *node);

instruction_type unary_arithmetic_node_to_instruction_type( parsing_node *node);

int comparison_operator_to_instruction_type(int t);


void print_instruction_readable(instruction instruction);






#endif //MATEMATIX_INSTRUCTIONS_H
