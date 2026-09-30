//
// Created by sofiane on 25/09/2026.
//

#ifndef MATEMATIX_MVM_H
#define MATEMATIX_MVM_H

#include <stdio.h>







typedef struct object {
    int type;
    size_t ref_counts;
    union {
        void *value;
        int int_value;
        double double_value;
    };

} object;

typedef struct stack {
    object *objects;
    size_t size;

} stack;



#endif //MATEMATIX_MVM_H
