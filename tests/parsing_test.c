#include "parser.h"
#include "lexer.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("parsing_test <file>\n");
        return 1;
    }

    char* path = argv[1];

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Error : can't find file '%s'.\n", path);
        return 1;
    }

    printf("path= \"\"\"%s\"\"\"\n", path);

    char buffer[1024];

    ParsingResult parsing_res = parse_from_file(file);
    LexingResult lexing_res = parsing_res.lexing_res;
    ParsingNode* res = parsing_res.head;
    if (lexing_res.lexing_status == LEXING_ERROR) {
        fprintf(stderr, "%s\n", lexing_res.error_message);
        return 1;
    }

    printf("parsing result :\n");

    if (parsing_res.parsing_status == NO_PARSING_ERROR) {
        ParsingNode_print_recursive(res);

        printf("\n\n");

        printf("parsing result (readable) :\n");

        ParsingNode_print_readable_recursive(res, 0);

        printf("\n");
    }
    else {
        for (int i = 0; i < parsing_res.errors->size; i++) {
            ParsingError error = parsing_res.errors->elements[i];
            printf("error during parsing line %d character %d : %s \n", error.token.line + 1, error.token.character + 1,
                   error.message);
        }
    }

    ParsingResult_deinit(parsing_res);

    return 0;
}
