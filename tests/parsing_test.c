#include "parser.h"
#include "lexer.h"


int main(int argc, char ** argv) {
    if (argc<2) {
        printf("parsing_test <code>\n");
        return 1;
    }

    // parsing_node * n = new_parsing_node();
    // n->left=new_parsing_node();
    // n->right=new_parsing_node();

    // printf("left : %p\n right : %p\n condition : %p\n true_condition : %p\n jump : %p\n", n->left, n->right, n->condition, n->true_condition, n->jump);

    // return 0;

    char * code = argv[1];
    printf("code = '''%s'''", code);
    token_array_list *list = lex_code(code);

    printf("\ntokens list : ");
    print_token_list(list);
    printf("\n");

 



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

    free_tree_node(res);
    free_token_array_list(list);
    return 0;
    
}