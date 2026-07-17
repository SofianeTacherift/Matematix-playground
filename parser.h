#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include <stdlib.h>







#define PARSING_ERROR 1
#define NO_PARSING_ERROR 0


typedef struct parser {
    token_array_list *tokens;
    int current;
    char parsing_error_buffer[2048];
    int parsing_status;

} parser;


static char* NODE_TYPE_STR[]= {
    "NONE",
    "BINARY",
    "AFFECTATION",
    "INT",
    "FLOAT",
    "DOUBLE",
    "VARIABLE",
    "UNARY"
};

typedef enum node_type {
    NONE,
    BINARY_NODE,
    AFFECTATION_NODE,
    INT_NODE,
    FLOAT_NODE,
    DOUBLE_NODE,
    VARIABLE_NODE,
    UNARY_NODE


} node_type;

typedef enum operator {
    ADD_OPERATOR,
    SUB_OPERATOR,
    MULTIPLY_OPERATOR,
    DIVIDE_OPERATOR,
    UNARY_MINUS_OPERATOR
} operator;






typedef struct node {
    node_type type;
    operator operation;
    union {
        int int_val;
        float float_val;
        double double_val;
        char * string_val;
    };
    struct node *left;
    struct node *right;
    struct node *next;

} node;


parser * new_parser(token_array_list * token);




node * parse_primary(parser * parse); 

node * parse_multiplicative(parser * parse);


node *parse_expression(parser *parse);

void display_node(node * n);

node * parse_additive(parser *parse);

node * parse_statement(parser *parse) ;

node * parse_identifier(parser * parse);

node * parse_affectation(parser *parse) ;
 
void write_in_error_buffer(parser *parse, int line, int character, char * message);

void free_tree_node(node *n);

void display_tree_node(node * node);

node * begin_parsement(parser *parse);
#endif