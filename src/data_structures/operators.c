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

bool is_logical_binary_operator(operators operator) {
    return operator==LOGICAL_AND_OPERATOR || operator==LOGICAL_OR_OPERATOR;
}

bool is_logical_unary_operator(operators operator) {
    return operator==LOGICAL_NOT_OPERATOR;
}

bool is_arithmetic_binary_operator(operators operator) {
    switch (operator) {
        case ADD_OPERATOR:
        case SUB_OPERATOR:
        case MULTIPLY_OPERATOR:
        case DIVIDE_OPERATOR:
        case POWER_OPERATOR:
            return true;
        default:
            return false;
    }
}

bool is_comparison_operator(operators operator) {
    switch (operator) {
        case NOT_EQUALS_OPERATOR:
        case EQUALS_OPERATOR:
        case LESS_OR_EQUAL_OPERATOR:
        case GREATER_OR_EQUAL_OPERATOR:
        case LESS_THAN_OPERATOR:
        case GREATER_THAN_OPERATOR:
            return true;
        default:
            return false;
    }
}

operators inverse_comparison_operator(int operator) {
    const int index= operator-EQUALS_OPERATOR;
    if (index<0 || index>=sizeof( inverse_comparison_operator_buffer )) {
        return NONE_OPERATOR;
    }
    return inverse_comparison_operator_buffer[index];
}