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



parser * new_parser(token_array_list * token);




parsing_node * parse_primary(parser * parse); 

parsing_node * parse_multiplicative(parser * parse);


parsing_node *parse_expression(parser *parse);

void display_node(parsing_node * n);

parsing_node * parse_comparaison(parser * parse);

parsing_node * parse_additive(parser *parse);

parsing_node * parse_statement(parser *parse) ;

parsing_node * parse_identifier(parser * parse);

parsing_node * parse_affectation(parser *parse) ;
 
void write_in_error_buffer(parser *parse, int line, int character, char * message);

void free_tree_node(parsing_node *n);

void display_tree_node(parsing_node * parsing_node);

parsing_node * begin_parsement(parser *parse);

parsing_node * parse_logical_and(parser *parse);

parsing_node * parse_logical_or(parser *parse);
#endif