//
// Created by sofiane on 14/10/2026.
//

#ifndef MATEMATIX_EXECUTION_H
#define MATEMATIX_EXECUTION_H

#include "spds/array_list.h"
#include "spds/hash_map.h"
#include "compiler.h"
#include "matx_object.h"

typedef struct InstructionStack {
    InstructionArrayList* instructions;
    size_t current_instruction_index;
} InstructionStack;

typedef struct {
    MatxObject* objects;
    size_t current_index;
} ExecutionStack;

typedef MatxObject* variable_stack;

typedef struct ExecutionContext {
    BinMatxObjOperationHashMap binary_operations;
    InstructionArrayList function;
} ExecutionContext;

typedef struct ExecutionInstance {
    ExecutionContext context;
    InstructionStack instructions_stack;
};

ExecutionContext read_byte_code(FILE* file);
void ExecutionContext_deinit(ExecutionContext* context);

#endif //MATEMATIX_EXECUTION_H
