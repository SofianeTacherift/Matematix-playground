#include "parser.h"
#include "token.h"
#include <stdarg.h>


#define P_NEW_LINE printf("\n");

#define RET_NULL_IF_ERROR if (parse->parsing_status== PARSING_ERROR ) { return NULL; }

#define DEBUG_PRINT printf("\n-----------------------------------------\n");





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




bool is_unary_operator_token(token t ) {
    return t.type==UNARY_MINUS;
}




parsing_node * parse_code(parser *parse) {
    if (parse->tokens==NULL) {return NULL;}
    parsing_node_linked_list * linked_list = new_parsing_node_linked_list();
    while (get_current_token(parse).type!=END) {
        parsing_node *statement= parse_statement(parse);
        RET_NULL_IF_ERROR
        add_parsing_node(linked_list, statement);
    }
    parsing_node *result= linked_list->head;
    free(linked_list);
    return result;
}

parsing_node_linked_list * parse_scope(parser * parse) {
    token current = get_current_token(parse);
    parsing_node_linked_list *res = new_parsing_node_linked_list();

    if (current.type!=OPENING_SCOPE) {
        write_in_error_buffer(parse, current.line, current.character, "expected {");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }

    advance(parse);
    while ((current=get_current_token(parse)).type!=END && current.type!=CLOSING_SCOPE) {
        parsing_node * statement = parse_statement(parse);
        RET_NULL_IF_ERROR
        add_parsing_node(res, statement);
    }
    if (current.type!=CLOSING_SCOPE) {
        write_in_error_buffer(parse, current.line, current.character, "expected }");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }
    advance(parse);

    return res;
}



parsing_node * parse_statement(parser *parse) {
    RET_NULL_IF_ERROR
    token current = get_current_token(parse);
    parsing_node * result;

    print_token(current);
    P_NEW_LINE

    if (current.type==OPENING_SCOPE) {
        parsing_node_linked_list * scope = parse_scope(parse);
        RET_NULL_IF_ERROR
        result = new_parsing_node_of(OPENING_SCOPE_NODE);
        result->right=scope->head;
        free(scope);
    }
    else {    
        if (current.type==IDENTIFIER) {
        result = (get_next_token(parse).type==AFFECTATION) ? parse_affectation(parse) : parse_expression(parse);
        }
        else {
            result = parse_expression(parse);
        }
        token t =get_current_token(parse);
        if (t.type!=DELIMITER) {
            write_in_error_buffer(parse, t.line, t.character, "expected ';'");
            parse->parsing_status=PARSING_ERROR;
            return NULL;
        }
        advance(parse);
    }
  
    return result;
    
    

}

parsing_node * parse_if_statement(parser *parse) {

    return NULL;

}

parsing_node * parse_affectation(parser *parse) {
        
    RET_NULL_IF_ERROR

    parsing_node *left = parse_identifier(parse);
    RET_NULL_IF_ERROR
    parsing_node * result = new_parsing_node();
    advance(parse);
    parsing_node * right = parse_expression(parse);
    RET_NULL_IF_ERROR
    result->type=AFFECTATION_NODE;
    result->left=left;
    result->right=right;
    return result;

}

parsing_node *parse_expression(parser *parse) {
    RET_NULL_IF_ERROR
    return parse_logical_or(parse);
}



parsing_node * parse_identifier(parser * parse) {
    RET_NULL_IF_ERROR
    parsing_node * res = new_parsing_node();
    token current = get_current_token(parse);
    res->type=VARIABLE_NODE;
    res->string_val=current.string_val;
    advance(parse);
    return res;
}

parsing_node * parse_logical_or(parser *parse) {
    RET_NULL_IF_ERROR
    parsing_node *left=parse_logical_and(parse);
    RET_NULL_IF_ERROR
    parsing_node *center=NULL;
    token current;
    while ((current = get_current_token(parse)   ).type==OR) {

        parsing_node * new_center =new_parsing_node();
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

parsing_node * parse_logical_and(parser *parse) {
    RET_NULL_IF_ERROR
    parsing_node *left=parse_comparaison(parse);
    RET_NULL_IF_ERROR
    parsing_node *center=NULL;
    token current;
    while ((current = get_current_token(parse)   ).type==AND) {

        parsing_node * new_center =new_parsing_node();
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

parsing_node * parse_comparaison(parser * parse) {
    RET_NULL_IF_ERROR
    parsing_node * left = parse_additive(parse);
    RET_NULL_IF_ERROR
    parsing_node *center=NULL;
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

parsing_node * parse_additive(parser *parse) {
    RET_NULL_IF_ERROR
    parsing_node *left=parse_multiplicative(parse);
    RET_NULL_IF_ERROR
    parsing_node *center=NULL;
    token current;
    while ((current = get_current_token(parse)   ).type==ADD || current.type==SUB ) {

        parsing_node * new_center =new_parsing_node();
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

parsing_node * parse_multiplicative(parser * parse) {
    RET_NULL_IF_ERROR
    parsing_node *left = parse_primary(parse);
    RET_NULL_IF_ERROR
    parsing_node *center=NULL;
    token current;

    while ((current=get_current_token(parse)).type==MULTIPLY || current.type==DIVIDE) {
        parsing_node *new_center= new_parsing_node();
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

parsing_node * parse_unary(parser * parse) {
    RET_NULL_IF_ERROR
    token t = get_current_token(parse);
    parsing_node * res = unary_token_to_node(t);
    advance(parse);
    res->right=parse_primary(parse);
    return res;
}

parsing_node * parse_primary(parser * parse) {
    RET_NULL_IF_ERROR
    token current = get_current_token(parse);
    parsing_node * result;
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














void write_in_error_buffer(parser *parse, int line, int character, char * message) {
    snprintf(parse->parsing_error_buffer, sizeof(parse->parsing_error_buffer), "Error in parsing at line %d character %d : %s.\n", line+1, character+1, message);
}













