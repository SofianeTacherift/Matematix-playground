#include "lexer.h"
#include <stdio.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("lexing_test <file>\n");
        return 1;
    }

    char* path = argv[1];
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("open file");
        return 1;
    }

    printf("path= \"\"\"%s\"\"\"\n", path);

    LexingResult lexing_result = lex_file(file);

    TokenArrayList* list = lexing_result.tokens;

    printf("result : \n");
    TokenArrayList_println(list, Token_print);

    LexingResult_deinit(lexing_result);
}
