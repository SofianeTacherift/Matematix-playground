#include "parser.h"
#include "lexer.h"


int main(int argc, char ** argv) {
    if (argc<2) {
        printf("parsing_test <file>\n");
        return 1;
    }

    // char *path = argv[1];
    char *file_name = argv[1];

    char *dir = "/home/sofiane/Documents/PROJETS/PROGRAMMING_LANGUAGE/ressources/mxp_files/";
    char path[strlen(file_name)+strlen(dir)+2];

    memcpy(path+2, file_name, strlen(file_name));

    strcpy(path, dir);
    strcat(path, file_name);


    FILE *file = fopen(path, "r");
    if (file==NULL) {
        printf("Error : can't find file '%s'.\n", path);
        return 1;
    }


    printf("path= \"\"\"%s\"\"\"\n", path);

    char buffer[1024];


    parsing_result parsing_res = parse_from_file(file);
    lexing_result lexing_res = parsing_res.lexing_res;
    parsing_node *res = parsing_res.head;
    if (lexing_res.lexing_status==LEXING_ERROR) {
        fprintf(stderr, "%s\n",lexing_res.error_buffer);
        return 1;
    }


    printf("parsing result :\n");


    if (parsing_res.parsing_status==NO_PARSING_ERROR) {

        display_tree_node(res);

        printf("\n\n");

        printf("parsing result (readable) :\n");

        display_tree_node_readable(res, 0);



        printf("\n");

    }
    else {
        for (int i=0; i<parsing_res.errors->size; i++) {
            parsing_error error = parsing_res.errors->elements[i];
            printf("error during parsing line %d character %d : %s \n", error.token.line+1, error.token.character+1, error.message);
        }
    }



    free_tree_node(res, true);
    free_token_array_list(parsing_res.lexing_res.tokens);


    return 0;
    
}