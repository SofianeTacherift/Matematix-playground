//
// Created by userdepinfo on 14/11/26.
//

#ifndef EXECUTION_CONTEXT_H
#define EXECUTION_CONTEXT_H

#include "spds/array_list.h"
#include "spds/hash_map.h"
#include "compiler.h"
#include "matx_object.h"

typedef struct ExecutionContext {
    BinMatxObjOperationHashMap binary_operations;
    InstructionArrayList function;
} ExecutionContext;

ExecutionContext  *ExecutionContext_new_from_file(FILE* file);

void ExecutionContext_deinit(ExecutionContext *);

#endif //EXECUTION_CONTEXT_H
