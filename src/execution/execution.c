//
// Created by sofiane on 14/10/2026.
//

#include "execution.h"

execution_infos *new_executions_infos() {
    execution_infos *res = calloc(1, sizeof(execution_infos));
    if (res!=NULL) {
        res->instructions=new_instruction_array_list();
    }
    return res;
}

execution_infos *read_byte_code(FILE *file) {
    execution_infos *infos  =new_executions_infos();
    instruction_type current_type;
    size_t n_read;
    while  ((n_read= (fread(&current_type, sizeof(instruction_type), 1 , file) )) != 0 )  {
        instruction current_instruction;
        current_instruction.type=current_type;
        size_t operands[] = {0,0};
        fread(operands, sizeof(size_t), operands_per_type[current_type], file);
        current_instruction.operand1=operands[0];
        current_instruction.operand2=operands[1];
        add_instruction(infos->instructions, current_instruction);
    }
    return infos;
}


