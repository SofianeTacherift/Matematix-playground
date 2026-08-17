#ifndef PARSING_NODE_H
#define PARSING_NODE_H
#include "token.h"
#include "parsing_node.h"
#include <stdlib.h>







#define PARSING_ERROR 1
#define NO_PARSING_ERROR 0





static char* PARSING_NODE_TYPE_STR[]= {
    "NONE",
    "BINARY",
    "CONDITIONAL",
    "AFFECTATION",
    "INT",
    "FLOAT",
    "DOUBLE",
    "VARIABLE",
    "UNARY",
    "OPENING_SCOPE",
    "CLOSING_SCOPE"
};

typedef enum parsing_node_type {
    NONE,
    BINARY_NODE,
    CONDITIONAL_NODE,
    AFFECTATION_NODE,
    INT_NODE,
    FLOAT_NODE,
    DOUBLE_NODE,
    VARIABLE_NODE,
    UNARY_NODE,
    OPENING_SCOPE_NODE,
    CLOSING_SCOPE_NODE


} parsing_node_type;

typedef enum parsing_operator {
    ADD_OPERATOR,
    SUB_OPERATOR,
    MULTIPLY_OPERATOR,
    DIVIDE_OPERATOR,
    UNARY_MINUS_OPERATOR,
    EQUALS_OPERATOR,
    GREATER_THAN_OPERATOR,
    LESS_THAN_OPERATOR,
    GREATER_OR_EQUAL_OPERATOR,
    LESS_OR_EQUAL_OPERATOR,
    OR_OPERATOR,
    AND_OPERATOR
} parsing_operator;







typedef struct parsing_node {
    parsing_node_type type;
    parsing_operator operation;
    struct parsing_node *previous;
    struct parsing_node *next;
    union {
        int int_val;
        float float_val;
        double double_val;
        char * string_val;
    };
    union {
        struct { // basic node
            struct parsing_node *left;
            struct parsing_node *right;
        };
        struct { // conditional_node 
            struct parsing_node_linked_list *true_condition;
            struct parsing_node_linked_list *false_condition;
            struct parsing_node *condition;
        };


    };
} parsing_node;

typedef struct parsing_node_linked_list {
    parsing_node *head;
    parsing_node *end;
} parsing_node_linked_list;



parsing_node *new_parsing_node(void);
parsing_node *new_parsing_node_of(int type);
parsing_node *token_num_to_node(token t);
parsing_node *unary_token_to_node(token t);
parsing_node *comparaison_token_to_node(token t);
void print_operation(parsing_operator op);
void print_num_val(parsing_node *n);
_Bool is_num_node(parsing_node *n);
void display_node(parsing_node *n);
void display_tree_node(parsing_node *n);
void display_node_readable(parsing_node *n);
void display_tree_node_readable(parsing_node *n);
void free_tree_node(parsing_node *n);
parsing_node_linked_list *new_parsing_node_linked_list(void);
void add_parsing_node(parsing_node_linked_list *list, parsing_node *node);

#endif
