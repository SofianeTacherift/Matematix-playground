#include "lexer.h"
#include <stdio.h>



int main(int argc, char **argv) {
    if (argc<2) {
        printf("lexing_test <file>\n");
        return 1;
    }

    char *path = argv[1];
    FILE *file = fopen(path, "r");
    if (file==NULL) {
        perror("open file");
        return 1;
    }

    printf("path= \"\"\"%s\"\"\"\n", path);

    lexing_result lexing_result = lex_code_from_file( 1024 , file);

    token_array_list * list = lexing_result.tokens;

    printf("result = \n");
    print_token_list(list);
    apply_token_operation(list, free_token_elements);
    free_token_array_list(list);



}