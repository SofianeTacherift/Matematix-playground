#include "token.h"
#include <stdio.h>
#include "operators.h"
#include <stdlib.h>
#include <stdarg.h>

#define ZERO_INITIALISATION {0}


void Token_print(Token t) {
    switch (t.type) {
    case INT_TOKEN:
        printf("token[ type = INT_TOKEN - value=%d - line=%d - char=%d ]", t.int_val, t.line, t.character);
        break;
    case FLOAT_TOKEN:
        printf("token[ type=FLOAT_TOKEN - value=%f - line=%d - char=%d ]", t.float_val, t.line, t.character);
        break;
    case DOUBLE_TOKEN:
        printf("token[ type=DOUBLE_TOKEN - value=%lf - line=%d - char=%d ]", t.double_val, t.line, t.character);
        break;
    case CHAR_TOKEN:
        printf("token[ type=CHAR_TOKEN - value=%c - line=%d - char=%d ]", t.char_val, t.line, t.character);
        break;
    case IDENTIFIER_TOKEN:
        printf("token[ type=IDENTIFIER_TOKEN - value=%s - line=%d - char=%d ]", t.string_val, t.line, t.character);
        break;
    case OPERATOR_TOKEN:
        printf("token[ type=OPERATOR_TOKEN - operation='%s' - line=%d - char=%d ]", OPERATORS_STR[t.operation], t.line,
               t.character);
        break;
    default:
        printf("token[ type=%s - line=%d - char=%d type_int : %d]", TOKEN_TYPE_NAMES[t.type], t.line, t.character,
               t.type);
        break;
    }
}


void token_reverse_num_val(Token* t) {
    switch (t->type) {
    case INT_TOKEN:
        t->int_val = -t->int_val;
        break;
    case DOUBLE_TOKEN:
        t->double_val = -t->double_val;
        break;
    case FLOAT_TOKEN:
        t->float_val = -t->float_val;
        break;
    case CHAR_TOKEN:
        t->char_val = -t->char_val;
        break;
    default:
        break;
    }
}


Token Token_make_from_binary_operator(char c) {
    Token result = ZERO_INITIALISATION;
    result.type = OPERATOR_TOKEN;
    switch (c) {
    case '+':
        result.operation = ADD_OPERATOR;
        break;
    case '-':
        result.operation = SUB_OPERATOR;
        break;
    case '*':
        result.operation = MULTIPLY_OPERATOR;
        break;
    case '/':
        result.operation = DIVIDE_OPERATOR;
        break;
    case '^':
        result.operation = POWER_OPERATOR;
        break;
    case '&':
        result.operation = LOGICAL_AND_OPERATOR;
        break;
    case '|':
        result.operation = LOGICAL_OR_OPERATOR;
        break;
    }
    return result;
}


bool Token_involve_unary_minus(Token previous_token) {
    return (!Token_is_num(previous_token) && previous_token.type != IDENTIFIER_TOKEN && previous_token.type !=
        CLOSING_PARENTHESE_TOKEN);
}

bool Token_is_num(Token t) {
    switch (t.type) {
    case INT_TOKEN:
    case FLOAT_TOKEN:
    case DOUBLE_TOKEN:
        return true;
    default:
        return false;
    }
}

bool Token_is_unary(Token t) {
    return t.type = OPERATOR_TOKEN && Operator_is_unary(t.operation);
}


void Token_deinit(Token t) {
    if (t.type == IDENTIFIER_TOKEN) {
        free(t.string_val);
    }
}

bool is_an_token_of_type(Token t, ...) {
    va_list args;
    va_start(args, t);
    int current = va_arg(args, int);
    while (current != NONE_TOKEN) {
        if (current == t.type) {
            return true;
        }
        current = va_arg(args, int);
    }
    va_end(args);
    return false;
}
