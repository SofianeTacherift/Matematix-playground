//
// Created by sofiane on 13/09/2026.
//
#include "instruction.h"

static int operands_per_type[]= {
    0,
    1,
    1,
    1,
    1,
    0, //BINARY_ADD_INSTRUCTION,
    0, //BINARY_SUB_INSTRUCTION,
    0, //BINARY_MULT_INSTRUCTION,
    0, //BINARY_DIVIDE_INSTRUCTION,
    0, //BINARY_POWER_INSTRUCTION,
    0, //UNARY_MINUS_INSTRUCTION,
    1, //GOTO,
    1, //IF_CMPEQ,
    1,//IF_CMPNE,
    1,//IF_CMPGT,
    1,//IF_CMPLT,
    1,//IF_CMPGTE,
    1, //IF_CMPTLTE,
    0, //RETURN_INSTRUCTION
};
static int comparison_operator_to_instr_type_buffer[] = {
    IF_CMPEQ,
    IF_CMPNE,
    IF_CMPGT,
    IF_CMPGTE,
    IF_CMPLT,
    IF_CMPTLTE,
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



size_t write_instruction_in_buffer(const instruction element, char * dest) {
    size_t written_n=0;
    memcpy(dest, &element, sizeof element.type);
    written_n+=sizeof element.type;

    if (operands_per_type[element.type]>=1) {
        memcpy(dest, & element.operand1, sizeof element.operand1);
        written_n+=sizeof element.operand1;
    }

    if (operands_per_type[element.type]>=2) {
        memcpy(dest, & element.operand2, sizeof element.operand2);
        written_n+=sizeof element.operand2;
    }

    return written_n;

}
void save_instruction_list(const instruction *elements, const size_t end, FILE *file) {
    char *bytes = malloc(sizeof(instruction) * end);
    size_t n_written = 0;
    for (size_t i=0; i<end; i++) {
        instruction t = elements[i];
         n_written+= write_instruction_in_buffer(t, bytes+n_written);
    }
    fwrite(bytes, sizeof (char), n_written, file);
    free(bytes);
}


