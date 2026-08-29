#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdio.h>


#define DELIMITATION ';'
#define NOT_USED_RES_I -2
#define LEXING_ERROR -1

typedef struct lexer {
    token_array_list *tokens_list;
    int current_line;
    int current_char;
    char error_buffer[1024];
    int lexing_status;
} lexer;


static char ERROR_BUFFER[1024];



int lex_number(lexer *lexe, char *code, int start, int str_end);

int lex_string( lexer *lexe, char *code, int start, int str_end);

bool start_new_token(char c);

bool is_arithmetic_operator(char c);

int lex_minus(lexer *lexe, char *code, int i, int str_end);

token operator_to_token(char c);


token_array_list * lex_code(char * code);

void write_in_lexing_error_buffer(lexer *lexe, char *message);

#endif