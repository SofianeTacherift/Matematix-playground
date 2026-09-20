#include "operators.h"
#include "stdio.h"



static int inverse_comparison_operator_buffer[] = {
    NOT_EQUALS_OPERATOR,
    EQUALS_OPERATOR,
    LESS_OR_EQUAL_OPERATOR,
    GREATER_OR_EQUAL_OPERATOR,
    LESS_THAN_OPERATOR,
    GREATER_THAN_OPERATOR
};


void print_operator(operators operation) {
    printf("%s", OPERATORS_STR[operation]);
}

bool is_unary_operator(operators operation) {
    return operation==UNARY_MINUS_OPERATOR || operation==LOGICAL_NOT_OPERATOR;
}

operators inverse_comparison_operator(int operator) {
    int index= operator-EQUALS_OPERATOR;
    if (index<0 || index>=sizeof( inverse_comparison_operator_buffer )) {
        return NONE_OPERATOR;
    }
    return inverse_comparison_operator_buffer[index];
}