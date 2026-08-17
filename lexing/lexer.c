
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"


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

bool is_new_token(char c) {
    switch (c)
    {
    case '*':
    case '+':
    case '/':
    case '-':
    case '=':
    case ' ':
    case '(':
    case ')':
    case DELIMITATION:
    case '<':
    case '>':
    case '&':
    case '|':
    case '{':
    case '}':
        return true;
    default:
        return false;
    }
}

int lexe_number(lexer *lexe, char *code, int start, int str_end) {
    int i=start;
    int line=lexe->current_line;
    int character=lexe->current_char;
    char * number =malloc(sizeof(char)*30);
    int type=INT;
    while (i<str_end && !is_new_token(code[i]) &&isdigit(code[i]) || code[i]=='.') {
        int current_index=i-start;
        char charI=code[i];
        if (charI=='.') {
            if (type==DOUBLE) {
                lexe->lexing_status=LEXING_ERROR;
                write_in_lexing_error_buffer(lexe, "invalid number\n");
                return -1;
            }
            type=DOUBLE;
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

    if ((i<str_end && code[i]=='f')) {
        type=FLOAT;
        LEXER_ADV_UPDATE(lexe, code[i]);
        i++;
    }
    t.type=type;
    switch (type)
    {
    case INT:
        t.int_val=atoi(number);
        break;
    case FLOAT:
        t.float_val=atof(number);
        break;
    case DOUBLE:
        t.double_val=atof(number);
        break;
    }

    add_token(lexe->tokens_list, t);
    return i;

}

int lexe_string(lexer *lexe, char *code, int start, int str_end) {
    int i=start;
    int current_len = 32;
    char * str =malloc(sizeof(char)*current_len);
    int line=lexe->current_line;
    int character=lexe->current_char;

    while (i<str_end && !is_new_token(code[i])) {
        int current_index=i-start;
        char charI=code[i];

        // printf("i=%d - current_index=%d - charI=%c\n", i, current_index, charI);
        if (current_index>current_len) {
            str=realloc(str, current_len*2*sizeof(char));
            current_len*=2;
        }
        str[current_index]=code[i];

        LEXER_ADV_UPDATE(lexe, charI)
        i++;

    }
    str[i-start]='\0';
    token t;
    if (strcmp("if", str)==0) {
        t.type=IF; t.line=line, t.character=character;
    }
    else if (strcmp("elif", str)==0) {
        t.type=ELIF, t.line=line, t.character=character;
    }

    else if (strcmp("else", str)==0) {
        t.type=ELSE, t.line=line, t.character=character;
    }

    else {
        t.string_val=str; t.type=IDENTIFIER ; t.line=line; t.character=character;
    }
    add_token(lexe->tokens_list, t);
    t.line=line;
    t.character=character;
    return i;
}

bool is_operator(char c) {
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
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





int lexe_minus(lexer *lexe, char *code, int i, int str_end) {
    int non_space_index=next_non_space_index(code, i+1, str_end);
    int line=lexe->current_line;
    int character = lexe->current_char;
    LEXER_ADV_UPDATE(lexe, code[i]);

    token_array_list *tokens_list = lexe->tokens_list;

    if (tokens_list->size==0 || is_unary_minus(tokens_list->elements[tokens_list->size-1])) {
        char next_char = code[non_space_index];
            add_token(tokens_list, (token) {.type=UNARY_MINUS, .line=line, .character=character});
            return i+1;
          
    }
    add_token(tokens_list, (token) {.type=SUB, .line=line, .character=character});
    return i+1;
}
int lexe_operator(lexer *lexe, char *code, int i, int str_end) {
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
        return lexe_minus(lexe, code, i, str_end);
    }


}


int lexe_opening_parenthese(lexer *lexe, char *code, int i, int str_end) {
    add_token(lexe->tokens_list, (token) {.type=OPENING_PARENTHESE , .line=lexe->current_line, .character=lexe->current_char});
    LEXER_ADV_UPDATE(lexe, code[i]);
    return i+1;
}

int lexe_closing_parenthese(lexer *lexe, char *code, int i, int str_end) {
    add_token(lexe->tokens_list, (token) {.type=CLOSING_PARENTHESE, .character=lexe->current_char, .line=lexe->current_line});
    LEXER_ADV_UPDATE(lexe, code[i]);
    return i+1;
}

int lexe_comparaison(lexer *lexe, char *code, int i, int str_end) {
    if (i<str_end && code[i+1]=='=') {
        switch (code[i]) {
        case '=':
            add_token(lexe->tokens_list, (token) {.type=EQUALS, .character=lexe->current_char, .line=lexe->current_line});
            break;
        case '>':
            add_token(lexe->tokens_list, (token) {.type=GREATER_OR_EQUAL, .character=lexe->current_char, .line=lexe->current_line});
            break;
        case '<':
            add_token(lexe->tokens_list, (token) {.type=LESS_OR_EQUAL, .character=lexe->current_char, .line=lexe->current_line}); 
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
            add_token(lexe->tokens_list, (token) {.type=AFFECTATION, .character=lexe->current_char, .line=lexe->current_line});
            break;
        case '>':
            add_token(lexe->tokens_list, (token) {.type=GREATER_THAN, .character=lexe->current_char, .line=lexe->current_line});
            break;
        case '<':
            add_token(lexe->tokens_list, (token) {.type=LESS_THAN, .character=lexe->current_char, .line=lexe->current_line}); 
            break;
        default:
            break;
        }
        lexe->current_char++;
        return i+1;       
    }
}



token_array_list * lexe_code(char * code) {
    lexer *lexe = new_lexer();

    int i=0;
    char charI = code[i];
    int end=strlen(code);
    // printf("end=%d\n", end);
    while (i<end) {
        charI=code[i];
        // printf("i=%d - charI='%c' - list=\n", i, charI);
        // print_token_list(tokens_list);

        switch (charI) {
            case '=':
            case '<':
            case '>':
                i=lexe_comparaison(lexe, code, i, end);
                break;
            case '&':
                add_token(lexe->tokens_list, (token) {.type=AND ,.line=lexe->current_line, .character=lexe->current_char  }); 
                lexe->current_char++;  
                i++;
                break;
            case '|':
                add_token(lexe->tokens_list, (token) {.type=OR ,.line=lexe->current_line, .character=lexe->current_char  }); 
                lexe->current_char++;  
                i++;
                break;
            case DELIMITATION:
                add_token(lexe->tokens_list, (token) {.type=DELIMITER ,.line=lexe->current_line, .character=lexe->current_char  });
                LEXER_ADV_UPDATE(lexe, charI)
                i++;
                break;
            case '(':
                i=lexe_opening_parenthese(lexe, code, i, end);
                break;
            case ')':
                i=lexe_closing_parenthese(lexe, code, i, end);
                break;
            case ' ':
                i++;
                LEXER_ADV_UPDATE(lexe, charI);
                break;
            case '{':
                i++;
                add_token(lexe->tokens_list, (token){.type=OPENING_SCOPE, .line=lexe->current_line, .character=lexe->current_char});
                LEXER_ADV_UPDATE(lexe, charI);
                break;
            case '}':
                i++;
                add_token(lexe->tokens_list, (token){.type=CLOSING_SCOPE, .line=lexe->current_line, .character=lexe->current_char});
                LEXER_ADV_UPDATE(lexe, charI);
                break;
            
            default:
                if (isalpha(charI)) {
                i=lexe_string(lexe, code, i, end);
                }
                else if (isdigit(charI)) {
                    i=lexe_number(lexe, code, i, end);
                }
                else if (is_operator(charI)) {
                    i=lexe_operator(lexe, code, i, end);
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
    add_token(lexe->tokens_list, (token) {.type=END, .line=lexe->current_line, .character=lexe->current_char});


    return lexe->tokens_list;


}


void write_in_lexing_error_buffer(lexer *lexe, char *message) {
    snprintf(lexe->error_buffer, sizeof(lexe->error_buffer),"Error during lexing at line %d character %d : %s.\n",lexe->current_line+1, lexe->current_char+1, message );
}