//
// Created by userdepinfo on 14/11/26.
//

#include "execution.h"
#include "matpx_object_operations.h"
#include "instruction_execution.h"
#include "spds/hash_set.h"


HASH_SET(void *, AdressHashSet, AdressEntry, AdressHashSet)

long address_hash(const void * address){
    return (long) address;
}

bool address_equals(const void * adress1, const void * adress2){
    return (long) adress1 == (long) adress2;
}

InstructionStack* InstructionStack_new(InstructionArrayList* instructions){
    InstructionStack* res = malloc(sizeof(InstructionStack));
    if (res == NULL) return NULL;
    res->instructions = instructions;
    res->current_instruction_index = 0;
    return res;
}

void InstructionStack_free(InstructionStack *stack){
    free(stack);
}

ExecutionStack* ExecutionStack_new(){
    ExecutionStack* res = MatxObjectArrayList_new();
    res->capacity=EXECUTION_STACK_LENGTH;
    res->elements=realloc(res->elements, res->capacity);
    return res;
}

// VariableStack *VariableStack_new(){
//     VariableStack *res = MatxObjectArrayList_new();
//     if (res==NULL) return res;
//     res->capacity=EXECUTION_STACK_LENGTH;
//     res->elements=realloc(res->elements, res->capacity);
//     return res;
// }

ExecutionInstance* ExecutionInstance_new(ExecutionContext* context){
    ExecutionInstance* res = calloc(1, sizeof(ExecutionInstance));
    if (res == NULL) return res;
    res->context = context;
    res->instructions_stack = InstructionStack_new(&context->function);
    res->execution_stack = ExecutionStack_new();
    res->variable_stack = calloc(VARIABLE_STACK_LENGTH, sizeof(MatxObject *) );
    return res;
}

void VariableStack_free(MatxObject ** vars){

    AdressHashSet address_set;
    AdressHashSet_init(&address_set, address_hash, address_equals);
    for (size_t i= 0; i<VARIABLE_STACK_LENGTH; i++){
        if (  vars[i]!=NULL && !AdressHashSet_contains(&address_set, (void*) vars[i])){
            free(vars[i]);
            AdressHashSet_add(&address_set, (void *) vars[i]);
        }
    }
    free(vars);
    AdressHashSet_deinit(&address_set);
}

void ExecutionInstance_free(ExecutionInstance *instance){
    InstructionStack_free(instance->instructions_stack);
    VariableStack_free(instance->variable_stack);
    MatxObjectArrayList_free(instance->execution_stack);
    free(instance);
}


void execute_from_file(FILE* file){
    ExecutionContext *context = ExecutionContext_new_from_file(file);
    BinMatxObjOperationHashMap_add_base_operations(&context->binary_operations);
    ExecutionInstance* instance = ExecutionInstance_new(context);
    ExecutionInstance_execute(instance);
    ExecutionContext_deinit(context);
    free(context);
    ExecutionInstance_free(instance);

}
