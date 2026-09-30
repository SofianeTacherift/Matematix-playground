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

    lexing_result lexing_result = lex_code_from_file(  file);

    token_array_list * list = lexing_result.tokens;

    printf("result = \n");
    print_token_list(list);

    free_lexing_result_members(lexing_result);



}