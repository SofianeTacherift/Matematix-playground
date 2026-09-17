//
// Created by sofiane on 13/09/2026.
//
#include "instructions.h"

int binary_node_to_instruction_type(parsing_node *node) {
    switch (node->operation) {
        case ADD_OPERATOR:
            return BINARY_ADD_INSTRUCTION;
        case MULTIPLY_OPERATOR:
            return BINARY_MULT_INSTRUCTION;
        default:
            return NONE_INSTRUCTION;
    }
}

void print_instruction_readable(instruction instruction) {
    printf("%s", INSTRUCTION_TYPE_STR[instruction.type]);
    switch (instruction.type) {
        case ICONST_INSTRUCTION:
            printf(" %d", (int) instruction.operand1);
            break;
        case DCONST_INSTRUCTION:
            printf(" %lf", (double) instruction.operand1);
            break;
        case OLOAD_INSTRUCTION:
        case OSTORE_INSTRUCTION:
            printf(" %zu", instruction.operand1);
            break;
        default:
            break;
    }
    printf("\n");

}
