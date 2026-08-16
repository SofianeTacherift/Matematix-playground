#include "lexer.h"
#include <stdio.h>



int main(int argc, char **argv) {
    char * code;
    if (argc<2) {
        code="a=a";
    }
    else {
        code=argv[1];
    }
    printf("code= \"\"\"%s\"\"\"\n", code);
    printf("result = \n");
    token_array_list * list = lexe_code(code);
    print_token_list(list);
}