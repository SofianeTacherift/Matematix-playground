#include "lexer.h"
#include <stdio.h>



int main(int argc, char **argv) {
    char * code;
    if (argc<2) {
        printf("lexing_test <code>\n");
        return 1;
    }
    else {
        code=argv[1];
    }
    printf("code= \"\"\"%s\"\"\"\n", code);
    lexer *lexer = new_lexer();
    lexer->code_buffer=code;
    lexer->buffer_end=strlen(code);

    lex_code(lexer);

    token_array_list * list = lexer->tokens_list;

    printf("result = \n");
    print_token_list(list);
}