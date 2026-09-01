#ifndef PARSER_H
#define PARSER_H
#include <stdlib.h>
#include "token.h"
#include "parsing_node.h"







#define PARSING_ERROR 1
#define NO_PARSING_ERROR 0




typedef struct parsing_error {
    token token;
    char message[1024];
} parsing_error;

array_list(parsing_error)

typedef struct parser {

    token_array_list *tokens;
    int current;
    parsing_error_array_list *parsing_errors;
    int parsing_status;

} parser;


bool is_an_token_of_type(token t, ...);
token get_current_token(parser *parse);
token get_next_token(parser *parse);
token advance(parser *parse);
_Bool has_parsing_error(parser *parse);
parser *new_parser(token_array_list *tokens);
_Bool is_unary_operator_token(token t);
parsing_node *parse_main_scope(parser *parse);
parsing_node_linked_list *parse_scope(parser *parse);
parsing_node_linked_list *parse_statement(parser *parse);
parsing_node_linked_list *parse_if_statement(parser *parse);
parsing_node * parse_conditional_node(parser *parser);
parsing_node *parse_affectation(parser *parse);
parsing_node *parse_expression(parser *parse);
parsing_node *parse_identifier(parser *parse);
parsing_node *parse_logical_or(parser *parse);
parsing_node *parse_logical_and(parser *parse);
parsing_node *parse_comparison(parser *parse);
parsing_node *parse_additive(parser *parse);
parsing_node *parse_multiplicative(parser *parse);
parsing_node *parse_power(parser * parse);
parsing_node *parse_unary(parser *parse);
parsing_node *parse_primary(parser *parse);
parsing_node *parse_instruction(parser *parser);
void write_in_error_buffer(parser *parse, token current, char *message);
void free_nodes(int count, ...);

#endif