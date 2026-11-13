//
// Created by sofiane on 13/09/2026.
//

#ifndef MATEMATIX_INSTRUCTIONS_H
#define MATEMATIX_INSTRUCTIONS_H
#include <stdlib.h>
#include "spds/array_list.h"
#include "parsing_node.h"

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
    IF_CMPTLTE,
    RETURN_INSTRUCTION
} InstructionType;

static const int operands_per_type[] = {
    0, // NONE INSTRUCTION
    1, // ICONST
    1, // DCONST
    1, // OLOAD
    1, // OSTORE
    0, //BINARY_ADD_INSTRUCTION,
    0, //BINARY_SUB_INSTRUCTION,
    0, //BINARY_MULT_INSTRUCTION,
    0, //BINARY_DIVIDE_INSTRUCTION,
    0, //BINARY_POWER_INSTRUCTION,
    0, //UNARY_MINUS_INSTRUCTION,
    1, //GOTO,
    1, //IF_CMPEQ,
    1, //IF_CMPNE,
    1, //IF_CMPGT,
    1, //IF_CMPLT,
    1, //IF_CMPGTE,
    1, //IF_CMPTLTE,
    0, //RETURN_INSTRUCTION
};

static const int comparison_operator_to_instr_type_buffer[] = {
    IF_CMPEQ,
    IF_CMPNE,
    IF_CMPGT,
    IF_CMPGTE,
    IF_CMPLT,
    IF_CMPTLTE,
};

static char* INSTRUCTION_TYPE_STR[] = {
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
    "IF_CMPTLTE",
    "RETURN"
};

typedef struct {
    InstructionType type;
    size_t operand1;
    size_t operand2;
} Instruction;

ARRAY_LIST(Instruction, InstructionArrayList, InstructionArrayList)

// functions

InstructionType InstructionType_from_binary_arithmetic_node(ParsingNode* node);
InstructionType InstructionType_from_unary_arithmetic_node(ParsingNode* node);
int InstructionType_from_comparison_operator(int t);
void Instruction_save_array(const Instruction* elements, const size_t end, FILE* file);
void Instruction_print_readable(Instruction instruction);

#endif //MATEMATIX_INSTRUCTIONS_H
