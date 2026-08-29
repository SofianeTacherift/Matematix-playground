#include "parsing_node.h"
#include  "hash_table.h"





#define PRINT_SPACE printf(" ");

parsing_node * new_parsing_node() {
    parsing_node *res=calloc(1, sizeof(parsing_node));
    return res;
}

parsing_node *new_parsing_node_of(int type) {
    parsing_node *res=calloc(1, sizeof(parsing_node));
    res->type=type;
    return res;
}

parsing_node * token_num_to_node(token t) {
    parsing_node * parsing_node=new_parsing_node();
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
        return NULL;
    return parsing_node;
    }
}







parsing_node *  operator_token_to_parsing_node(token t) {
    if (t.type!=OPERATOR_TOKEN) {
        return NULL;
    }
    parsing_node *result = new_parsing_node();
    if (!is_unary_operator(t.operation)) {
        result->type=UNARY_NODE;
    }
    else {
        result->type=BINARY_NODE;
    }
    result->operation=t.operation;
    return result;
}


void print_num_val(parsing_node *n) {
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
    }
}

bool is_num_node(parsing_node * n) {
    return n->type==INT_NODE || n->type==FLOAT_NODE || n->type==DOUBLE_NODE;
}

void display_node_readable(parsing_node *n) {
   if (n==NULL) {printf("NULL"); return;}
   if (n->type==IF_NODE) {
        printf("if");
   }
   if (n->type==ELIF_NODE) {
        printf("elif");
   }
   if (n->type==ELSE_NODE) {
        printf("else");
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
        print_operation(n->operation);
    }
    if (n->type==VARIABLE_NODE) {
        printf("%s", n->string_val);
    }
    if (is_num_node(n)) {
        print_num_val(n);
    }
}
void display_node(parsing_node * n) {
    if (n==NULL) {printf("NULL");}
    printf("parsing_node[ type=%s ", PARSING_NODE_TYPE_STR[n->type]);
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf("operation='");
        print_operation(n->operation);
        printf("' ");
    }
    if (n->type==VARIABLE_NODE) {
        printf("name=%s ", n->string_val);
    }
    if (is_num_node(n)) {
        printf("value=");
        print_num_val(n);
        printf(" ");
    }
    printf("]");
}

void display_tree_node(parsing_node * n) {
    if (n==NULL) {
        printf("%s", "NULL");
        return;
     }
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf("( ");
    }
    if (n->left!=NULL) {
    display_tree_node(n->left);
    }
    display_node(n);
    printf(" ");
    if (n->right!=NULL) {
        display_tree_node(n->right);
    }
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf(") ");
    }
    if (n->type==OPENING_SCOPE_NODE) {
        printf("}");
    }
    if (n->next!=NULL) {
        printf("--> ");
        display_tree_node(n->next);
    }

}





void display_tree_node_readable(parsing_node *n) {
   if (n==NULL) {
        printf("%s", "NULL");
        return;
    }

    if (conditional_node(n)) {
        display_node_readable(n);
        printf(" ");
        if (n->type!=ELSE_NODE) {
            display_tree_node_readable(n->condition);
        }
        display_tree_node_readable(n->true_condition);
        if (n->next!=NULL) {
            display_tree_node_readable(n->next);
        }
        return;
    }

    if (n->type==BINARY_NODE) {
        printf("( ");
    }

    if (n->left!=NULL) {
        display_tree_node_readable(n->left);
    }


    display_node_readable(n);
    PRINT_SPACE

    if (n->type==UNARY_NODE) {
        printf("( ");
    }
    if (n->right!=NULL) {
        display_tree_node_readable(n->right);
    }
    if (n->type==BINARY_NODE || n->type==UNARY_NODE) {
        printf(") ");
    }
    if (n->type==OPENING_SCOPE_NODE) {
        printf("\n} ");
    }

    if (n->next!=NULL) {
        printf("\n");
        display_tree_node_readable(n->next);
    }
}


void free_tree_node(parsing_node *n) {
    if (n==NULL) {
        return;
    }
    parsing_node *left =n->left;
    parsing_node *right = n->right;
    free_tree_node(n->next);
    free(n);
    free_tree_node(left);
    free_tree_node(right);
}


bool conditional_node(parsing_node *node) {
    return node->type==IF_NODE || node->type==ELIF_NODE || node->type==ELSE_NODE;
}

// linked list


parsing_node_linked_list  *new_parsing_node_linked_list() {
    parsing_node_linked_list * result = calloc(1, sizeof(parsing_node_linked_list));
    result->head=NULL;
    result->end=NULL;
    return result;

}

void add_parsing_node_to_linked_list(parsing_node_linked_list * list, parsing_node * node) {
    if (list->end==NULL && list->head==NULL) {
        list->head=node;
        list->end=node;
    }
    else {
        list->end->next=node;
        node->previous=list->end;
        list->end=node;
    }
}

void skip_conditional_node(parsing_node_linked_list *list, parsing_node *node) {
    if (! conditional_node(node)) {
        return;
    }
    if (list->head==NULL && list->end==NULL) {
        list->head=node;
        list->end=node->jump;
    }
    else {
        list->end->next=node;
        list->end=node->jump;
    }
}



parsing_node *operator_token_to_parsing_node(token t) {
    if (!is_operator_token(t)) {
        return NULL;
    }

}


