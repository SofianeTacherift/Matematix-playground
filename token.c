#include "token.h"
#include <stdio.h>

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
        case INT :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type = INT - value=%d - line=%d - char=%d ]", t.int_val , t.line, t.character);
            break;
        case FLOAT :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=FLOAT - value=%f - line=%d - char=%d ]", t.float_val, t.line, t.character );
            break;
        case DOUBLE :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=DOUBLE - value=%lf - line=%d - char=%d ]", t.double_val, t.line, t.character );
            break;
        case CHAR :
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=CHAR - value=%c - line=%d - char=%d ]", t.char_val, t.line, t.character );
            break;
        case IDENTIFIER:
            snprintf(TOKEN_DISPLAY,TOKEN_DISPLAY_SIZE(), "token[ type=IDENTIFIER - value=%s - line=%d - char=%d ]", t.string_val, t.line, t.character );
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
    case INT:
        t->int_val=-t->int_val;
        break;
    case DOUBLE:
        t->double_val=-t->double_val;
        break;
    case FLOAT:
        t->float_val=-t->float_val;
        break;
    case CHAR:
        t->char_val=-t->char_val;
        break;
    default:
        break;
    }
}


token operator_to_token(char c) {
    switch (c) {
        case '+':
            return (token) {.type=ADD};
        case '-':
            return (token) {.type=SUB};
        case '*':
            return (token) {.type=MULTIPLY};
        case '/':
            return (token) {.type=DIVIDE};
        
    }

}

bool is_operator_token(token t) {
    switch (t.type)
    {
    case ADD:
    case MULTIPLY :
    case SUB:
    case DIVIDE:
    case UNARY_MINUS:
    case EQUALS:
    case GREATER_THAN:
    case GREATER_OR_EQUAL:
    case LESS_THAN:
    case LESS_OR_EQUAL:
    case AND:
    case OR:
        return true;
    default:
        return false;
    }
}

bool is_multiply_minus(token previous_token) {
    return (is_operator_token(previous_token) || previous_token.type==AFFECTATION);
}

bool is_num_token(token t) {
    switch (t.type)
    {
    case INT:
    case FLOAT:
    case DOUBLE:
        return true;
    default:
        return false;
    }
}

bool is_comparaison_operator_token(token t) {
    switch (t.type)
    {
    case EQUALS:
    case GREATER_THAN:
    case GREATER_OR_EQUAL:
    case LESS_THAN:
    case LESS_OR_EQUAL:
        return true;

    default:
        return false;
    }
}