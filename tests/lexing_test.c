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
    token_array_list * list = lex_code(code);
    printf("result = \n");
    print_token_list(list);
}