//
// Created by sofiane on 13/09/2026.
//
#include "instruction.h"


InstructionType InstructionType_from_binary_arithmetic_node(ParsingNode* node) {
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

InstructionType InstructionType_from_unary_arithmetic_node(ParsingNode* node) {
    switch (node->operation) {
    case UNARY_MINUS_OPERATOR:
        return UNARY_MINUS_INSTRUCTION;
    default:
        return NONE_INSTRUCTION;
    }
}


int InstructionType_from_comparison_operator(int t) {
    if (t < EQUALS_OPERATOR || t > LESS_OR_EQUAL_OPERATOR) {
        return NONE_OPERATOR;
    }
    return IF_CMPEQ + t - EQUALS_OPERATOR;
}

void Instruction_print_readable(Instruction instruction) {
    printf("%s", INSTRUCTION_TYPE_STR[instruction.type]);
    switch (instruction.type) {
    case ICONST_INSTRUCTION:
        printf(" %d", (int)instruction.operand1);
        break;
    case DCONST_INSTRUCTION:
        printf(" %lf", (double)instruction.operand1);
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


size_t Instruction_write_in_buffer(const Instruction element, char* dest) {
    size_t written_n = 0;
    memcpy(dest, &element.type, sizeof element.type);
    written_n += sizeof(InstructionType);

    if (operands_per_type[element.type] >= 1) {
        size_t operand1 = element.operand1;

        memcpy(dest + written_n, &operand1, sizeof(size_t));
        written_n += sizeof(size_t);
    }

    if (operands_per_type[element.type] >= 2) {
        memcpy(dest + written_n, &element.operand2, sizeof(size_t));
        written_n += sizeof element.operand2;
    }

    // Instruction_print_readable(element);
    // printf("%zu\n",((size_t *) dest+4 )[0]   ) ;
    printf("\n");

    return written_n;
}

void Instruction_save_array(const Instruction* elements, const size_t end, FILE* file) {
    char* bytes = malloc(sizeof(Instruction) * end);
    size_t n_written = 0;
    for (size_t i = 0; i < end; i++) {
        const Instruction t = elements[i];
        n_written += Instruction_write_in_buffer(t, bytes + n_written);
    }
    fwrite(bytes, sizeof(char), n_written, file);
    free(bytes);
}
