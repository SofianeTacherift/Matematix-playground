#include "parsing_node.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


#define PRINT_SPACE printf(" ");



bool ParsingNode_is_conditional(ParsingNode *node) {
    if (node==NULL) {
        return NULL;
    }
    return node->type==IF_NODE || node->type==ELIF_NODE || node->type==ELSE_NODE || node->type==WHILE_NODE;
}

bool ParsingNode_is_binary_comparison(ParsingNode *node) {
    if (node->type!=BINARY_NODE) {return false;}
    return Operator_is_comparison(node->operation);
}

bool ParsingNode_is_logical_binary(ParsingNode *node) {
    return node->type==BINARY_NODE &&  Operator_is_logical_binary(node->operation);
}

bool is_logical_unary_node(ParsingNode *node) {
    return node->type==UNARY_NODE && Operator_is_logical_unary(node->operation);
}

bool is_logical_node(ParsingNode *node) {
    return ParsingNode_is_logical_binary(node) || is_logical_unary_node(node);
}

bool ParsingNode_is_numerical(ParsingNode * n) {
    return n->type==INT_NODE || n->type==FLOAT_NODE || n->type==DOUBLE_NODE;
}


long ParsingNode_hash_address(const ParsingNode *node) {
    return  (long) node;
}

bool ParsingNode_equals_address(const ParsingNode *n1, const ParsingNode *n2) {
    return n1==n2;
}

long hash_size_t(size_t i) {
    return (long) i;
}

bool equals_size_t(size_t i1, size_t i2) {
    return i1==i2;
}

void print_p_node_size_t_entry(ParsingNode *p, size_t i) {
    printf("{ ");
    ParsingNode_print(p);
    printf(" : %d}\n", (int) i);
}






ParsingNode * ParsingNode_new() {
    ParsingNode *res=calloc(1, sizeof(ParsingNode));
    return res;
}

ParsingNode *ParsingNode_new_from_type(ParsingNodeType type) {
    ParsingNode *res=calloc(1, sizeof(ParsingNode));
    res->type=type;
    return res;
}

ParsingNode * ParsingNode_new_from_num_token(Token t) {
    ParsingNode * parsing_node=ParsingNode_new();
    switch (t.type) {
    case INT_TOKEN:
        parsing_node->type=INT_NODE;
        parsing_node->int_val=t.int_val;
        break;
    case FLOAT_TOKEN:
        parsing_node->type=FLOAT_NODE;
        parsing_node->float_val=t.float_val;
        break;
    case DOUBLE_TOKEN:
        parsing_node->type=DOUBLE_NODE;
        parsing_node->double_val=t.double_val;
        break;
    default:
            free(parsing_node);
            return NULL;
    }
    return parsing_node;
}







ParsingNode *  ParsingNode_new_from_operator_token(Token t) {
    if (t.type!=OPERATOR_TOKEN) {
        return NULL;
    }
    ParsingNode *result = ParsingNode_new();
    if (Operator_is_unary(t.operation)) {
        result->type=UNARY_NODE;    }
    else {
        result->type=BINARY_NODE;
    }
    result->operation=t.operation;
    return result;
}

ParsingNodeType ParsingNodeType_from_conditional_token(Token t) {
    switch (t.type)
    {
        case IF_TOKEN:
            return IF_NODE;
        case ELIF_TOKEN:
            return ELIF_NODE;
        case ELSE_TOKEN :
            return ELSE_NODE;
        case WHILE_TOKEN:
            return WHILE_NODE;
    default:
        return NONE_NODE;
    }
}


void print_numerical_node_val(ParsingNode *n) {
    switch(n->type) {
        case INT_NODE:
            printf("%d", n->int_val);
            break;
        case FLOAT_NODE:
            printf("%f", n->float_val);
            break;
        case DOUBLE_NODE:
            printf("%lf", n->double_val);
            break;
            default:
            break;
    }
}


void ParsingNode_print_readable(ParsingNode *n) {
   if (n==NULL) {printf("NULL "); return;}
   if (n->type==IF_NODE) {
        printf("if");
   }
   if (n->type==ELIF_NODE) {
        printf("elif");
   }
   if (n->type==ELSE_NODE) {
        printf("else");
   }
    if (n->type==WHILE_NODE) {
        printf("while");
    }
   if (n->type==AFFECTATION_NODE) {
        printf("=");
   }
   if (n->type==OPENING_SCOPE_NODE) {
        printf("{");
    }
    if (n->type==CLOSING_SCOPE_NODE) {
        printf("}");
    }
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf("%s", OPERATORS_STR[n->operation]);
    }
    if (n->type==VARIABLE_NODE) {
        printf("%s", n->name);
    }
    if (ParsingNode_is_numerical(n)) {
        print_numerical_node_val(n);
    }
}
void ParsingNode_print(ParsingNode * n) {
    if (n==NULL) {
        printf("NULL");
        return;
    }
    printf("parsing_node[ type=%s ", PARSING_NODE_TYPE_STR[n->type]);
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf("operation='");
        printf("%s", OPERATORS_STR[n->operation]);
        printf("' ");
    }
    if (n->type==VARIABLE_NODE) {
        printf("name=%s ", n->name);
    }
    if (ParsingNode_is_numerical(n)) {
        printf("value=");
        print_numerical_node_val(n);
        printf(" ");
    }
    printf("]");
}

void ParsingNode_print_recursive(ParsingNode * n) {
    if (n==NULL) {
        printf("%s", "NULL");
        return;
     }
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf("( ");
    }
    if (n->left!=NULL) {
    ParsingNode_print_recursive(n->left);
    }
    ParsingNode_print(n);
    printf(" ");
    if (n->right!=NULL) {
        ParsingNode_print_recursive(n->right);
    }
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf(") ");
    }
    if (n->type==OPENING_SCOPE_NODE) {
        printf("}");
    }
    if (n->next!=NULL) {
        printf("--> ");
        ParsingNode_print_recursive(n->next);
    }

}



void print_indentation(int indentation) {
    for (int i=0; i<indentation; i++) {
        printf("\t");
    }
}


void ParsingNode_print_readable_recursive(ParsingNode *n, int indentation) {
   if (n==NULL) {
        printf("%s", "NULL ");
        return;
    }

    if (ParsingNode_is_conditional(n)) {
        ParsingNode_print_readable(n);
        printf(" ");
        if (n->type!=ELSE_NODE) {
            ParsingNode_print_readable_recursive(n->condition, indentation);
        }
        ParsingNode_print_readable_recursive(n->true_condition, indentation);
        if (n->next!=NULL) {
            print_indentation(indentation);
            ParsingNode_print_readable_recursive(n->next, indentation);
        }
        return;
    }

    if (n->type==BINARY_NODE) {
        printf("( ");
    }

    if (n->left!=NULL) {
        ParsingNode_print_readable_recursive(n->left, indentation);
    }


    ParsingNode_print_readable(n);
    if (n->type==OPENING_SCOPE_NODE) {
        indentation++;
        printf("\n");
        print_indentation(indentation);
    }
    else {
        PRINT_SPACE
    }

    if (n->type==UNARY_NODE) {
        printf("( ");
    }
    if (n->right!=NULL) {
        ParsingNode_print_readable_recursive(n->right, indentation);
    }
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf(") ");
    }
    if (n->type==OPENING_SCOPE_NODE) {
        indentation--;
        printf("\n");
        print_indentation(indentation);
        printf("}\n");

    }

    if (n->next!=NULL) {

        printf("\n");
        print_indentation(indentation);
        ParsingNode_print_readable_recursive(n->next, indentation);
    }
}


void ParsingNode_free_recursive(ParsingNode *n, bool free_next ) {
    if (n==NULL) {
        return;
    }
    if (free_next) {
        ParsingNode_free_recursive(n->next, free_next);
    }
    ParsingNode *left =n->left;
    ParsingNode *right = n->right;

    if (ParsingNode_is_conditional(n)) {
        ParsingNode_free_recursive(n->condition, free_next);
        ParsingNode_free_recursive(n->true_condition, free_next);

    }
    else {
        ParsingNode_free_recursive(left, free_next);
        ParsingNode_free_recursive(right, free_next);
    }
    if (n->type==VARIABLE_NODE) {
        free(n->name);
    }
    free(n);
}


ParsingNode *skip_if_statement(ParsingNode *n) {
    ParsingNode *current=n;
    if (n->type==IF_NODE) {
        current=current->next;
    }
    while (current && (current->type==ELSE_NODE || current->type==ELIF_NODE)) {
        current=current->next;
    }
    return current;
}

void display_node_node_entry(ParsingNode *n1, ParsingNode *n2) {
    printf("{");
    ParsingNode_print(n1);
    printf(" : ");
    ParsingNode_print(n2);
    printf("}");
}

void free_nodes(int count, ...) {
    va_list pointers;
    va_start(pointers, count);
    for (int i=0; i<count; i++ ) {
        ParsingNode *p = va_arg(pointers, ParsingNode*);
        ParsingNode_free_recursive(p, true);
    }
}









