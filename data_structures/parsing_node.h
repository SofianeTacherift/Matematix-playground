#ifndef PARSING_NODE_H
#define PARSING_NODE_H
#include "token.h"
#include "parsing_node.h"
#include <stdlib.h>







#define PARSING_ERROR 1
#define NO_PARSING_ERROR 0





static char* PARSING_NODE_TYPE_STR[]= {
    "NONE_NODE",
    "BINARY",
    "AFFECTATION_TOKEN",
    "INT_TOKEN",
    "FLOAT_TOKEN",
    "DOUBLE_TOKEN",
    "VARIABLE",
    "UNARY",
    "OPENING_SCOPE_TOKEN",
    "CLOSING_SCOPE_TOKEN",
    "IF_NODE",
    "ELIF_NODE",
    "ELSE_NODE"
};

typedef enum parsing_node_type {
    NONE_NODE,
    BINARY_NODE,
    AFFECTATION_NODE,
    INT_NODE,
    FLOAT_NODE,
    DOUBLE_NODE,
    VARIABLE_NODE,
    UNARY_NODE,
    OPENING_SCOPE_NODE,
    CLOSING_SCOPE_NODE,
    IF_NODE,
    ELIF_NODE,
    ELSE_NODE


} parsing_node_type;










typedef struct parsing_node {
    parsing_node_type type;
    operators operation;
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
            struct parsing_node *condition;
            struct parsing_node_linked_list *true_condition;
            struct parsing_node_linked_list *jump;
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
parsing_node *operator_token_to_parsing_node(token t);
int conditional_token_to_parsing_node_type(token t);
void print_num_val(parsing_node *n);
_Bool is_num_node(parsing_node *n);
void display_node(parsing_node *n);
void display_tree_node(parsing_node *n);
void display_node_readable(parsing_node *n);
void display_tree_node_readable(parsing_node *n, int indentation);
void free_tree_node(parsing_node *n);
parsing_node_linked_list *new_parsing_node_linked_list(void);
bool is_conditional_node(parsing_node *node);
void add_parsing_node_to_linked_list(parsing_node_linked_list *list, parsing_node *node);
void add_conditional_node_to_linked_list(parsing_node_linked_list *list, parsing_node *node);
inline bool parsing_node_linked_list_empty(parsing_node_linked_list * list);
void merge_linked_lists(parsing_node_linked_list * list, parsing_node_linked_list *to_add);


#endif
