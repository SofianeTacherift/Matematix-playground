#ifndef PARSING_NODE_H
#define PARSING_NODE_H
#include "token.h"
#include "parsing_node.h"

#define PARSING_ERROR 1
#define NO_PARSING_ERROR 0

static const char* PARSING_NODE_TYPE_STR[] = {
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
    "ELSE_NODE",
    "WHILE_NODE"
};

typedef enum ParsingNodeType {
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
    ELSE_NODE,
    WHILE_NODE
} ParsingNodeType;

typedef struct ParsingNode {
    ParsingNodeType type;
    Operator operation;
    struct ParsingNode* previous;
    struct ParsingNode* next;

    union {
        int int_val;
        float float_val;
        double double_val;
        char* name;
    };

    union {
        struct {
            // basic jump_node
            struct ParsingNode* left;
            struct ParsingNode* right;
        };

        struct {
            // conditional_node
            struct ParsingNode* condition;
            struct ParsingNode* true_condition;
            struct ParsingNode* jump;
        };
    };
} ParsingNode;


long ParsingNode_hash_address(const ParsingNode* node);
bool ParsingNode_equals_address(const ParsingNode* n1, const ParsingNode* n2);
ParsingNode* ParsingNode_new(void);
ParsingNode* ParsingNode_new_from_type(ParsingNodeType type);
ParsingNode* ParsingNode_new_from_num_token(Token t);
ParsingNode* ParsingNode_new_from_unary_token(Token t);
ParsingNode* ParsingNode_new_from_operator_token(Token t);
ParsingNodeType ParsingNodeType_from_conditional_token(Token t);
_Bool ParsingNode_is_numerical(ParsingNode* n);
void ParsingNode_print(ParsingNode* n);
void ParsingNode_print_recursive(ParsingNode* n);
void ParsingNode_print_readable(ParsingNode* n);
void ParsingNode_print_readable_recursive(ParsingNode* n, int indentation);
void ParsingNode_free_recursive(ParsingNode* n, bool free_next);
bool ParsingNode_is_binary_comparison(ParsingNode* node);
bool ParsingNode_is_conditional(ParsingNode* node);
ParsingNode* skip_if_statement(ParsingNode* n);
bool ParsingNode_is_logical_binary(ParsingNode* node);
void free_nodes(int count, ...);
#endif
