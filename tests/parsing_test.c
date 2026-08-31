#include "parser.h"
#include "lexer.h"


int main(int argc, char ** argv) {
    if (argc<2) {
        printf("parsing_test <code>\n");
        return 1;
    }


    char * code = argv[1];
    printf("code = '''%s'''", code);
    token_array_list *list = lex_code(code);

    printf("\ntokens list : ");
    print_token_list(list);
    printf("\n");

    if (list==NULL) {
        printf("error during lexing, exit\n");
        return 2;
    }


 



    parser *parse= new_parser(list);

    parsing_node * res = parse_main_scope(parse);




    printf("parsing result :\n");

    if (parse->parsing_status==NO_PARSING_ERROR) {
        display_tree_node(res);

        printf("\n\n");

        printf("parsing result (readable) :\n");

        display_tree_node_readable(res, 0);



        printf("\n");

    }
    else {
        for (int i=0; i<parse->parsing_errors->size; i++) {
            parsing_error error = parse->parsing_errors->elements[i];
            printf("error during parsing line %d character %d : %s \n", error.token.line, error.token.character, error.message);
        }
    }
    free_tree_node(res, true);
    free_token_array_list(list);
    return 0;
    
}