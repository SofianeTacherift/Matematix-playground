//
// Created by sofiane on 14/10/2026.
//

#ifndef MATEMATIX_EXECUTION_H
#define MATEMATIX_EXECUTION_H


#include "array_list.h"
#include "compiler.h"


typedef struct matematix_object {
    size_t type;
    union {
        int int_value;
        double double_value;
        void *attributes;
    };
} matematix_object;

typedef struct execution_infos {
    instruction_array_list * instructions;
} execution_infos;




execution_infos *read_byte_code(FILE *file);



#endif //MATEMATIX_EXECUTION_H
