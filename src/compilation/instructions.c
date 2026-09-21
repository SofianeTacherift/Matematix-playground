//
// Created by sofiane on 13/09/2026.
//
#include "instructions.h"


static int comparison_operator_to_instr_type_buffer[] = {
    IF_CMPEQ,
    IF_CMPNE,
    IF_CMPGT,
    IF_CMPGTE,
    IF_CMPLT,
    IF_CMPTLTE,
};


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




int comparison_operator_to_instruction_type(int t) {
    if (t<EQUALS_OPERATOR || t>LESS_OR_EQUAL_OPERATOR) {
        return NONE_OPERATOR;
    }
    return IF_CMPEQ + t -EQUALS_OPERATOR;
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
        case IF_CMPEQ:
        case IF_CMPNE:
        case IF_CMPGT:
        case IF_CMPGTE:
        case IF_CMPTLTE:
        case IF_CMPLT:
        case GOTO:
            printf(" %zu", instruction.operand1);
            break;
        default:
            break;
    }
    printf("\n");

}


