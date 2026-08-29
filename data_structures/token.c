#include "token.h"
#include <stdio.h>
#include "operators.h"


#define ZERO_INITIALISATION {0}


void print_token_in_list(token t) {
    write_in_token_buffer(t);
    printf("%s ", TOKEN_DISPLAY);
}

void print_token_list(token_array_list *list) {
    if (list==NULL) {
        printf("NULL\n");
        return;
    }
    printf("{ ");
    apply_token_operation(list, print_token_in_list);
    printf("}\n");
}

void write_in_token_buffer(token t) {
    switch(t.type) {
        case INT_TOKEN :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type = INT_TOKEN - value=%d - line=%d - char=%d ]", t.int_val , t.line, t.character);
            break;
        case FLOAT_TOKEN :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=FLOAT_TOKEN - value=%f - line=%d - char=%d ]", t.float_val, t.line, t.character );
            break;
        case DOUBLE_TOKEN :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=DOUBLE_TOKEN - value=%lf - line=%d - char=%d ]", t.double_val, t.line, t.character );
            break;
        case CHAR_TOKEN :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=CHAR_TOKEN - value=%c - line=%d - char=%d ]", t.char_val, t.line, t.character );
            break;
        case IDENTIFIER_TOKEN:
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=IDENTIFIER_TOKEN - value=%s - line=%d - char=%d ]", t.string_val, t.line, t.character );
            break;
        case OPERATOR_TOKEN:
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=OPERATOR_TOKEN - operation='%s' - line=%d - char=%d ]", operators_str[t.operation] , t.line, t.character );
            break;
        default:
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=%s - line=%d - char=%d ]", TOKEN_TYPE_NAMES[t.type], t.line, t.character);
            break;
    }

}

void print_token(token t) {
    write_in_token_buffer(t);
    printf("%s", TOKEN_DISPLAY);
}

void reverse_number_token_value(token *t) {
    switch (t->type)
    {
    case INT_TOKEN:
        t->int_val=-t->int_val;
        break;
    case DOUBLE_TOKEN:
        t->double_val=-t->double_val;
        break;
    case FLOAT_TOKEN:
        t->float_val=-t->float_val;
        break;
    case CHAR_TOKEN:
        t->char_val=-t->char_val;
        break;
    default:
        break;
    }
}


token operator_to_token(char c) {
    token result=ZERO_INITIALISATION;
    result.type=OPERATOR_TOKEN;
    switch (c) {
        case '+':
            result.operation=ADD_OPERATOR;
            break;
       case '-':
            result.operation=SUB_OPERATOR;
            break;
       case '*':
            result.operation=MULTIPLY_OPERATOR;
            break;
       case '/':
            result.operation=DIVIDE_OPERATOR;
            break;
        
    }
    return result;

}


bool is_unary_minus(token previous_token) {
    return (!is_num_token(previous_token) && previous_token.type!=IDENTIFIER_TOKEN);
}

bool is_num_token(token t) {
    switch (t.type)
    {
    case INT_TOKEN:
    case FLOAT_TOKEN:
    case DOUBLE_TOKEN:
        return true;
    default:
        return false;
    }
}

bool is_comparaison_operator_token(token t) {
    switch (t.operation)
    {
    case EQUALS_OPERATOR:
    case GREATER_THAN_OPERATOR:
    case GREATER_OR_EQUAL_OPERATOR:
    case LESS_THAN_OPERATOR:
    case LESS_OR_EQUAL_OPERATOR:
        return true;

    default:
        return false;
    }
}