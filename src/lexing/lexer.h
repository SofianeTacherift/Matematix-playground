#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdio.h>
#include "spds/array_list.h"

#define DELIMITATION ';'
#define LEXING_SUCCESS 0
#define LEXING_ERROR 1

ARRAY_LIST(Token, TokenArrayList, TokenArrayList)

typedef struct LexingResult {
    char error_message[1024];
    int lexing_status;
    TokenArrayList* tokens;
} LexingResult;

LexingResult lex_file(FILE* file);

void LexingResult_deinit(LexingResult lexing_res);

#endif
