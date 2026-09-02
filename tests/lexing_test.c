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
        printf("Error : can't find file %s.\n", path);
        return 1;
    }


    printf("path= \"\"\"%s\"\"\"\n", path);

    char buffer[1024];


    lexer *lexer = new_lexer();

    lex_code_from_file(lexer, buffer, sizeof(buffer), file);

    token_array_list * list = lexer->tokens_list;

    printf("result = \n");
    print_token_list(list);
}