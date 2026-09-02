
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"
#include "operators.h"




// return true if a substring who start with c is automaticly a new token


lexer * new_lexer() {
    lexer *res = calloc(1,sizeof(lexer));
    if (res!=NULL) {
        res->tokens_list=new_token_array_list();
        res->lexing_status=NOT_USED_RES_I;
    }
    return res;
}

bool start_new_token(char c) {
    switch (c)
    {
    case '*':
    case '+':
    case '/':
    case '-':
    case '=':
    case ' ':
    case '^':
    case '(':
    case ')':
    case DELIMITATION:
    case '<':
    case '>':
    case '&':
    case '|':
    case '{':
    case '}':
    case '!':
        return true;
    default:
        return false;
    }
}

void lex_number(lexer *lexe) {
    char * code = lexe->code_buffer;
    int start = lexe->reading_index;
    int i=start;
    int line=lexe->current_line;
    int character=lexe->current_char;
    char * number =malloc(sizeof(char)*30);
    int type=INT_TOKEN;
    while (i<lexe->buffer_end && isdigit(code[i]) || code[i]=='.') {
        int current_index=i-start;
        char charI=code[i];
        if (charI=='.') {
            if (type==DOUBLE_TOKEN) {
                lexe->lexing_status=LEXING_ERROR;
                write_in_lexing_error_buffer(lexe, "invalid number\n");
            }
            type=DOUBLE_TOKEN;
        } 
        number[current_index]=code[i];
        i++;
    }

    int number_end_index=i-start;
    number[number_end_index]='\0';
    token t;
    t.line=line;
    t.character=character;
    t.operation=NONE_OPERATOR;

    if ((i<lexe->buffer_end && code[i]=='f')) {
        type=FLOAT_TOKEN;
        i++;
    }
    t.type=type;
    switch (type)
    {
    case INT_TOKEN:
        t.int_val=atoi(number);
        break;
    case FLOAT_TOKEN:
        t.float_val=atof(number);
        break;
    case DOUBLE_TOKEN:
        t.double_val=atof(number);
        break;
    }
    advance_n(lexe, i-start);
    add_token(lexe->tokens_list, t);

}

void lex_string(lexer *lexer) {
    char *code =lexer->code_buffer;
    int start = lexer->reading_index;
    int str_end = lexer->buffer_end;

    int i=start;
    int current_len = 32;
    char * str =malloc(sizeof(char)*current_len);
    int line=lexer->current_line;
    int character=lexer->current_char;

    while (i<str_end && !start_new_token(code[i])) {
        int current_index=i-start;
        if (current_index>current_len) {
            str=realloc(str, current_len*2*sizeof(char));
            current_len*=2;
        }
        str[current_index]=code[i];

        i++;

    }
    advance_n(lexer, i-start);
    str[i-start]='\0';
    token t= {0};
    if (strcmp("while", str)==0) {
        t.type=WHILE_TOKEN; t.line=line, t.character=character;
    }
    else if (strcmp("if", str)==0) {
        t.type=IF_TOKEN; t.line=line, t.character=character;
    }
    else if (strcmp("elif", str)==0) {
        t.type=ELIF_TOKEN, t.line=line, t.character=character;
    }

    else if (strcmp("else", str)==0) {
        t.type=ELSE_TOKEN, t.line=line, t.character=character;
    }

    else {
        t.string_val=str; t.type=IDENTIFIER_TOKEN ; t.line=line; t.character=character;
    }
    t.operation=NONE_OPERATOR;
    add_token(lexer->tokens_list, t);
    t.line=line;
    t.character=character;
}

bool is_arithmetic_operator(char c) {
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            return true;
        default:
            return false;
    }
}



char next_non_space_index(const char * str, const int i, const int end) {
    for (int j=i; j<end;j++) {
        if (str[j]!=' ') {
            return j;
        }
    }
    return -1;
}


token minus_to_token(lexer *lexer) {


    int line=lexer->current_line;
    int character = lexer->current_char;


    token_array_list *tokens_list = lexer->tokens_list;

    if (tokens_list->size==0 || involve_unary_minus(tokens_list->elements[tokens_list->size-1])) {
        return tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=UNARY_MINUS_OPERATOR , .line=line, .character=character};
    }
    return (token) {.type=OPERATOR_TOKEN, .operation=SUB_OPERATOR, .line=line, .character=character};

}
int lex_operator(lexer *lexer) {
    char *code = lexer->code_buffer;
    int start = lexer->reading_index;
    int line=lexer->current_line;
    int character = lexer->current_char;
    token result;
    char parsing_operator=code[start];
    if (parsing_operator!='-') {
        result=binary_operator_to_token(parsing_operator);
    }
    else {
        result=minus_to_token(lexer);
    }
    result.character=character;
    result.line=line;
    add_token(lexer->tokens_list, result);
    advance_check_ln(lexer);
    return start+1;
}



int lex_comparison(lexer *lexer) {

    char *code = lexer->code_buffer;
    int start = lexer->reading_index;
    int end = lexer->buffer_end;


    if (start<end-1 && code[start+1]=='=') {
        switch (code[start]) {
        case '=':
            add_token(lexer->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=EQUALS_OPERATOR   , .character=lexer->current_char, .line=lexer->current_line});
            break;
        case '>':
            add_token(lexer->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=GREATER_OR_EQUAL_OPERATOR   , .character=lexer->current_char, .line=lexer->current_line});
            break;
        case '<':
            add_token(lexer->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=LESS_OR_EQUAL_OPERATOR , .character=lexer->current_char, .line=lexer->current_line});
            break;
        default:
            break;
        }
        advance_n(lexer, 2);
        return 0;
    }


    switch (code[start]) {
    case '=':
        add_token(lexer->tokens_list, (token) {.type=AFFECTATION_TOKEN, .character=lexer->current_char, .line=lexer->current_line});
        break;
    case '>':
        add_token(lexer->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=GREATER_THAN_OPERATOR   , .character=lexer->current_char, .line=lexer->current_line});
        break;
    case '<':
        add_token(lexer->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=LESS_THAN_OPERATOR  , .character=lexer->current_char, .line=lexer->current_line});
        break;
    default:
        break;
    }
    advance_n(lexer, 1);
    return 0;

}



void advance_check_ln(lexer *lexer) {
    char current_char = lexer->code_buffer[lexer->reading_index];
    lexer->reading_index++;
    lexer->current_char++;
    lexer->total_bytes_read++;

    if (current_char=='\n') {
        lexer->current_line++;
        lexer->current_char=0;
    }

}

void advance_n(lexer *lexer, int n) {
    lexer->reading_index+=n;
    lexer->current_char+=n;
    lexer->total_bytes_read+=n;
}



void lex_code(lexer *lexer) {

    char * code = lexer->code_buffer;

    while (lexer->reading_index<lexer->buffer_end) {
        char charI=code[lexer->reading_index];
        int res=0;
        switch (charI) {
            case '=':
            case '<':
            case '>':
                lex_comparison(lexer);
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '&':
            case '|':
                lex_operator(lexer);
                break;
            case '!':
                add_token(lexer->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=LOGICAL_NOT_OPERATOR   , .character=lexer->current_char, .line=lexer->current_line});
                advance_check_ln(lexer);
                break;
            case DELIMITATION:
                add_token(lexer->tokens_list, (token) {.type=DELIMITER_TOKEN ,.line=lexer->current_line, .character=lexer->current_char  });
                advance_check_ln(lexer);
                break;
            case '(':
                add_token(lexer->tokens_list, (token) {.type=OPENING_PARENTHESE_TOKEN  , .character=lexer->current_char, .line=lexer->current_line});
                advance_check_ln(lexer);
                break;
            case ')':
                add_token(lexer->tokens_list, (token) {.type=CLOSING_PARENTHESE_TOKEN   , .character=lexer->current_char, .line=lexer->current_line});
                advance_check_ln(lexer);
                break;
            case '{':
                res++;
                add_token(lexer->tokens_list, (token){.type=OPENING_SCOPE_TOKEN, .line=lexer->current_line, .character=lexer->current_char});
                advance_check_ln(lexer);
                break;
            case '}':
                res++;
                add_token(lexer->tokens_list, (token){.type=CLOSING_SCOPE_TOKEN, .line=lexer->current_line, .character=lexer->current_char});
                advance_check_ln(lexer);
                break;
            case ' ':
            case '\n':
                res=0;;
                advance_check_ln(lexer);
                break;
            default:
                if (isalpha(charI)) {
                lex_string(lexer);
                }
                else if (isdigit(charI)) {
                    lex_number(lexer);
                }
                else {
                    lexer->lexing_status=LEXING_ERROR;
                    char buffer[100];
                    snprintf(buffer, sizeof(buffer),"Character %c is not valid", charI );
                    write_in_lexing_error_buffer(lexer, buffer);
                }
                break;
        }
        if (lexer->lexing_status==LEXING_ERROR) {
            fwrite(lexer->error_buffer, sizeof(char), 1024, stderr);
            return ;
        }



    }
    printf("\ncode lexed\n");
    add_token(lexer->tokens_list, (token) {.type=EOF_TOKEN, .line=lexer->current_line, .character=lexer->current_char});


}


void write_in_lexing_error_buffer(lexer *lexe, char *message) {
    snprintf(lexe->error_buffer, sizeof(lexe->error_buffer),"Error during lexing at line %d character %d : %s.\n",lexe->current_line+1, lexe->current_char+1, message );
}