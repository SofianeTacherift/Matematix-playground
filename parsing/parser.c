#include "parser.h"
#include "token.h"
#include <stdarg.h>


#define P_NEW_LINE printf("\n");

#define RET_NULL_IF_ERROR if (parse->parsing_status== PARSING_ERROR ) { return NULL; }

#define DEBUG_PRINT printf("\n-----------------------------------------\n");



#define DEFINE_BINARY_PARSING_FUNCTION(NAME, CONDITION, PARSING_FUNC_LEFT, PARSING_FUNC_RIGHT) parsing_node * NAME (parser *parse) { \
    RET_NULL_IF_ERROR \
    parsing_node *left= PARSING_FUNC_LEFT(parse); \
    RET_NULL_IF_ERROR \
    parsing_node *center=NULL; \
    token current_token; \
    while ( current_token=get_current_token(parse), current_token.type==OPERATOR_TOKEN && CONDITION ) { \
         \
        parsing_node * new_center =new_parsing_node(); \
        new_center->type=BINARY_NODE; \
\
        new_center->operation=current_token.operation; \
        if (center==NULL) { \
            new_center->left=left; \
        } \
        else { \
            new_center->left=center; \
        } \
        advance(parse); \
        new_center->right=  PARSING_FUNC_RIGHT(parse); \
        RET_NULL_IF_ERROR \
        center=new_center; \
    } \
    if (center==NULL) {center = left;} \
    return center; \
}
DEFINE_BINARY_PARSING_FUNCTION(parse_logical_or, (current_token.operation==LOGICAL_OR_OPERATOR) , parse_logical_and, parse_logical_and)
DEFINE_BINARY_PARSING_FUNCTION(parse_logical_and, (current_token.operation==LOGICAL_AND_OPERATOR) , parse_comparison, parse_comparison)
DEFINE_BINARY_PARSING_FUNCTION(parse_additive, (current_token.operation==ADD_OPERATOR || current_token.operation==SUB_OPERATOR) , parse_multiplicative, parse_multiplicative)
DEFINE_BINARY_PARSING_FUNCTION(parse_multiplicative, (current_token.operation==MULTIPLY_OPERATOR || current_token.operation==DIVIDE_OPERATOR) , parse_primary, parse_primary)


token get_current_token(parser *parse) {
    if (parse->current<parse->tokens->size) {
        return parse->tokens->elements[parse->current];
    }
    return (token) {.type=EOF_TOKEN};
}

token get_next_token(parser *parse) {
    if (parse->current+1<parse->tokens->size) {
        return parse->tokens->elements[parse->current+1];
    }
    return (token) {.type=EOF_TOKEN}; 
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







parsing_node * parse_code(parser *parse) {
    if (parse->tokens==NULL) {return NULL;}
    parsing_node_linked_list * linked_list = new_parsing_node_linked_list();
    while (get_current_token(parse).type!=EOF_TOKEN) {
        parsing_node *statement= parse_statement(parse);
        RET_NULL_IF_ERROR
        if (is_conditional_node(statement)) {
            add_conditional_node_to_linked_list(linked_list, statement);
        }
        else {add_parsing_node_to_linked_list(linked_list, statement);}
    }
    parsing_node *result= linked_list->head;
    free(linked_list);
    return result;
}

parsing_node_linked_list * parse_scope(parser * parse) {
    token current = get_current_token(parse);
    parsing_node_linked_list *res = new_parsing_node_linked_list();

    if (current.type!=OPENING_SCOPE_TOKEN) {
        write_in_error_buffer(parse, current.line, current.character, "expected {");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }

    advance(parse);
    while ((current=get_current_token(parse)).type!=EOF_TOKEN && current.type!=CLOSING_SCOPE_TOKEN) {
        parsing_node * statement = parse_statement(parse);
        RET_NULL_IF_ERROR
        if (is_conditional_node(statement)) {
            add_conditional_node_to_linked_list(res, statement);
        }
        else {add_parsing_node_to_linked_list(res, statement);}
        
    }
    if (current.type!=CLOSING_SCOPE_TOKEN) {
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


    P_NEW_LINE

    if (current.type==OPENING_SCOPE_TOKEN) {
        parsing_node_linked_list * scope = parse_scope(parse);
        RET_NULL_IF_ERROR
        result = new_parsing_node_of(OPENING_SCOPE_NODE);
        result->right=scope->head;
        free(scope);
    }
    else if (current.type==IF_TOKEN) {
        result = parse_if_statement(parse);
        RET_NULL_IF_ERROR
        token current_token = get_current_token(parse);
        if (current_token.type!=EOF_TOKEN && current_token.type!=CLOSING_SCOPE_TOKEN) {
            parsing_node * current_node = result;
            parsing_node * jump = parse_statement(parse);
            while (current_node->next!=NULL) {
                current_node->jump=jump;
                current_node=current_node->next;
            }
            current_node->next=jump;
            current_node->jump=jump;
        }
    }
    else {    
        if (current.type==IDENTIFIER_TOKEN) {
        result = (get_next_token(parse).type==AFFECTATION_TOKEN) ? parse_affectation(parse) : parse_expression(parse);
        }
        else {
            result = parse_expression(parse);
        }
        token t =get_current_token(parse);
        RET_NULL_IF_ERROR
        if (t.type!=DELIMITER_TOKEN ) {
            write_in_error_buffer(parse, t.line, t.character, "(STATEMENT) expected ';'");
            parse->parsing_status=PARSING_ERROR;
            return NULL;
        }
        RET_NULL_IF_ERROR
        advance(parse);
    }
  
    return result;
    
    

}

parsing_node * parse_if_statement(parser *parse) {
    token current_token = get_current_token(parse);
    if (current_token.type!=IF_TOKEN) {
        write_in_error_buffer(parse, current_token.line, current_token.character, "expected 'if'");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }
    advance(parse);
    parsing_node * result = new_parsing_node_of(IF_NODE);
    result->condition=parse_logical_or(parse);

    RET_NULL_IF_ERROR

    result->true_condition=parse_statement(parse);
    RET_NULL_IF_ERROR
    if (get_current_token(parse).type==ELIF_TOKEN) {
        result->next=parse_elif_statement(parse);
    }
    else if (get_current_token(parse).type==ELSE_TOKEN) {
        result->next=parse_else_statement(parse);
    }
    return result;

    
}

parsing_node *parse_elif_statement(parser *parse) {
   token current_token = get_current_token(parse);
    if (current_token.type!=ELIF_TOKEN) {
        write_in_error_buffer(parse, current_token.line, current_token.character, "expected 'elif'");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }
    advance(parse);
    parsing_node * result = new_parsing_node_of(ELIF_NODE);
    result->condition=parse_logical_or(parse);
    RET_NULL_IF_ERROR
    result->true_condition=parse_statement(parse);
    RET_NULL_IF_ERROR
    if (get_current_token(parse).type==ELIF_TOKEN) {
        result->next=parse_elif_statement(parse);
    }
    else if (get_current_token(parse).type==ELSE_TOKEN) {
        result->next=parse_else_statement(parse);
    }
    return result;
}

parsing_node *parse_else_statement(parser *parse) {
   token current_token = get_current_token(parse);
    if (current_token.type!=ELSE_TOKEN) {
        write_in_error_buffer(parse, current_token.line, current_token.character, "expected 'else'");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }
    advance(parse);
    parsing_node * result = new_parsing_node_of(ELSE_NODE);
    result->true_condition=parse_statement(parse);
    RET_NULL_IF_ERROR
    return result;

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


parsing_node * parse_comparison(parser * parse) {
    RET_NULL_IF_ERROR
    parsing_node * left = parse_additive(parse);
    RET_NULL_IF_ERROR
    parsing_node *center=NULL;
    token current=get_current_token(parse);
    if (is_comparaison_operator_token(current)) {
        center = operator_token_to_parsing_node(current);
        center->left=left;
        advance(parse);
        center->right=parse_additive(parse);
        RET_NULL_IF_ERROR
    }
    else {
        center=left;   
    }


    return center;
}



parsing_node * parse_unary(parser * parse) {
    RET_NULL_IF_ERROR
    token t = get_current_token(parse);
    parsing_node * res = operator_token_to_parsing_node(t);
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
    else if (current.type==OPERATOR_TOKEN &&  is_unary_operator(current.operation)) {
        result = parse_unary(parse);
    }
    else if (current.type==IDENTIFIER_TOKEN) {
        result=parse_identifier(parse);
    }
    else if (current.type==OPENING_PARENTHESE_TOKEN) {
        advance(parse);
        result=parse_expression(parse);
        token t=get_current_token(parse);
        if (t.type!=CLOSING_PARENTHESE_TOKEN) {
            parse->parsing_status=PARSING_ERROR;
            write_in_error_buffer(parse, t.line, t.character, "(PRIMARY) expected ')'");
            return NULL;
        }
        advance(parse);
    }
    else {

        int line=current.line;
        int character=current.character;

        write_in_error_buffer(parse, line, character, "(PRIMARY) expected a number, an identifier, an unary, or  an opening parenthese");
        parse->parsing_status=PARSING_ERROR;

        return NULL;
    }
    return result;
}














void write_in_error_buffer(parser *parse, int line, int character, char * message) {
    snprintf(parse->parsing_error_buffer, sizeof(parse->parsing_error_buffer), "Error in parsing at line %d character %d : %s.\n", line+1, character+1, message);
}













