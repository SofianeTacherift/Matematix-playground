#ifndef OPERATORS_H
#define OPERATORS_H

#include <stdbool.h>

typedef enum Operator {
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
} Operator;

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

void Operator_print(Operator operation);
bool Operator_is_unary(Operator operation);
bool Operator_is_logical_binary(Operator operator);
bool Operator_is_logical_unary(Operator operator);
bool Operator_is_arithmetic_binary(Operator operator);
bool Operator_is_comparison(Operator operator);
Operator Operator_inverse_comparison(Operator operator);

#endif
