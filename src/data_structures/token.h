

#ifndef TOKEN_H
#define TOKEN_H

#define TOKEN_DISPLAY_SIZE() 100


#include <stdbool.h>
#include "operators.h"




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
} TokenType;


typedef struct {
    TokenType type;
    Operator operation;
    union {
    int int_val;
    float float_val;
    double double_val;
    char char_val;
    char *string_val;
    };
    int line;
    int character;
} Token;

bool Token_is_unary(Token t);

void Token_print(Token t);

Token Token_make_from_binary_operator(char c);

bool Token_involve_unary_minus(Token previous_token);

bool Token_is_num(Token t);

void Token_deinit(Token t);

bool is_an_token_of_type(Token t, ...);


#endif
