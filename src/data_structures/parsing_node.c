#include "parsing_node.h"
#include  "void_hash_table.h"





#define PRINT_SPACE printf(" ");



bool is_conditional_node(parsing_node *node) {
    if (node==NULL) {
        return NULL;
    }
    return node->type==IF_NODE || node->type==ELIF_NODE || node->type==ELSE_NODE || node->type==WHILE_NODE;
}

bool is_binary_comparison_node(parsing_node *node) {
    if (node->type!=BINARY_NODE) {return false;}
    return is_comparison_operator(node->operation);
}

bool is_logical_binary_node(parsing_node *node) {
    return node->type==BINARY_NODE &&  is_logical_binary_operator(node->operation);
}

bool is_logical_unary_node(parsing_node *node) {
    return node->type==UNARY_NODE && is_logical_unary_operator(node->operation);
}

bool is_logical_node(parsing_node *node) {
    return is_logical_binary_node(node) || is_logical_unary_node(node);
}

bool is_numerical_node(parsing_node * n) {
    return n->type==INT_NODE || n->type==FLOAT_NODE || n->type==DOUBLE_NODE;
}


long hash_node_addr(parsing_node *node) {
    return  (long) node;
}

bool equals_node_addr(parsing_node *n1, parsing_node *n2) {
    return n1==n2;
}

long hash_size_t(size_t i) {
    return (long) i;
}

bool equals_size_t(size_t i1, size_t i2) {
    return i1==i2;
}

void print_p_node_size_t_entry(parsing_node *p, size_t i) {
    printf("{ ");
    display_node(p);
    printf(" : %d}\n", (int) i);
}




void print_jump_infos(jump_infos jump_inf) {
    printf(" jump_infos[ jumpif : %s - jump_node : ", jump_inf.jump_if ? "TRUE" : "FALSE"   );
    display_node(jump_inf.jump_node);
    printf(" - next_condition : ");
    display_node(jump_inf.next_node);
    printf(" ]\n");
}


void print_node_jump_entry(parsing_node *node, jump_infos infos) {
    printf("{");
    display_node(node);
    printf(" : ");
    print_jump_infos(infos);
    printf("}");
}


parsing_node * new_parsing_node() {
    parsing_node *res=calloc(1, sizeof(parsing_node));
    return res;
}

parsing_node *new_parsing_node_of(int type) {
    parsing_node *res=calloc(1, sizeof(parsing_node));
    res->type=type;
    return res;
}

parsing_node * numerical_token_to_node(token t) {
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
            free(parsing_node);
            return NULL;
    }
    return parsing_node;
}







parsing_node *  operator_token_to_parsing_node(token t) {
    if (t.type!=OPERATOR_TOKEN) {
        return NULL;
    }
    parsing_node *result = new_parsing_node();
    if (is_unary_operator(t.operation)) {
        result->type=UNARY_NODE;    }
    else {
        result->type=BINARY_NODE;
    }
    result->operation=t.operation;
    return result;
}

int conditional_token_to_parsing_node_type(token t) {
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


void print_numerical_node_val(parsing_node *n) {
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


void display_node_readable(parsing_node *n) {
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
        printf("%s", n->string_val);
    }
    if (is_numerical_node(n)) {
        print_numerical_node_val(n);
    }
}
void display_node(parsing_node * n) {
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
        printf("name=%s ", n->string_val);
    }
    if (is_numerical_node(n)) {
        printf("value=");
        print_numerical_node_val(n);
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



void print_indentation(int indentation) {
    for (int i=0; i<indentation; i++) {
        printf("\t");
    }
}


void display_tree_node_readable(parsing_node *n, int indentation) {
   if (n==NULL) {
        printf("%s", "NULL ");
        return;
    }

    if (is_conditional_node(n)) {
        display_node_readable(n);
        printf(" ");
        if (n->type!=ELSE_NODE) {
            display_tree_node_readable(n->condition, indentation);
        }
        display_tree_node_readable(n->true_condition, indentation);
        if (n->next!=NULL) {
            print_indentation(indentation);
            display_tree_node_readable(n->next, indentation);
        }
        return;
    }

    if (n->type==BINARY_NODE) {
        printf("( ");
    }

    if (n->left!=NULL) {
        display_tree_node_readable(n->left, indentation);
    }


    display_node_readable(n);
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
        display_tree_node_readable(n->right, indentation);
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
        display_tree_node_readable(n->next, indentation);
    }
}


void free_tree_node(parsing_node *n, bool free_next ) {
    if (n==NULL) {
        return;
    }
    parsing_node *left =n->left;
    parsing_node *right = n->right;

    if (is_conditional_node(n)) {
        free_tree_node(n->condition, free_next);
        free_tree_node(n->true_condition, free_next);
    }
    else {
        free(n);
        free_tree_node(left, free_next);
        free_tree_node(right, free_next);
    }
}







// linked list


parsing_node_linked_list  *new_parsing_node_linked_list() {
    parsing_node_linked_list * result = calloc(1, sizeof(parsing_node_linked_list));
    result->head=NULL;
    result->end=NULL;
    return result;

}


inline bool parsing_node_linked_list_empty(parsing_node_linked_list * list) {
    return list->head==NULL && list->end==NULL;
}
void add_parsing_node_to_linked_list(parsing_node_linked_list * list, parsing_node * node) {
    if (node==NULL) {
        return;
    }
    if (parsing_node_linked_list_empty(list)) {
        list->head=node;
        list->end=node;
    }
    else {
        list->end->next=node;
        node->previous=list->end;
        list->end=node;
    }
}

void merge_linked_lists(parsing_node_linked_list * list, parsing_node_linked_list *to_add) {
    if (list==NULL || to_add==NULL || to_add->head==NULL && to_add->end==NULL) {
        return;
    }
    
    if (parsing_node_linked_list_empty(list)) {
        list->head=to_add->head;
        list->end=to_add->end;
    }
    else { 
        list->end->next=to_add->head;
        list->end=to_add->end;
    }
}

void add_conditional_node_to_linked_list(parsing_node_linked_list *list, parsing_node *node) {
    if (! is_conditional_node(node) || node->jump==NULL) {
        return;
    }
    if (list->head==NULL && list->end==NULL) {
        list->head=node;
        list->end=node->jump;
    }
    else {
        list->end->next=node;
        if (node->jump!=NULL) {
            list->end=node->jump;
        }
        else {
            while (node->next!=NULL) {
                node=node->next;
            }
            list->end=node;
        }
    }
}

void free_parsing_node_linked_list(parsing_node_linked_list* list) {
    parsing_node * current = list->head;
    while (current!=NULL) {
        parsing_node * to_free= current;
        current=current->next;
        free_tree_node(to_free, false);
    }

}

void print_parsing_node_linked_list(parsing_node_linked_list * list) {
    parsing_node * current = list->head;
    while (current!=NULL) {
        display_node(current);
        current=current->next;
        if (current!=NULL) {
            printf("  ");
        }
    }
    printf("\n");
}




void display_node_node_entry(parsing_node *n1, parsing_node *n2) {
    printf("{");
    display_node(n1);
    printf(" : ");
    display_node(n2);
    printf("}");
}

void display_tree_node_tree_node(parsing_node *n1, parsing_node *n2) {
    printf("{");
    if (n1==NULL) {printf("NULL");}
    else {display_tree_node(n1);}
    printf(" : ");
    if (n2==NULL) {printf("NULL");}
    else {display_tree_node(n2);}
    printf("}");
}


parsing_node * most_left_non_logical_operand_node(parsing_node *node, p_node_p_node_hash_map *map) {
    if (node==NULL) {
        return NULL;
    }

    if (node->type!=BINARY_NODE || (node->operation!=LOGICAL_OR_OPERATOR && node->operation!=LOGICAL_AND_OPERATOR )) {
        return node;
    }


    parsing_node *res = most_left_non_logical_operand_node(node->left, map );
    put_to_p_node_p_node_hash_map(map, node, res);
    if (node->right->type==BINARY_NODE && (node->right->operation==LOGICAL_OR_OPERATOR || node->right->operation==LOGICAL_AND_OPERATOR )) {
        most_left_non_logical_operand_node(node->right, map);
    }
    else {
        put_to_p_node_p_node_hash_map(map, node->right, node->right);
    }

    return res;
}


void map_most_not_logical_node_left(parsing_node *condition, p_node_p_node_hash_map *map) {
    most_left_non_logical_operand_node(condition, map);

}


HASH_MAP(parsing_node *, size_t, p_node, size_t)


void map_condition_jumps_recursive(parsing_node *node, parsing_node *last_or, parsing_node *last_and, parsing_node* parent, p_node_p_node_hash_map *most_left_node_map, p_node_size_t_hash_map *levels, p_node_jump_hash_map *jumps_map) {

    put_to_p_node_size_t_hash_map(levels, node, * (get_from_p_node_size_t_hash_map(levels, parent)) +1);

    if (node->type==BINARY_NODE && (node->operation==LOGICAL_OR_OPERATOR || node->operation==LOGICAL_AND_OPERATOR)) {
        parsing_node *or = (node->operation==LOGICAL_OR_OPERATOR) ? node : last_or;
        parsing_node *and = (node->operation==LOGICAL_AND_OPERATOR) ? node : last_and;
        map_condition_jumps_recursive(node->left, or, and, node, most_left_node_map,levels, jumps_map);
        map_condition_jumps_recursive(node->right, last_or, last_and, node , most_left_node_map, levels, jumps_map);
    }
    else {
        bool jump_if= parent==NULL || parent->operation==LOGICAL_OR_OPERATOR; // if parent type is logical and, we jump only if the condition is false so we must store it
        parsing_node *last_checkpoint = (jump_if) ? last_and: last_or;
        parsing_node *next_condition_root = (jump_if) ? last_or : last_and;

        parsing_node **jump_node_ptr = (parsing_node**) ( last_checkpoint==NULL ? NULL :  get_from_p_node_p_node_hash_map(most_left_node_map, last_checkpoint->right));
        parsing_node *jump_node= jump_node_ptr ? *jump_node_ptr : NULL;

        parsing_node **next_node = (parsing_node **)  (next_condition_root==NULL ? NULL : get_from_p_node_p_node_hash_map(most_left_node_map, next_condition_root->right));
        parsing_node *next_condition =  (next_condition_root) ? *next_node : NULL;

        jump_infos infos = {0};
        const size_t *jump_node_level_ptr =  get_from_p_node_size_t_hash_map(levels, last_checkpoint);
        const size_t *next_condition_level_ptr = get_from_p_node_size_t_hash_map(levels, next_condition_root);
        size_t jump_node_level = jump_node_level_ptr ? *jump_node_level_ptr  : 0;
        size_t next_condition_level = next_condition_level_ptr ? *next_condition_level_ptr : 0;
        if (jump_node_level<next_condition_level || (jump_node_level==next_condition_level && jump_if)) {
            infos=(jump_infos) {.jump_if = jump_if, .jump_node = jump_node, .next_node = next_condition}; // T
        }
        else {
            infos=(jump_infos) {.jump_if =  !jump_if,  .jump_node =next_condition,  .next_node =  jump_node};
        }

        put_to_p_node_jump_hash_map(jumps_map, node, infos );
    }
}

p_node_jump_hash_map *map_condition_jumps(parsing_node *root) {
    p_node_p_node_hash_map *most_left = new_p_node_p_node_hash_map(hash_node_addr, equals_node_addr, display_node_node_entry);
    map_most_not_logical_node_left(root, most_left);
    p_node_jump_hash_map *res =  new_p_node_jump_hash_map(hash_node_addr, equals_node_addr, print_node_jump_entry);
    p_node_size_t_hash_map *levels = new_p_node_size_t_hash_map(hash_node_addr, equals_node_addr, print_p_node_size_t_entry);
    put_to_p_node_size_t_hash_map(levels, NULL , 0);
    map_condition_jumps_recursive(root, NULL, NULL, NULL, most_left,levels,  res);
    free_p_node_p_node_hash_map(most_left);
    free_p_node_size_t_hash_map(levels);
    return res;

}










