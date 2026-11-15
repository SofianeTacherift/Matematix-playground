//
// Created by userdepinfo on 14/11/26.
//

#include "instruction_execution.h"
#include "execution.h"





void MatxObject_print(MatxObject *object){
    if (object==NULL) return;
    switch (object->type){
    case INT_TYPE:
        printf("MatxObject[ type : INT - value : %d - refs : %zu]", object->int_value, object->ref_count);
        break;
    case DOUBLE_TYPE:
        printf("MatxObject[ type : DOUBLE - value : %lf - refs : %zu]", object->double_value, object->ref_count);
        break;
    default:
        break;
    }
}

void ExecutionInstance_println(ExecutionInstance *instance){
    printf("ExecutionInstance[\n\n");
    MatxObjectArrayList_println(instance->execution_stack, MatxObject_print);
    printf("\nvariables : { ");
   for (size_t i=0 ; i<VARIABLE_STACK_LENGTH; i++){
       MatxObject_print(instance->variable_stack[i]);
       if (i<VARIABLE_STACK_LENGTH-1){
           printf(" | ");
       }
   }
    printf("}\n]\n");
}

MatxObject *MatxObject_new_of(const size_t type){
    MatxObject *res= MatxObject_new();
    if (!res) return res;
    res->type=type;
    return res;
}

void ExecutionInstanceInstructionStack_advance(ExecutionInstance *instance){
    InstructionStack * instruction_stack = instance->instructions_stack;
    instruction_stack->current_instruction_index++;
    if (instruction_stack->current_instruction_index>=instruction_stack->instructions->size){
        instance->finished=true;
    }
}



void execute_iconst(ExecutionInstance *instance, Instruction instruction){
    ExecutionStack *execution_stack = instance->execution_stack;
    MatxObject * push = MatxObject_new_of(INT_TYPE);
    push->int_value=(int) instruction.operand1;
    MatxObjectArrayList_add(execution_stack, push);
    ExecutionInstanceInstructionStack_advance(instance);
}

void execute_dconst(ExecutionInstance *instance, Instruction instruction){
    ExecutionStack *execution_stack = instance->execution_stack;
    MatxObject * push = MatxObject_new_of(DOUBLE_TYPE);
    push->double_value= *((double*)&instruction.operand1);
    MatxObjectArrayList_add(execution_stack, push);
    ExecutionInstanceInstructionStack_advance(instance);
}

void execute_oload(ExecutionInstance *instance, Instruction instruction){
    ExecutionStack *execution_stack = instance->execution_stack;
    size_t index = instruction.operand1;
    MatxObject *loaded_object = instance->variable_stack[index];
    MatxObjectArrayList_add(execution_stack, loaded_object);
    ExecutionInstanceInstructionStack_advance(instance);

}
void execute_ostore( ExecutionInstance *instance, Instruction instruction){
    ExecutionStack *execution_stack = instance->execution_stack;
    MatxObject *last_object = execution_stack->elements[execution_stack->size-1];
    last_object->ref_count++;
    MatxObjectArrayList_pop(execution_stack);
    instance->variable_stack[instruction.operand1]=last_object;
    ExecutionInstanceInstructionStack_advance(instance);
}

void execute_binary_add(ExecutionInstance *instance, Instruction instruction){
    ExecutionStack *execution_stack = instance->execution_stack;
    MatxObject *object2 = execution_stack->elements[execution_stack->size-1];
    MatxObjectArrayList_pop(execution_stack);
    MatxObject *object1 = execution_stack->elements[execution_stack->size-1];
    MatxObjectArrayList_pop(execution_stack);
    MatxObjTypeCombinaison types  = (MatxObjTypeCombinaison){.type1 = object1->type, .type2 =object2->type, .operation = ADD_OPERATOR};

    BinMatxObjOperationHashMap operations = instance->context->binary_operations;
    BinMatxObjOperation operation = BinMatxObjOperationHashMap_get_or_default(&operations, types, NULL);
    if (operation==NULL){
        instance->error_code=2;
        return;
    }
    MatxObject *res = operation(object1, object2);

    MatxObjectArrayList_add(execution_stack, res);
    ExecutionInstanceInstructionStack_advance(instance);

    if (object1->ref_count==0) free(object1);
    if (object2->ref_count==0) free(object2);


}

typedef void (*InstructionExecutionFunc) (ExecutionInstance *, Instruction instruction);

static const InstructionExecutionFunc INSTRUCTION_FUNCTION[] = {
    NULL,
    execute_iconst,
    execute_dconst,
    execute_oload,
    execute_ostore,
    execute_binary_add
};

void ExecutionInstance_execute_single_instruction(ExecutionInstance *instance, Instruction instruction){
    if (instruction.type>=0 && instruction.type<sizeof INSTRUCTION_FUNCTION/sizeof (InstructionExecutionFunc)){
        INSTRUCTION_FUNCTION[instruction.type](instance, instruction);
    }
    else{
        instance->error_code=1;
    }
}

void ExecutionInstance_execute(ExecutionInstance *instance){
    InstructionStack *instruction_stack = instance->instructions_stack;
    while (!instance->finished && !instance->error_code){
        Instruction current_instruction =  instruction_stack->instructions->elements[instruction_stack->current_instruction_index];
        ExecutionInstance_execute_single_instruction(instance, current_instruction);
    }
    ExecutionInstance_println(instance);
}
