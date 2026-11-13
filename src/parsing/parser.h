#ifndef PARSER_H
#define PARSER_H
#include <stdlib.h>
#include "lexer.h"
#include "token.h"
#include "parsing_node.h"

#define PARSING_ERROR 1
#define NO_PARSING_ERROR 0

typedef struct ParsingError {
    Token token;
    char message[1024];
} ParsingError;

ARRAY_LIST(ParsingError, ParsingErrorArrayList, ParsingErrorArrayList)

typedef struct ParsingResult {
    LexingResult lexing_res;
    ParsingNode* head;
    int parsing_status;
    ParsingErrorArrayList* errors;
} ParsingResult;

void ParsingResult_deinit(ParsingResult);

ParsingResult parse_from_file(FILE* file);

#endif
