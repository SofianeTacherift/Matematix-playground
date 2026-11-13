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

void Operator_print(Operator operation) {
    printf("%s", OPERATORS_STR[operation]);
}

bool Operator_is_unary(Operator operation) {
    return operation == UNARY_MINUS_OPERATOR || operation == LOGICAL_NOT_OPERATOR;
}

bool Operator_is_logical_binary(Operator operator) {
    return operator == LOGICAL_AND_OPERATOR || operator == LOGICAL_OR_OPERATOR;
}

bool Operator_is_logical_unary(Operator operator) {
    return operator == LOGICAL_NOT_OPERATOR;
}

bool Operator_is_arithmetic_binary(Operator operator) {
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

bool Operator_is_comparison(Operator operator) {
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

Operator Operator_inverse_comparison(Operator operator) {
    const int index = operator - EQUALS_OPERATOR;
    if (index < 0 || index >= sizeof(inverse_comparison_operator_buffer)) {
        return NONE_OPERATOR;
    }
    return inverse_comparison_operator_buffer[index];
}
