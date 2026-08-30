#include "parser.h"
#include "token.h"
#include <stdarg.h>


#define P_NEW_LINE printf("\n");



#define DEBUG_PRINT printf("\n-----------------------------------------\n");



#define DEFINE_BINARY_PARSING_FUNCTION(NAME, CONDITION, PARSING_FUNC_LEFT, PARSING_FUNC_RIGHT) parsing_node * NAME (parser *parse) { \
      \
    parsing_node *left= PARSING_FUNC_LEFT(parse); \
      \
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
          \
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







parsing_node * parse_main_scope(parser *parse) {
    if (parse->tokens==NULL) {return NULL;}
    parsing_node_linked_list * res = new_parsing_node_linked_list();
    while (get_current_token(parse).type!=EOF_TOKEN) {
        parsing_node_linked_list * statement = parse_statement(parse);
        merge_linked_lists(res, statement);
        free(statement);
    }
    parsing_node *result= res->head;
    free(res);
    return result;
}

parsing_node_linked_list * parse_scope(parser * parse) {
    token current = get_current_token(parse);
    parsing_node_linked_list *res = new_parsing_node_linked_list();

    if (current.type!=OPENING_SCOPE_TOKEN) {
        write_in_error_buffer(parse, current, "expected {");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }

    advance(parse);
    while ((current=get_current_token(parse)).type!=EOF_TOKEN && current.type!=CLOSING_SCOPE_TOKEN) {
        parsing_node_linked_list * statement = parse_statement(parse);
        merge_linked_lists(res, statement);
        free(statement);
    }
    if (current.type!=CLOSING_SCOPE_TOKEN) {
        write_in_error_buffer(parse, current, "expected }");
        parse->parsing_status=PARSING_ERROR;
        return NULL;
    }
    advance(parse);

    return res;
}



parsing_node_linked_list * parse_statement(parser *parse) {
     
    token current = get_current_token(parse);
    parsing_node_linked_list * result;

    if (current.type==OPENING_SCOPE_TOKEN) {
        parsing_node_linked_list * scope = parse_scope(parse);
        parsing_node *head = new_parsing_node_of(OPENING_SCOPE_NODE);
        head->right=scope->head;
        free(scope);
        result=new_parsing_node_linked_list();
        add_parsing_node_to_linked_list(result, head);
    }
    else if (current.type==IF_TOKEN) {
        result = parse_if_statement(parse);
    }
    else {
        parsing_node *instruction =parse_instruction(parse);
        result = new_parsing_node_linked_list();
        add_parsing_node_to_linked_list(result, instruction);
        token t =get_current_token(parse);
        
        if (t.type!=DELIMITER_TOKEN && !parse->parsing_status==PARSING_ERROR) {
            write_in_error_buffer(parse, t, "(STATEMENT) expected ';'");
            parse->parsing_status=PARSING_ERROR;
            result=NULL;
        }

        if (parse->parsing_status==PARSING_ERROR) {
            while (t=get_current_token(parse), t.type!=EOF_TOKEN && t.type!=CLOSING_SCOPE_TOKEN) {
                advance(parse);
            }
            parse->parsing_status=NO_PARSING_ERROR;
        }

         
        advance(parse);

    }
    return result;
}


parsing_node * parse_conditional_node(parser *parser) {
    token current_token = get_current_token(parser);
    int type = conditional_token_to_parsing_node_type(current_token);
    if (type==NONE_NODE) {
        return NULL;
    }
    parsing_node *result = new_parsing_node_of( type );
    advance(parser);
    if (type!=ELSE_NODE) {
        result->condition=parse_logical_or(parser);
    }
    parsing_node_linked_list * true_branch = parse_statement(parser);
    result->true_condition=true_branch->head;
    free(true_branch);
    return result;
}
parsing_node_linked_list * parse_if_statement(parser *parse) {
    token current_token = get_current_token(parse);
    parsing_node_linked_list * result = new_parsing_node_linked_list();
    add_parsing_node_to_linked_list(result, parse_conditional_node(parse));
    while ((current_token=get_current_token(parse)).type==ELIF_TOKEN) {
        add_parsing_node_to_linked_list(result, parse_conditional_node(parse));
    }
    if ((current_token=get_current_token(parse)).type==ELSE_TOKEN) {
        add_parsing_node_to_linked_list(result, parse_conditional_node(parse));     
    }
    return result;
}


parsing_node *parse_instruction(parser *parser) {
    parsing_node *res=NULL;
    token current_token = get_current_token(parser);
    if (current_token.type==IDENTIFIER_TOKEN) {
    res = (get_next_token(parser).type==AFFECTATION_TOKEN) ? parse_affectation(parser) : parse_expression(parser);
    }
    else {
        res = parse_expression(parser);
    }
    return res;
}

parsing_node * parse_affectation(parser *parse) {
    parsing_node *left = parse_identifier(parse);
    parsing_node * result = new_parsing_node();
    advance(parse);
    parsing_node * right = parse_expression(parse);
     
    result->type=AFFECTATION_NODE;
    result->left=left;
    result->right=right;
    return result;

}

parsing_node *parse_expression(parser *parse) {
    return parse_logical_or(parse);
}



parsing_node * parse_identifier(parser * parse) {
    parsing_node * res = new_parsing_node();
    token current = get_current_token(parse);
    res->type=VARIABLE_NODE;
    res->string_val=current.string_val;
    advance(parse);
    return res;
}


parsing_node * parse_comparison(parser * parse) {
     
    parsing_node * left = parse_additive(parse);
     
    parsing_node *center=NULL;
    token current=get_current_token(parse);
    if (is_comparaison_operator_token(current)) {
        center = operator_token_to_parsing_node(current);
        center->left=left;
        advance(parse);
        center->right=parse_additive(parse);
         
    }
    else {
        center=left;   
    }


    return center;
}



parsing_node * parse_unary(parser * parse) {
    token t = get_current_token(parse);
    parsing_node * res = operator_token_to_parsing_node(t);
    advance(parse);
    res->right=parse_primary(parse);
    return res;
}

parsing_node * parse_primary(parser * parse) {
     
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
            write_in_error_buffer(parse, t, "(PRIMARY) expected ')'");
            return NULL;
        }
        advance(parse);
    }
    else {
        write_in_error_buffer(parse, current, "(PRIMARY) expected a number, an identifier, an unary, or  an opening parenthese");
        parse->parsing_status=PARSING_ERROR;

        return NULL;
    }
    return result;
}












add_error (parser *parser, token t, char * message) {
    
}

void write_in_error_buffer(parser *parse, token t, char * message) {
    advance(parse);
    snprintf(parse->parsing_error_buffer, sizeof(parse->parsing_error_buffer), "Error in parsing at line %d character %d : %s.\n", t.line+1, t.character+1, message);
    printf(parse->parsing_error_buffer);
    
}













