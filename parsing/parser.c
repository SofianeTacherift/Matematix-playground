#include "parser.h"
#include "token.h"
#include <stdarg.h>
#include <stdio.h>


#define P_NEW_LINE printf("\n");

#define RETURN_NULL_IF_ERROR(variable, ...) if (variable==NULL) {  \
    free_nodes(__VA_ARGS__); \
    return NULL;\
} \

#define ADV_PARSER_WHILE_NOT_CURRTYPE(PARSER, ...)  \
    while (!is_an_token_of_type( get_current_token( PARSER) , __VA_ARGS__ )) { \
        advance( PARSER );\
    } \
 \


#define DEFINE_BINARY_PARSING_FUNCTION(NAME, CONDITION, PARSING_FUNC_LEFT, PARSING_FUNC_RIGHT) parsing_node * NAME (parser *parse) { \
      \
    parsing_node *left= PARSING_FUNC_LEFT(parse); \
      \
    RETURN_NULL_IF_ERROR(left, 0) \
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
        new_center->right=PARSING_FUNC_RIGHT(parse); \
          \
        RETURN_NULL_IF_ERROR(new_center->right, 1, new_center) \
        center=new_center; \
    } \
    if (center==NULL) {center = left;} \
    return center; \
}
DEFINE_BINARY_PARSING_FUNCTION(parse_logical_or, (current_token.operation==LOGICAL_OR_OPERATOR) , parse_logical_and, parse_logical_and)
DEFINE_BINARY_PARSING_FUNCTION(parse_logical_and, (current_token.operation==LOGICAL_AND_OPERATOR) , parse_comparison, parse_comparison)
DEFINE_BINARY_PARSING_FUNCTION(parse_additive, (current_token.operation==ADD_OPERATOR || current_token.operation==SUB_OPERATOR) , parse_multiplicative, parse_multiplicative)
DEFINE_BINARY_PARSING_FUNCTION(parse_multiplicative, (current_token.operation==MULTIPLY_OPERATOR || current_token.operation==DIVIDE_OPERATOR) , parse_power, parse_power)
DEFINE_BINARY_PARSING_FUNCTION(parse_power, (current_token.operation==POWER_OPERATOR),parse_primary, parse_power)

bool is_an_token_of_type(token t, ...) {
    va_list args;
    va_start(args, t);
    int  current = va_arg(args, int);
    while (current!=NONE_TOKEN) {
        if (current==t.type) {
            return true;
        }
        current=va_arg(args, int);
    }
    va_end(args);
    return false;
}



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

bool has_parsing_error(parser *parse) {
    return parse->parsing_status==PARSING_ERROR;
}



parser * new_parser(token_array_list * tokens) {
    parser *result=calloc(1, sizeof(parser));
    result->parsing_errors=new_parsing_error_array_list();
    result->current=0;
    result->tokens=tokens;
    return result;
}





#define print_current_token(PARSER) print_token(get_current_token(PARSER)); printf("\n");

parsing_node * parse_main_scope(parser *parse) {
    if (parse->tokens==NULL) {return NULL;}
    parsing_node_linked_list * res = new_parsing_node_linked_list();
    while (get_current_token(parse).type!=EOF_TOKEN) {
        parsing_node_linked_list * statement = parse_statement(parse);
        if (statement==NULL) {
            ADV_PARSER_WHILE_NOT_CURRTYPE(parse, OPENING_SCOPE_TOKEN,  DELIMITER_TOKEN, EOF_TOKEN,  NONE_TOKEN)
            if (get_current_token(parse).type!=OPENING_SCOPE_TOKEN) {
                advance(parse);
            }
        }
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
    advance(parse);

    while ((current=get_current_token(parse)).type!=EOF_TOKEN && current.type!=CLOSING_SCOPE_TOKEN) {
        parsing_node_linked_list * statement = parse_statement(parse);
        if (statement==NULL) {
            ADV_PARSER_WHILE_NOT_CURRTYPE(parse, OPENING_SCOPE_NODE, DELIMITER_TOKEN, CLOSING_SCOPE_TOKEN, EOF_TOKEN,  NONE_TOKEN)
            if (get_current_token(parse).type!=OPENING_SCOPE_TOKEN) {
                advance(parse);
            }
        }
        else {
            merge_linked_lists(res, statement);
            free(statement);
        }
    }

    if (current.type!=CLOSING_SCOPE_TOKEN && parse->parsing_status!=PARSING_ERROR) {
        write_in_error_buffer(parse, current, "expected }");
        parse->parsing_status=PARSING_ERROR;
        free_parsing_node_linked_list(res);
        return NULL;
    }
    advance(parse);

    return res;
}



parsing_node_linked_list * parse_statement(parser *parse) {
    
    token current = get_current_token(parse);
    parsing_node_linked_list * result=NULL;

    if (current.type==OPENING_SCOPE_TOKEN) {
        parsing_node_linked_list * scope = parse_scope(parse);
        if (scope==NULL) {
            return NULL; // if there is a scope error, this mean we are at the EOF and that there is no '}' finishing a scope
        }
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
        if (instruction!=NULL) {
            result = new_parsing_node_linked_list();
            add_parsing_node_to_linked_list(result, instruction);
        }
        else {
            return NULL;
        }

        token t =get_current_token(parse);
        if (t.type!=DELIMITER_TOKEN) {
            write_in_error_buffer(parse, t, "(STATEMENT) expected ';'");
            parse->parsing_status=PARSING_ERROR;
            free(result);
            result=NULL;
        }
        else {
            advance(parse);
        }

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
        RETURN_NULL_IF_ERROR(result->condition, 1,result);
    }
    parsing_node_linked_list * true_branch = parse_statement(parser);
    if (true_branch==NULL) {
        ADV_PARSER_WHILE_NOT_CURRTYPE(parser, DELIMITER_TOKEN, CLOSING_SCOPE_TOKEN, EOF_TOKEN,  NONE_TOKEN)
        if (get_current_token(parser).type!=CLOSING_SCOPE_TOKEN) {
            advance(parser);
        }
        free_tree_node(result, true);
        return NULL;
    }
    result->true_condition=true_branch->head;
    free(true_branch);
    return result;
}


parsing_node_linked_list * parse_if_statement(parser *parse) {
    token current_token = get_current_token(parse);
    parsing_node_linked_list * result = new_parsing_node_linked_list();

    parsing_node *if_statement = parse_conditional_node(parse);

    if (if_statement==NULL) {
        free_parsing_node_linked_list(result);
        return NULL;
    }

    add_parsing_node_to_linked_list(result, if_statement);

    while ((current_token=get_current_token(parse)).type==ELIF_TOKEN) {
        parsing_node *elif_statement = parse_conditional_node(parse);
        if (elif_statement==NULL) {
            free_parsing_node_linked_list(result);
            return NULL;
        }
        add_parsing_node_to_linked_list(result, elif_statement);
    }
    if ((current_token=get_current_token(parse)).type==ELSE_TOKEN) {
        parsing_node *else_statement = parse_conditional_node(parse);
        if (else_statement==NULL) {
            free_parsing_node_linked_list(result);
            return NULL;
        }
        add_parsing_node_to_linked_list(result, else_statement); 
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
    RETURN_NULL_IF_ERROR(res, 0)
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
    RETURN_NULL_IF_ERROR(left, 0)
     
    parsing_node *center=NULL;
    token current=get_current_token(parse);
    if (is_comparaison_operator_token(current)) {
        center = operator_token_to_parsing_node(current);
        center->left=left;
        advance(parse);
        center->right=parse_additive(parse);
        RETURN_NULL_IF_ERROR(center->right, 2, left, center)
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
    res->right=parse_power(parse);
    RETURN_NULL_IF_ERROR(res->right, 1, res)
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
        RETURN_NULL_IF_ERROR(result, 1, result)
        token t=get_current_token(parse);
        if (t.type!=CLOSING_PARENTHESE_TOKEN) {
            parse->parsing_status=PARSING_ERROR;
            write_in_error_buffer(parse, t, "(PRIMARY) expected ')'");
            free(result);
            return NULL;
        }
        RETURN_NULL_IF_ERROR(result, 0)
        advance(parse);
    }
    else {
  
        write_in_error_buffer(parse, current, "(PRIMARY) expected a number, an identifier, an unary, or  an opening parenthese");
        parse->parsing_status=PARSING_ERROR;

        return NULL;
    }
    RETURN_NULL_IF_ERROR(result, 0)
    return result;
}












add_error (parser *parser, token t, char * message) {
    
}

void write_in_error_buffer(parser *parse, token t, char * message) {
    parse->parsing_status=PARSING_ERROR;
    parsing_error error = {.token=t};
    snprintf(error.message, sizeof(error.message), message);
    add_parsing_error(parse->parsing_errors, error);
}

void free_nodes(int count, ...) {
    va_list pointers;
    va_start(pointers, count);
    for (int i=0; i<count; i++ ) {
        parsing_node *p = va_arg(pointers, parsing_node*);
        free_tree_node(p, true);
    }
} 










