#include "array_list.h"
#include <stdio.h>
#include <stdbool.h>
#include "operators.h"

#ifndef TOKEN_H
#define TOKEN_H

#define TOKEN_DISPLAY_SIZE() 100



static char * TOKEN_TYPE_NAMES[] = {
    "NONE_TOKEN",
    "VAR_TYPE_TOKEN",
    "OPERATOR_TOKEN",
    "INT_TOKEN",
    "FLOAT_TOKEN",
    "DOUBLE_TOKEN",
    "CHAR_TOKEN",
    "AFFECTATION_TOKEN",
    "DELIMITER_TOKEN",
    "IDENTIFIER_TOKEN",
    "IF_TOKEN",
    "ELIF_TOKEN",
    "ELSE_TOKEN",
    "WHILE_TOKEN",
    "OPENING_PARENTHESE_TOKEN",
    "CLOSING_PARENTHESE_TOKEN",
    "OPENING_SCOPE_TOKEN",
    "CLOSING_SCOPE_TOKEN",
    "EOF_TOKEN"
};

typedef enum {
    NONE_TOKEN,
    VAR_TYPE_TOKEN,
    OPERATOR_TOKEN,
    INT_TOKEN,
    FLOAT_TOKEN,
    DOUBLE_TOKEN,
    CHAR_TOKEN,
    AFFECTATION_TOKEN,
    DELIMITER_TOKEN,
    IDENTIFIER_TOKEN,
    IF_TOKEN,
    ELIF_TOKEN,
    ELSE_TOKEN,
    WHILE_TOKEN,
    OPENING_PARENTHESE_TOKEN,
    CLOSING_PARENTHESE_TOKEN,
    OPENING_SCOPE_TOKEN,
    CLOSING_SCOPE_TOKEN,
    EOF_TOKEN
} token_type;


typedef struct {
    token_type type;
    operators operation;
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

token binary_operator_to_token(char c);



bool involve_unary_minus(token previous_token);

bool is_num_token(token t);

bool is_comparison_operator_token(token t);





#endif
