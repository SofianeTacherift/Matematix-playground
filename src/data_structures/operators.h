#ifndef OPERATORS_H
#define OPERATORS_H

#include <stdbool.h>

typedef enum operators {
    NONE_OPERATOR,
    ADD_OPERATOR,
    SUB_OPERATOR,
    MULTIPLY_OPERATOR,
    DIVIDE_OPERATOR,
    POWER_OPERATOR,
    UNARY_MINUS_OPERATOR,
    LOGICAL_NOT_OPERATOR,
    EQUALS_OPERATOR,
    NOT_EQUALS_OPERATOR,
    GREATER_THAN_OPERATOR,
    LESS_THAN_OPERATOR,
    GREATER_OR_EQUAL_OPERATOR,
    LESS_OR_EQUAL_OPERATOR,
    LOGICAL_OR_OPERATOR,
    LOGICAL_AND_OPERATOR
} operators;





static char* OPERATORS_STR[] = {
    "NONE_OPERATOR",
    "+",
    "-",
    "*",
    "/",
    "^",
    "-",
    "!",
    "==",
    "!=",
    ">",
    "<",
    ">=",
    "<=",
    "|",
    "&"
};

void print_operator(operators operation);
bool is_unary_operator(operators operation);
bool is_logical_binary_operator(operators operator);
bool is_logical_unary_operator(operators operator);
bool is_arithmetic_binary_operator(operators operator);
bool is_comparison_operator(operators operator);
operators inverse_comparison_operator(int operator);


#endif