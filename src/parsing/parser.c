#include "parser.h"
#include "token.h"
#include <stdarg.h>
#include <stdio.h>
#include "parsing_node_linked_list.h"

typedef struct parser {
    TokenArrayList* tokens;
    size_t current_index;
    ParsingErrorArrayList* parsing_errors;
    int parsing_status;
} parser;

Token Parser_get_current_token(parser* parse);
Token Parser_get_next_token(parser* parse);
Token Parser_advance(parser* parse);
ParsingNode* parse_main_scope(parser* parse);
ParsingNodeLinkedList* parse_scope(parser* parse);
ParsingNodeLinkedList* parse_statement(parser* parse);
ParsingNodeLinkedList* parse_if_statement(parser* parse);
ParsingNode* parse_conditional_node(parser* parser);
ParsingNode* parse_affectation(parser* parse);
ParsingNode* parse_expression(parser* parse);
ParsingNode* parse_identifier(parser* parse);
ParsingNode* parse_logical_or(parser* parse);
ParsingNode* parse_logical_and(parser* parse);
ParsingNode* parse_comparison(parser* parse);
ParsingNode* parse_additive(parser* parse);
ParsingNode* parse_multiplicative(parser* parse);
ParsingNode* parse_power(parser* parse);
ParsingNode* parse_unary(parser* parse);
ParsingNode* parse_primary(parser* parse);
ParsingNode* parse_instruction(parser* parser);
void add_parsing_error_to_parser(parser* parse, Token current, char* message);

#define RETURN_NULL_IF_ERROR(variable, ...) if (variable==NULL) {  \
    free_nodes(__VA_ARGS__); \
    return NULL;\
}
#define ADV_PARSER_WHILE_NOT_CURRTYPE(PARSER, ...)  \
    while (!is_an_token_of_type( Parser_get_current_token( PARSER) , __VA_ARGS__ )) { \
        Parser_advance( PARSER );\
    }
#define print_current_token(PARSER) print_token(Parser_get_current_token(PARSER)); printf("\n");

#define DEFINE_BINARY_PARSING_FUNCTION(NAME, CONDITION, PARSING_FUNC_LEFT, PARSING_FUNC_RIGHT) ParsingNode * NAME (parser *parse) { \
    ParsingNode *left= PARSING_FUNC_LEFT(parse); \
      \
    RETURN_NULL_IF_ERROR(left, 0) \
    ParsingNode *center=NULL; \
    Token current_token; \
    while ( current_token=Parser_get_current_token(parse), current_token.type==OPERATOR_TOKEN && CONDITION ) {       \
        ParsingNode * new_center = ParsingNode_new(); \
        new_center->type=BINARY_NODE; \
        \
        new_center->operation=current_token.operation; \
        if (center==NULL) { \
            new_center->left=left; \
        } \
        else { \
            new_center->left=center; \
        } \
        Token t = Parser_get_current_token(parse); \
        Parser_advance(parse); \
        new_center->right=PARSING_FUNC_RIGHT(parse); \
        RETURN_NULL_IF_ERROR(new_center->right, 1, new_center) \
        center=new_center; \
    } \
    if (center==NULL) {center = left;} \
    return center; \
}
DEFINE_BINARY_PARSING_FUNCTION(parse_logical_or, (current_token.operation==LOGICAL_OR_OPERATOR), parse_logical_and,
                               parse_logical_and)
DEFINE_BINARY_PARSING_FUNCTION(parse_logical_and, (current_token.operation==LOGICAL_AND_OPERATOR), parse_comparison,
                               parse_comparison)
DEFINE_BINARY_PARSING_FUNCTION(parse_additive,
                               (current_token.operation==ADD_OPERATOR || current_token.operation==SUB_OPERATOR),
                               parse_multiplicative, parse_multiplicative)
DEFINE_BINARY_PARSING_FUNCTION(parse_multiplicative,
                               (current_token.operation==MULTIPLY_OPERATOR || current_token.operation==DIVIDE_OPERATOR),
                               parse_power, parse_power)
DEFINE_BINARY_PARSING_FUNCTION(parse_power, (current_token.operation==POWER_OPERATOR), parse_primary, parse_power)

void ParsingResult_deinit(ParsingResult parsing_res) {
    LexingResult_deinit(parsing_res.lexing_res);
    ParsingNode_free_recursive(parsing_res.head, true);
    if (parsing_res.errors != NULL) {
        ParsingErrorArrayList_free(parsing_res.errors);
    }
}

Token Parser_get_current_token(parser* parse) {
    if (parse->current_index < parse->tokens->size) {
        return parse->tokens->elements[parse->current_index];
    }
    return (Token){.type = EOF_TOKEN};
}

Token Parser_get_next_token(parser* parse) {
    if (parse != NULL && parse->current_index + 1 < parse->tokens->size) {
        return parse->tokens->elements[parse->current_index + 1];
    }
    return (Token){.type = EOF_TOKEN};
}

Token Parser_advance(parser* parse) {
    parse->current_index++;
    return Parser_get_current_token(parse);
}

bool Parser_has_error(parser* parse) {
    return parse->parsing_status == PARSING_ERROR;
}

parser* Parser_new(TokenArrayList* tokens) {
    parser* result = calloc(1, sizeof(parser));
    result->parsing_errors = ParsingErrorArrayList_new();
    result->current_index = 0;
    result->tokens = tokens;
    return result;
}

ParsingResult parse_from_file(FILE* file) {
    ParsingResult parsing_res = {0};

    LexingResult lexing_res = lex_file(file);
    parsing_res.lexing_res = lexing_res;
    if (lexing_res.lexing_status == LEXING_ERROR) {
        return parsing_res;
    }
    parser* parser = Parser_new(lexing_res.tokens);
    parsing_res.head = parse_main_scope(parser);
    parsing_res.parsing_status = parser->parsing_status;
    parsing_res.errors = parser->parsing_errors;
    free(parser);
    return parsing_res;
}

ParsingNode* parse_main_scope(parser* parse) {
    if (parse->tokens == NULL) { return NULL; }
    ParsingNodeLinkedList* res = ParsingNodeLinkedList_new();
    while (Parser_get_current_token(parse).type != EOF_TOKEN) {
        ParsingNodeLinkedList* statement = parse_statement(parse);
        if (statement == NULL) {
            ADV_PARSER_WHILE_NOT_CURRTYPE(parse, OPENING_SCOPE_TOKEN, DELIMITER_TOKEN, EOF_TOKEN, NONE_TOKEN)
            if (Parser_get_current_token(parse).type != OPENING_SCOPE_TOKEN) {
                Parser_advance(parse);
            }
        }
        else {
            ParsingNodeLinkedList_merge(res, statement);
            free(statement);
        }
    }
    ParsingNode* result = res->head;
    free(res);
    return result;
}

ParsingNodeLinkedList* parse_scope(parser* parse) {
    Token current;
    ParsingNodeLinkedList* scope = ParsingNodeLinkedList_new();
    Parser_advance(parse);
    while ((current = Parser_get_current_token(parse)).type != EOF_TOKEN && current.type != CLOSING_SCOPE_TOKEN) {
        ParsingNodeLinkedList* statement = parse_statement(parse);
        if (statement == NULL) {
            ADV_PARSER_WHILE_NOT_CURRTYPE(parse, OPENING_SCOPE_NODE, DELIMITER_TOKEN, CLOSING_SCOPE_TOKEN, EOF_TOKEN,
                                          NONE_TOKEN)
            Token current_token = Parser_get_current_token(parse);
            if (current_token.type != OPENING_SCOPE_TOKEN && current_token.type != CLOSING_SCOPE_TOKEN) {
                Parser_advance(parse);
            }
        }
        else {
            ParsingNodeLinkedList_merge(scope, statement);
            free(statement);
        }
    }

    if (current.type != CLOSING_SCOPE_TOKEN && parse->parsing_status != PARSING_ERROR) {
        add_parsing_error_to_parser(parse, current, "expected }");

        ParsingNodeLinkedList_free(scope);
        return NULL;
    }
    Parser_advance(parse);

    ParsingNodeLinkedList* result = ParsingNodeLinkedList_new();
    ParsingNode* head = ParsingNode_new_from_type(OPENING_SCOPE_NODE);
    head->right = scope->head;
    result->head = head;
    result->end = head;

    free(scope);

    return result;
}

ParsingNodeLinkedList* parse_statement(parser* parse) {
    Token current = Parser_get_current_token(parse);
    ParsingNodeLinkedList* result = NULL;

    switch (current.type) {
    case OPENING_SCOPE_TOKEN:
        result = parse_scope(parse);
        break;
    case IF_TOKEN:
        result = parse_if_statement(parse);
        break;
    case WHILE_TOKEN:
        ParsingNode* head = parse_conditional_node(parse);
        if (head != NULL) {
            result = ParsingNodeLinkedList_new();
            ParsingNodeLinkedList_add(result, head);
        }
        break;

    default:
        ParsingNode* instruction = parse_instruction(parse);
        if (instruction != NULL) {
            result = ParsingNodeLinkedList_new();
            ParsingNodeLinkedList_add(result, instruction);
        }
    }
    return result;
}

ParsingNode* parse_conditional_node(parser* parser) {
    Token current_token = Parser_get_current_token(parser);
    int type = ParsingNodeType_from_conditional_token(current_token);
    if (type == NONE_NODE) {
        return NULL;
    }
    ParsingNode* result = ParsingNode_new_from_type(type);
    Parser_advance(parser);
    if (type != ELSE_NODE) {
        result->condition = parse_logical_or(parser);
        RETURN_NULL_IF_ERROR(result->condition, 1, result);
    }
    ParsingNodeLinkedList* true_branch = parse_statement(parser);
    if (true_branch == NULL) {
        ParsingNode_free_recursive(result, 0);
        return NULL;
    }
    result->true_condition = true_branch->head;
    free(true_branch);
    return result;
}

ParsingNodeLinkedList* parse_if_statement(parser* parse) {
    Token current_token;
    ParsingNodeLinkedList* result = ParsingNodeLinkedList_new();

    ParsingNode* if_statement = parse_conditional_node(parse);

    if (if_statement == NULL) {
        ParsingNodeLinkedList_free(result);
        return NULL;
    }

    ParsingNodeLinkedList_add(result, if_statement);

    while ((current_token = Parser_get_current_token(parse)).type == ELIF_TOKEN) {
        ParsingNode* elif_statement = parse_conditional_node(parse);
        if (elif_statement == NULL) {
            ParsingNodeLinkedList_free(result);
            return NULL;
        }
        ParsingNodeLinkedList_add(result, elif_statement);
    }
    if ((current_token = Parser_get_current_token(parse)).type == ELSE_TOKEN) {
        ParsingNode* else_statement = parse_conditional_node(parse);
        if (else_statement == NULL) {
            ParsingNodeLinkedList_free(result);
            return NULL;
        }
        ParsingNodeLinkedList_add(result, else_statement);
    }
    return result;
}

ParsingNode* parse_instruction(parser* parser) {
    ParsingNode* res = NULL;
    Token current_token = Parser_get_current_token(parser);
    if (current_token.type == IDENTIFIER_TOKEN) {
        res = (Parser_get_next_token(parser).type == AFFECTATION_TOKEN)
                  ? parse_affectation(parser)
                  : parse_expression(parser);
    }
    else {
        res = parse_expression(parser);
    }
    RETURN_NULL_IF_ERROR(res, 0)
    current_token = Parser_get_current_token(parser);
    if (current_token.type != DELIMITER_TOKEN) {
        add_parsing_error_to_parser(parser, current_token, "(INSTRUCTION) expected ';'");
        ParsingNode_free_recursive(res, false);
        res = NULL;
    }
    Parser_advance(parser);
    return res;
}

ParsingNode* parse_affectation(parser* parse) {
    ParsingNode* left = parse_identifier(parse);
    ParsingNode* result = ParsingNode_new();
    Parser_advance(parse);
    ParsingNode* right = parse_expression(parse);

    result->type = AFFECTATION_NODE;
    result->left = left;
    result->right = right;
    return result;
}

ParsingNode* parse_expression(parser* parse) {
    return parse_logical_or(parse);
}

ParsingNode* parse_identifier(parser* parse) {
    ParsingNode* res = ParsingNode_new();
    Token current = Parser_get_current_token(parse);
    res->type = VARIABLE_NODE;
    size_t len = strlen(current.string_val);
    res->name = malloc(sizeof(char) * len + 1);
    memcpy(res->name, current.string_val, len + 1);
    Parser_advance(parse);
    return res;
}

ParsingNode* parse_comparison(parser* parse) {
    ParsingNode* left = parse_additive(parse);
    RETURN_NULL_IF_ERROR(left, 0)
    ParsingNode* center = NULL;
    Token current = Parser_get_current_token(parse);
    if (current.type == OPERATOR_TOKEN && Operator_is_comparison(current.operation)) {
        center = ParsingNode_new_from_operator_token(current);
        RETURN_NULL_IF_ERROR(center, 1, left);
        center->left = left;
        Parser_advance(parse);
        center->right = parse_additive(parse);
        RETURN_NULL_IF_ERROR(center->right, 2, left, center)
    }
    else {
        center = left;
    }

    return center;
}

ParsingNode* parse_unary(parser* parse) {
    Token current = Parser_get_current_token(parse);
    if (!Token_is_unary(current)) {
        return parse_primary(parse);
    }

    Operator unary = Parser_get_current_token(parse).operation;
    bool apply = false;
    while (Token_is_unary(current) && current.operation == unary) {
        apply = !apply;
        Parser_advance(parse);
        current = Parser_get_current_token(parse);
    }
    ParsingNode* result = NULL;
    if (apply) {
        result = ParsingNode_new_from_type(UNARY_NODE);
        result->operation = unary;
        result->right = parse_primary(parse);;
    }
    else {
        result = parse_primary(parse);
    }
    return result;
}

ParsingNode* parse_primary(parser* parse) {
    Token current = Parser_get_current_token(parse);
    ParsingNode* result;
    if (Token_is_num(current)) {
        result = ParsingNode_new_from_num_token(current);
        Parser_advance(parse);
    }
    else if (Token_is_unary(current)) {
        result = parse_unary(parse);
    }
    else if (current.type == IDENTIFIER_TOKEN) {
        result = parse_identifier(parse);
    }
    else if (current.type == OPENING_PARENTHESE_TOKEN) {
        Parser_advance(parse);
        result = parse_expression(parse);

        RETURN_NULL_IF_ERROR(result, 1, result)
        Token t = Parser_get_current_token(parse);
        if (t.type != CLOSING_PARENTHESE_TOKEN) {
            parse->parsing_status = PARSING_ERROR;
            add_parsing_error_to_parser(parse, t, "(PRIMARY) expected ')'");

            ParsingNode_free_recursive(result, false);
            return NULL;
        }
        RETURN_NULL_IF_ERROR(result, 0)
        Parser_advance(parse);
    }
    else {
        add_parsing_error_to_parser(parse, current,
                                    "(PRIMARY) expected a number, an identifier, an unary, or  an opening parenthese");
        return NULL;
    }
    RETURN_NULL_IF_ERROR(result, 0)
    return result;
}

void add_parsing_error_to_parser(parser* parse, Token t, char* message) {
    parse->parsing_status = PARSING_ERROR;
    ParsingError error = {.token = t};
    snprintf(error.message, sizeof(error.message), message);
    ParsingErrorArrayList_add(parse->parsing_errors, error);
}
