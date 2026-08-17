#ifndef PARSER_H
#define PARSER_H
#include <stdlib.h>
#include "token.h"
#include "parsing_node.h"







#define PARSING_ERROR 1
#define NO_PARSING_ERROR 0


typedef struct parser {
    token_array_list *tokens;
    int current;
    char parsing_error_buffer[2048];
    int parsing_status;

} parser;

token get_current_token(parser *parse);
token get_next_token(parser *parse);
token advance(parser *parse);
_Bool parsing_error(parser *parse);
parser *new_parser(token_array_list *tokens);
_Bool is_unary_operator_token(token t);
parsing_node *parse_code(parser *parse);
parsing_node_linked_list *parse_scope(parser *parse);
parsing_node *parse_statement(parser *parse);
parsing_node *parse_if_statement(parser *parse);
parsing_node *parse_affectation(parser *parse);
parsing_node *parse_expression(parser *parse);
parsing_node *parse_identifier(parser *parse);
parsing_node *parse_logical_or(parser *parse);
parsing_node *parse_logical_and(parser *parse);
parsing_node *parse_comparaison(parser *parse);
parsing_node *parse_additive(parser *parse);
parsing_node *parse_multiplicative(parser *parse);
parsing_node *parse_unary(parser *parse);
parsing_node *parse_primary(parser *parse);
void write_in_error_buffer(parser *parse, int line, int character, char *message);

#endif