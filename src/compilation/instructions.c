//
// Created by sofiane on 13/09/2026.
//
#include "instructions.h"
#include <stdio.h>


static int comparison_operator_to_instr_type_buffer[] = {
    IF_CMPEQ,
    IF_CMPNE,
    IF_CMPGT,
    IF_CMPGTE,
    IF_CMPLT,
    IF_CMPTLTE,
};

static int instruction_operands_number[] = {
    0,
    1,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    0
};


instruction_type binary_arithmetic_node_to_instruction_type(parsing_node *node) {
    switch (node->operation) {
        case ADD_OPERATOR:
            return BINARY_ADD_INSTRUCTION;
        case SUB_OPERATOR:
            return BINARY_SUB_INSTRUCTION;
        case MULTIPLY_OPERATOR:
            return BINARY_MULT_INSTRUCTION;
        case DIVIDE_OPERATOR:
            return BINARY_DIVIDE_INSTRUCTION;
        case POWER_OPERATOR:
            return BINARY_POWER_INSTRUCTION;

        default:
            return NONE_INSTRUCTION;
    }
}

instruction_type unary_arithmetic_node_to_instruction_type( parsing_node *node) {
    switch (node->operation) {
        case UNARY_MINUS_OPERATOR:
            return UNARY_MINUS_INSTRUCTION;
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

void save_instruction(instruction t, FILE *file) {
    size_t instruction_bytes[3];
    instruction_bytes[0]=t.type;
    instruction_bytes[1]=t.operand1;
    instruction_bytes[2]=t.operand2;


    const size_t n= (size_t) instruction_operands_number[t.type]+1;
    size_t a=fwrite(instruction_bytes, sizeof(size_t), n, file);
}


