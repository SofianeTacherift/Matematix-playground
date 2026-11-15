//
// Created by sofiane on 14/10/2026.
//

#ifndef MATEMATIX_EXECUTION_H
#define MATEMATIX_EXECUTION_H

#define VARIABLE_STACK_LENGTH 1000
#define EXECUTION_STACK_LENGTH 10000

#include "compiler.h"
#include "matx_object.h"
#include "execution_context.h"

ARRAY_LIST(MatxObject *, MatxObjectArrayList, MatxObjectArrayList)

typedef struct InstructionStack {
    InstructionArrayList* instructions;
    size_t current_instruction_index;
} InstructionStack;

typedef MatxObjectArrayList ExecutionStack;



typedef struct ExecutionInstance {
    ExecutionContext *context;
    InstructionStack *instructions_stack;
    ExecutionStack *execution_stack;
    MatxObject **variable_stack;
    bool finished;
    int error_code;
} ExecutionInstance;

void execute_from_file(FILE* file);


#endif //MATEMATIX_EXECUTION_H
