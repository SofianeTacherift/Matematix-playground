#ifndef PARSER_H
#define PARSER_H
#include <stdlib.h>

#include "lexer.h"
#include "token.h"
#include "parsing_node.h"







#define PARSING_ERROR 1
#define NO_PARSING_ERROR 0




typedef struct parsing_error {
    token token;
    char message[1024];
} parsing_error;

ARRAY_LIST(parsing_error, parsing_error)

typedef struct parser {

    token_array_list *tokens;
    int current_index;
    parsing_error_array_list *parsing_errors;
    int parsing_status;

} parser;

typedef struct parsing_result {
    lexing_result lexing_res;
    parsing_node *head;
    int parsing_status;
    parsing_error_array_list *errors;
} parsing_result;


bool is_an_token_of_type(token t, ...);
token get_current_token(parser *parse);
token get_next_token(parser *parse);
token advance(parser *parse);
_Bool has_parsing_error(parser *parse);

void free_parsing_result_members(parsing_result);

parser *new_parser(token_array_list *tokens);
parsing_result parse_from_file(FILE *file);

void free_nodes(int count, ...);

#endif