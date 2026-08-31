#include "operators.h"
#include "stdio.h"




void print_operator(operators operation) {
    printf("%s", operators_str[operation]);
}

bool is_unary_operator(operators operation) {
    return operation==UNARY_MINUS_OPERATOR || operation==NOT_OPERATOR;
}
