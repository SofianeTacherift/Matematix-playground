#include "parser.h"
#include "token.h"
#include <stdarg.h>


#define P_NEW_LINE printf("\n");

#define RET_NULL_IF_ERROR if (parse->parsing_status== PARSING_ERROR ) { return NULL; }

#define DEBUG_PRINT printf("\n-----------------------------------------\n");


node * new_node() {
    node *res=malloc(sizeof(node));
    res->left=NULL;
    res->right=NULL;
    return res;
}


token get_current_token(parser *parse) {
    if (parse->current<parse->tokens->size) {
        return parse->tokens->elements[parse->current];
    }
    return (token) {.type=END};
}

token get_next_token(parser *parse) {
    if (parse->current+1<parse->tokens->size) {
        return parse->tokens->elements[parse->current+1];
    }
    return (token) {.type=END}; 
}

token advance(parser *parse ) {
    parse->current++;
    return get_current_token(parse);
}

bool parsing_error(parser *parse) {
    return parse->parsing_status==PARSING_ERROR;
}



parser * new_parser(token_array_list * tokens) {
    void* p =malloc(sizeof(parser));
    parser *result = p;
    result->current=0;
    result->tokens=tokens;
    return result;
}

node * token_num_to_node(token t) {
    node * node=new_node();
    switch (t.type) {
    case INT:
        node->type=INT_NODE;
        node->int_val=t.int_val;
        break;
    case FLOAT:
        node->type=FLOAT_NODE;
        node->float_val=t.float_val;
        break;
    case DOUBLE:
        node->type=DOUBLE_NODE;
        node->double_val=t.double_val;
        break;
    default:
        return NULL;
    return node;
    }
}



bool is_unary_operator_token(token t ) {
    return t.type==UNARY_MINUS;
}

node * unary_token_to_node(token t) {

    node * result = new_node();
    if (t.type=UNARY_MINUS) {
        result->type=UNARY_NODE;
        result->operation=UNARY_MINUS;
    }
    return result;
}


node *  comparaison_token_to_node(token t) {
    node * res = new_node();
    res->type=BINARY_NODE;
    switch (t.type) {
    case EQUALS:
        res->operation=EQUALS_OPERATOR;
        break;
    case GREATER_THAN:
        res->operation=GREATER_THAN_OPERATOR;
        break;
    case GREATER_OR_EQUAL:
        res->operation=GREATER_OR_EQUAL_OPERATOR;
        break;
    case LESS_THAN:
        res->operation=LESS_THAN_OPERATOR;
        break;
    case LESS_OR_EQUAL:
        res->operation=LESS_OR_EQUAL_OPERATOR;
        break;
    default:
        res->type=NONE;
    }
    return res;
}

node * begin_parsement(parser *parse) {
    if (parse->tokens==NULL) {return NULL;}
    return parse_statement(parse);
}



node * parse_statement(parser *parse) {
    token current = get_current_token(parse);
    node * result;
    if (current.type==IDENTIFIER) {
        result = (get_next_token(parse).type==AFFECTATION) ? parse_affectation(parse) : parse_expression(parse);
    }
    else {    
        result = parse_expression(parse);
    }
    RET_NULL_IF_ERROR
    token t =get_current_token(parse);
    if (t.type!=DELIMITER) {
        write_in_error_buffer(parse, t.line, t.character, "expected ';'");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }
    advance(parse);
    t=get_current_token(parse);
    if (t.type!=END) {
        result->next=parse_statement(parse);
        return result;
    }
    else {
        return result;
    }
    

}


node * parse_affectation(parser *parse) {
        
    RET_NULL_IF_ERROR

    node *left = parse_identifier(parse);
    node * result = new_node();
    advance(parse);
    node * right = parse_expression(parse);
    result->type=AFFECTATION_NODE;
    result->left=left;
    result->right=right;
    return result;

}

node *parse_expression(parser *parse) {
    RET_NULL_IF_ERROR
    return parse_logical_or(parse);
}



node * parse_identifier(parser * parse) {
    RET_NULL_IF_ERROR
    node * res = new_node();
    token current = get_current_token(parse);
    res->type=VARIABLE_NODE;
    res->string_val=current.string_val;
    advance(parse);
    return res;
}

node * parse_logical_or(parser *parse) {
    RET_NULL_IF_ERROR
    node *left=parse_logical_and(parse);
    RET_NULL_IF_ERROR
    node *center=NULL;
    token current;
    while ((current = get_current_token(parse)   ).type==OR) {

        node * new_center =new_node();
        new_center->type=BINARY_NODE;

        new_center->operation=OR_OPERATOR;
        if (center==NULL) {
            new_center->left=left;
        }
        else {
            new_center->left=center;
        }
        advance(parse);
        new_center->right=parse_logical_and(parse);
        RET_NULL_IF_ERROR
        center=new_center;
    }
    if (center==NULL) {center = left;}

    
    if (!is_operator_token(current) && current.type!=END && current.type!=DELIMITER && current.type!=CLOSING_PARENTHESE) {
        parse->parsing_status=PARSING_ERROR;
        write_in_error_buffer(parse, current.line, current.character,"expected an operator, ')', ';' or EOF" );
        free_tree_node(center);
        return NULL;
    }

    return center;


}

node * parse_logical_and(parser *parse) {
    RET_NULL_IF_ERROR
    node *left=parse_comparaison(parse);
    RET_NULL_IF_ERROR
    node *center=NULL;
    token current;
    while ((current = get_current_token(parse)   ).type==AND) {

        node * new_center =new_node();
        new_center->type=BINARY_NODE;

        new_center->operation=AND_OPERATOR;
        if (center==NULL) {
            new_center->left=left;
        }
        else {
            new_center->left=center;
        }
        advance(parse);
        new_center->right=parse_comparaison(parse);
        RET_NULL_IF_ERROR
        center=new_center;
    }
    if (center==NULL) {center = left;}

    
    if (!is_operator_token(current) && current.type!=END && current.type!=DELIMITER && current.type!=CLOSING_PARENTHESE) {
        parse->parsing_status=PARSING_ERROR;
        write_in_error_buffer(parse, current.line, current.character,"expected an operator, ')', ';' or EOF" );
        free_tree_node(center);
        return NULL;
    }

    return center;
}

node * parse_comparaison(parser * parse) {
    RET_NULL_IF_ERROR
    node * left = parse_additive(parse);
    RET_NULL_IF_ERROR
    node *center=NULL;
    token current=get_current_token(parse);
    if (is_comparaison_operator_token(current)) {
        center = comparaison_token_to_node(current);
        center->left=left;
        advance(parse);
        center->right=parse_additive(parse);
        RET_NULL_IF_ERROR
    }
    else {
        center=left;   
    }

    if (!is_operator_token(current) && current.type!=END && current.type!=DELIMITER && current.type!=CLOSING_PARENTHESE) {
        parse->parsing_status=PARSING_ERROR;
        write_in_error_buffer(parse, current.line, current.character,"expected an operator, ')', ';' or EOF" );
        free_tree_node(center);
        return NULL;
    }
    return center;
}

node * parse_additive(parser *parse) {
    RET_NULL_IF_ERROR
    node *left=parse_multiplicative(parse);
    RET_NULL_IF_ERROR
    node *center=NULL;
    token current;
    while ((current = get_current_token(parse)   ).type==ADD || current.type==SUB ) {

        node * new_center =new_node();
        new_center->type=BINARY_NODE;

        new_center->operation=(current.type==ADD) ? ADD_OPERATOR : SUB_OPERATOR;
        if (center==NULL) {
            new_center->left=left;
        }
        else {
            new_center->left=center;
        }
        advance(parse);
        new_center->right=parse_multiplicative(parse);
        RET_NULL_IF_ERROR
        center=new_center;
    }
    if (center==NULL) {center = left;}

    
    if (!is_operator_token(current) && current.type!=END && current.type!=DELIMITER && current.type!=CLOSING_PARENTHESE) {
        parse->parsing_status=PARSING_ERROR;
        write_in_error_buffer(parse, current.line, current.character,"expected an operator, ')', ';' or EOF" );
        free_tree_node(center);
        return NULL;
    }

    return center;


}

node * parse_multiplicative(parser * parse) {
    RET_NULL_IF_ERROR
    node *left = parse_primary(parse);
    RET_NULL_IF_ERROR
    node *center=NULL;
    token current;

    while ((current=get_current_token(parse)).type==MULTIPLY || current.type==DIVIDE) {
        node *new_center= new_node();
        new_center->type=BINARY_NODE;

        new_center->operation=(current.type==MULTIPLY) ? MULTIPLY_OPERATOR : DIVIDE_OPERATOR;

        if (center==NULL) {
            new_center->left=left;
        }
        else {
            new_center->left=center;
        }
        advance(parse);
        new_center->right=parse_primary(parse);
        RET_NULL_IF_ERROR
        center=new_center;
    }

    if (center==NULL) {center = left;}
    
    if (!is_operator_token(current) && current.type!=END && current.type!=DELIMITER && current.type!=CLOSING_PARENTHESE) {
        parse->parsing_status=PARSING_ERROR;

        write_in_error_buffer(parse, current.line, current.character,"expected an operator, ')', ';' or EOF" );
        free_tree_node(center);
        return NULL;
    }

    if (center==NULL) {center = left;}
    return center;
}

node * parse_unary(parser * parse) {
    RET_NULL_IF_ERROR
    token t = get_current_token(parse);
    node * res = unary_token_to_node(t);
    advance(parse);
    res->right=parse_primary(parse);
    return res;
}

node * parse_primary(parser * parse) {
    RET_NULL_IF_ERROR
    token current = get_current_token(parse);
    node * result;
    if (is_num_token(current)) {
        result=token_num_to_node(current);
        advance(parse);
    }
    else if (is_unary_operator_token(current)) {
        result = parse_unary(parse);
    }
    else if (current.type==IDENTIFIER) {
        result=parse_identifier(parse);
    }
    else if (current.type==OPENING_PARENTHESE) {
        advance(parse);
        result=parse_expression(parse);
        token t=get_current_token(parse);
        if (t.type!=CLOSING_PARENTHESE) {
            parse->parsing_status=PARSING_ERROR;
            write_in_error_buffer(parse, t.line, t.character, "expected ')'");
            return NULL;
        }
        advance(parse);
    }
    else {

        int line=current.line;
        int character=current.character;

        write_in_error_buffer(parse, line, character, "expected a number, an identifier, an unary, or  an opening parenthese");
        parse->parsing_status=PARSING_ERROR;

        return NULL;
    }
    return result;
}













void print_operation(operator op) {
    switch (op) {
    case ADD_OPERATOR:
        printf("+");
        break;
    case SUB_OPERATOR:
        printf("-");
        break;
    case MULTIPLY_OPERATOR:
        printf("*");
        break;
    case DIVIDE_OPERATOR:
        printf("/");
        break;
    case UNARY_MINUS_OPERATOR:
        printf("-");
        break;
    case EQUALS_OPERATOR:
        printf("==");
        break;
    case GREATER_THAN_OPERATOR:
        printf(">");
        break;
    case GREATER_OR_EQUAL_OPERATOR:
        printf(">=");
        break;
    case LESS_THAN_OPERATOR:
        printf("<");
        break;
    case LESS_OR_EQUAL_OPERATOR:
        printf("<=");
        break;
    case OR_OPERATOR:
        printf("|");
        break;
    case AND_OPERATOR:
        printf("&");
        break;
        
    }
}

void print_num_val(node *n) {
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

bool is_num_node(node * n) {
    return n->type==INT_NODE || n->type==FLOAT_NODE || n->type==DOUBLE_NODE;
}
void display_node(node * n) {
    if (n==NULL) {printf("NULL");}
    printf("node[ type=%s ", NODE_TYPE_STR[n->type]);
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

void display_tree_node(node * n) {
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
    if (n->next!=NULL) {
        printf("--> ");
        display_tree_node(n->next);
    }
}



// void write_in_error_buffer(parser *parse, int line, int character, int count, ...) {
//     va_list expected;
//     va_start(expected, count);
//     char buffer[] = parse->parsing_error_buffer;
//     char * expected_string[count];
//     snprintf(buffer, sizeof(buffer), "Error during parsing, expected ");
//     for (int i=0,j=0; i<count && j<sizeof(buffer);i++, j=strlen(buffer)) {
//         // expected_string[i]=TOKEN_TYPE_NAMES[va_arg(expected, int)];
//         int current = va_arg(expected, int);
//         char * end = strlen(buffer);
//         if (i<count-1) {snprintf(end, sizeof(buffer) - j, "%s, ",  TOKEN_TYPE_NAMES[current]);}
//         else {snprintf(end, sizeof(buffer) - j, "%s or ", TOKEN_TYPE_NAMES[current]);}
//     }
//     int len=strlen(buffer);
//     snprintf(buffer+len, sizeof(buffer) - len, " at line %d, character %d.", line, character);
//     va_end(expected); 
// }


void free_tree_node(node *n) {
    if (n==NULL) {
        return;
    }
    node *left =n->left;
    node *right = n->right;
    free(n);
    free_tree_node(left);
    free_tree_node(right);
}

// void write_in_error_buffer(parser *parse, int line, int , int count, ...) {
//     va_list expected;
//     va_start(expected, count);
//     #define buffer parse->parsing_error_buffer
//     char * expected_string[count];
//     snprintf(buffer, sizeof(buffer), "Error during parsing, expected ");
//     for (int i=0,j=0; i<count && j<sizeof(buffer);i++, j=strlen(buffer)) {
//         // expected_string[i]=TOKEN_TYPE_NAMES[va_arg(expected, int)];
//         char * current = va_arg(expected, char *);
//         char * end = buffer + strlen(buffer);
//         if (i<count-2) {
//             snprintf(end, sizeof(buffer) - j, "%s, ",  current);
//         }
//         else if (i==count-1) {
//             snprintf(end, sizeof(buffer) - j, "%s ",  current);
//         }
//         else {
//             snprintf(end, sizeof(buffer) - j, "%s or ", current);
//         }
//     }
//     int len=strlen(buffer);
//     snprintf(buffer+len, sizeof(buffer) - len, "at line %d, character %d.", line+1, character+1);
//     va_end(expected); 
//     #undef buffer
// }


void write_in_error_buffer(parser *parse, int line, int character, char * message) {
    snprintf(parse->parsing_error_buffer, sizeof(parse->parsing_error_buffer), "Error in parsing at line %d character %d : %s.\n", line+1, character+1, message);
}













