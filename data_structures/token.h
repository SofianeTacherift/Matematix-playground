#include "array_list.h"
#include <stdio.h>
#include <stdbool.h>

#ifndef TOKEN_H
#define TOKEN_H

#define TOKEN_DISPLAY_SIZE() 100




static char * TOKEN_TYPE_NAMES[] = {
    "VAR_TYPE",
    "INT",
    "FLOAT",
    "DOUBLE",
    "CHAR",
    "AFFECTATION",
    "ADD",
    "SUB",
    "MULTIPLY",
    "DIVIDE",
    "DELIMITER",
    "IDENTIFIER",
    "IF",
    "ELIF",
    "ELSE",
    "UNARY_MINUS",
    "OPENING_PARENTHESE",
    "CLOSING_PARENTHESE",
    "OPENING_SCOPE",
    "CLOSING_SCOPE",
    "EQUALS",
    "GREATER_THAN",
    "LESS_THAN",
    "GREATER_OR_EQUAL",
    "LESS_OR_EQUAL",
    "AND",
    "OR",
    "NOT",
    "END"
};

typedef enum {
    VAR_TYPE,
    INT,
    FLOAT,
    DOUBLE,
    CHAR,
    AFFECTATION,
    ADD,
    SUB,
    MULTIPLY,
    DIVIDE,
    DELIMITER,
    IDENTIFIER,
    IF,
    ELIF,
    ELSE,
    UNARY_MINUS,
    OPENING_PARENTHESE,
    CLOSING_PARENTHESE,
    OPENING_SCOPE,
    CLOSING_SCOPE,
    EQUALS,
    GREATER_THAN,
    LESS_THAN,
    GREATER_OR_EQUAL,
    LESS_OR_EQUAL,
    AND,
    OR,
    NOT,
    END
} token_type;


typedef struct {
    token_type type;
    union {
    int int_val;
    float float_val;
    double double_val;
    char char_val;
    char *string_val;
    };
    int line;
    int character;
} token;


array_list(token);

static char TOKEN_DISPLAY[TOKEN_DISPLAY_SIZE()];


void print_token_in_list(token t);

void print_token_list(token_array_list *list);

void write_in_token_buffer(token t);

void print_token(token t);

void reverse_number_token_value(token *t);

token operator_to_token(char c);


bool is_operator_token(token t);

bool is_multiply_minus(token previous_token);

bool is_num_token(token t);

bool is_comparaison_operator_token(token t);



#endif