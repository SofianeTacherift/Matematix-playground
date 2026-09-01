
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"
#include "operators.h"


#define NL_UPDATE_LEXER(LEXER, C) if ( C =='\n') {LEXER->current_line++; LEXER->current_char=0; }  
#define LEXER_ADV_CHAR(LEXER) LEXER->current_char++;

#define LEXER_ADV_UPDATE(LEXER , C) LEXER_ADV_CHAR(LEXER) NL_UPDATE_LEXER( LEXER, C)

// return true if a substring who start with c is automaticly a new token


lexer * new_lexer() {
    lexer *res = malloc(sizeof(lexer));
    if (res!=NULL) {
        res->current_char=0;
        res->current_line=0;
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

int lex_number(lexer *lexe, char *code, int start, int str_end) {
    int i=start;
    int line=lexe->current_line;
    int character=lexe->current_char;
    char * number =malloc(sizeof(char)*30);
    int type=INT_TOKEN;
    while (i<str_end && !start_new_token(code[i]) &&isdigit(code[i]) || code[i]=='.') {
        int current_index=i-start;
        char charI=code[i];
        if (charI=='.') {
            if (type==DOUBLE_TOKEN) {
                lexe->lexing_status=LEXING_ERROR;
                write_in_lexing_error_buffer(lexe, "invalid number\n");
                return -1;
            }
            type=DOUBLE_TOKEN;
        } 
        number[current_index]=code[i];
        LEXER_ADV_UPDATE(lexe, charI)
        i++;
    }

    
    int end_index=i-start;
    number[end_index]='\0';
    token t;
    t.line=line;
    t.character=character;
    t.operation=NONE_OPERATOR;

    if ((i<str_end && code[i]=='f')) {
        type=FLOAT_TOKEN;
        LEXER_ADV_UPDATE(lexe, code[i]);
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

    add_token(lexe->tokens_list, t);
    return i;

}

int lex_string(lexer *lexe, char *code, int start, int str_end) {
    int i=start;
    int current_len = 32;
    char * str =malloc(sizeof(char)*current_len);
    int line=lexe->current_line;
    int character=lexe->current_char;

    while (i<str_end && !start_new_token(code[i])) {
        int current_index=i-start;
        char charI=code[i];
        if (current_index>current_len) {
            str=realloc(str, current_len*2*sizeof(char));
            current_len*=2;
        }
        str[current_index]=code[i];

        LEXER_ADV_UPDATE(lexe, charI)
        i++;

    }
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
    add_token(lexe->tokens_list, t);
    t.line=line;
    t.character=character;
    return i;
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



char next_non_space_index(char * str, int i, int end) {
    for (int j=i; j<end;j++) {
        if (str[j]!=' ') {
            return j;
        }
    }
    return -1;
}





int lex_minus(lexer *lexe, char *code, int i, int str_end) {
    int non_space_index=next_non_space_index(code, i+1, str_end);
    int line=lexe->current_line;
    int character = lexe->current_char;
    LEXER_ADV_UPDATE(lexe, code[i]);

    token_array_list *tokens_list = lexe->tokens_list;

    if (tokens_list->size==0 || is_unary_minus(tokens_list->elements[tokens_list->size-1])) {
        char next_char = code[non_space_index];
            add_token(tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=UNARY_MINUS_OPERATOR , .line=line, .character=character});
            return i+1;
          
    }
    add_token(tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=SUB_OPERATOR, .line=line, .character=character});
    return i+1;
}
int lex_arithmetic_operator(lexer *lexe, char *code, int i, int str_end) {
    char parsing_operator=code[i];
    int line=lexe->current_line;
    int character = lexe->current_char;
    if (parsing_operator!='-') {
        LEXER_ADV_UPDATE(lexe, code[i]);
        token result=operator_to_token(parsing_operator);
        result.character=character;
        result.line=line;
        add_token(lexe->tokens_list, result);
        return i+1;
    }
    else if (parsing_operator=='-') {
        return lex_minus(lexe, code, i, str_end);
    }


}


int lex_opening_parenthese(lexer *lexe, char *code, int i, int str_end) {
    add_token(lexe->tokens_list, (token) {.type=OPENING_PARENTHESE_TOKEN , .line=lexe->current_line, .character=lexe->current_char});
    LEXER_ADV_UPDATE(lexe, code[i]);
    return i+1;
}

int lex_closing_parenthese(lexer *lexe, char *code, int i, int str_end) {
    add_token(lexe->tokens_list, (token) {.type=CLOSING_PARENTHESE_TOKEN, .character=lexe->current_char, .line=lexe->current_line});
    LEXER_ADV_UPDATE(lexe, code[i]);
    return i+1;
}

int lex_comparison(lexer *lexe, char *code, int i, int str_end) {
    if (i<str_end && code[i+1]=='=') {
        switch (code[i]) {
        case '=':
            add_token(lexe->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=EQUALS_OPERATOR   , .character=lexe->current_char, .line=lexe->current_line});
            break;
        case '>':
            add_token(lexe->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=GREATER_OR_EQUAL_OPERATOR   , .character=lexe->current_char, .line=lexe->current_line});
            break;
        case '<':
            add_token(lexe->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=LESS_OR_EQUAL_OPERATOR , .character=lexe->current_char, .line=lexe->current_line});
            break;
        default:
            break;
        }
        lexe->current_char+=2;
        return i+2;
    }
    else {
        switch (code[i]) {
        case '=':
            add_token(lexe->tokens_list, (token) {.type=AFFECTATION_TOKEN, .character=lexe->current_char, .line=lexe->current_line});
            break;
        case '>':
            add_token(lexe->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=GREATER_THAN_OPERATOR   , .character=lexe->current_char, .line=lexe->current_line});
            break;
        case '<':
            add_token(lexe->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=LESS_THAN_OPERATOR  , .character=lexe->current_char, .line=lexe->current_line});
            break;
        default:
            break;
        }
        lexe->current_char++;
        return i+1;       
    }
}



token_array_list * lex_code(char * code) {
    lexer *lexe = new_lexer();

    int i=0;
    char charI = code[i];
    int end=strlen(code);
    // printf("end=%d\n", end);
    while (i<end) {
        charI=code[i];

        switch (charI) {
            case '=':
            case '<':
            case '>':
                i=lex_comparison(lexe, code, i, end);
                break;
            case '&':
            add_token(lexe->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=LOGICAL_AND_OPERATOR  , .character=lexe->current_char, .line=lexe->current_line});
                lexe->current_char++;  
                i++;
                break;
            case '|':
            add_token(lexe->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=LOGICAL_OR_OPERATOR   , .character=lexe->current_char, .line=lexe->current_line});
                lexe->current_char++;  
                i++;
                break;
            case '!':
                add_token(lexe->tokens_list, (token) {.type=OPERATOR_TOKEN, .operation=NOT_OPERATOR   , .character=lexe->current_char, .line=lexe->current_line});
                lexe->current_char++;  
                i++;
                break;
            case DELIMITATION:
                add_token(lexe->tokens_list, (token) {.type=DELIMITER_TOKEN ,.line=lexe->current_line, .character=lexe->current_char  });
                LEXER_ADV_UPDATE(lexe, charI)
                i++;
                break;
            case '(':
                i=lex_opening_parenthese(lexe, code, i, end);
                break;
            case ')':
                i=lex_closing_parenthese(lexe, code, i, end);
                break;
            case ' ':
            case '\n':
                i++;
                LEXER_ADV_UPDATE(lexe, charI);
                break;
            case '{':
                i++;
                add_token(lexe->tokens_list, (token){.type=OPENING_SCOPE_TOKEN, .line=lexe->current_line, .character=lexe->current_char});
                LEXER_ADV_UPDATE(lexe, charI);
                break;
            case '}':
                i++;
                add_token(lexe->tokens_list, (token){.type=CLOSING_SCOPE_TOKEN, .line=lexe->current_line, .character=lexe->current_char});
                LEXER_ADV_UPDATE(lexe, charI);
                break;
            
            default:
                if (isalpha(charI)) {
                i=lex_string(lexe, code, i, end);
                }
                else if (isdigit(charI)) {
                    i=lex_number(lexe, code, i, end);
                }
                else if (is_arithmetic_operator(charI)) {
                    i=lex_arithmetic_operator(lexe, code, i, end);
                }
                else {
                    lexe->lexing_status=LEXING_ERROR;
                    char buffer[100];
                    snprintf(buffer, sizeof(buffer),"Character %c is not valid", charI );
                    write_in_lexing_error_buffer(lexe, buffer);
                }
                break;
        }
        if (lexe->lexing_status==LEXING_ERROR) {
            fwrite(lexe->error_buffer, sizeof(char), 1024, stderr);
            return NULL;
        }



    }
    printf("\ncode lexed\n");
    add_token(lexe->tokens_list, (token) {.type=EOF_TOKEN, .line=lexe->current_line, .character=lexe->current_char});
    return lexe->tokens_list;


}


void write_in_lexing_error_buffer(lexer *lexe, char *message) {
    snprintf(lexe->error_buffer, sizeof(lexe->error_buffer),"Error during lexing at line %d character %d : %s.\n",lexe->current_line+1, lexe->current_char+1, message );
}