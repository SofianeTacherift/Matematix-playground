//
// Created by sofiane on 14/10/2026.
//

#include "execution.h"


void ExecutionContext_init(ExecutionContext* context) {
    BinMatxObjOperationHashMap_init(&context->binary_operations, MatxObjTypeCombinaison_hash, MatxObjTypeCombinaison_equals);
    InstructionArrayList_init(&context->function);
}

void ExecutionContext_deinit(ExecutionContext* context) {
    BinMatxObjOperationHashMap_deinit(&context->binary_operations);
    InstructionArrayList_deinit(&context->function);
}

ExecutionContext *ExecutionContext_new_from_file(FILE* file) {
    ExecutionContext *context = malloc(sizeof(ExecutionContext));
    ExecutionContext_init(context);
    InstructionType current_type;
    while ((fread(&current_type, sizeof(InstructionType), 1, file)) != 0) {
        Instruction current_instruction;
        current_instruction.type = current_type;
        size_t operands[] = {0, 0};
        fread(operands, sizeof(size_t), operands_per_type[current_type], file);
        current_instruction.operand1 = operands[0];
        current_instruction.operand2 = operands[1];
        InstructionArrayList_add(&context->function, current_instruction);
    }
    return context;
}


