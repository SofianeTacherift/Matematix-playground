#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdio.h>



#define DELIMITATION ';'
#define LEXING_SUCCESS 0
#define LEXING_ERROR 1

typedef struct lexer {
    token_array_list *tokens_list;
    int total_bytes_read;

    char *code_buffer;
    int buffer_size;
    int reading_index;

    int current_line;
    int current_char;
    char error_buffer[1024];
    int lexing_status;
} lexer;

typedef struct lexing_result {
    char error_buffer[1024];
    int lexing_status;
    token_array_list *tokens;
}lexing_result;


static char ERROR_BUFFER[1024];



lexer * new_lexer(token_array_list *);

void set_lexer_code_buffer(lexer *, char*, int);

void advance_n(lexer *, int);


void advance_check_ln(lexer *lexer) ;

void lex_number(lexer *lexer);

void lex_string( lexer *lexer);

bool start_new_token(char c);

bool is_arithmetic_operator(char c);

token minus_to_token(lexer *lexer);



void lex_code(lexer *lexer );

lexing_result lex_code_from_file(int buffer_size, FILE *file);

void write_in_lexing_error_buffer(lexer *lexe, char *message);

#endif