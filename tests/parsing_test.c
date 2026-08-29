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



    parser *parse= new_parser(list);
    parsing_node * res = parse_code(parse);


    if (parse->parsing_status==PARSING_ERROR) {
        printf("%s\n", parse->parsing_error_buffer);
        return 2;
    }

    printf("parsing result :\n ");

    display_tree_node(res);


    printf("\n\n");

    printf("parsing result (readable) :\n");

    display_tree_node_readable(res);


    printf("\n");



    free_tree_node(res);
    free_token_array_list(list);











    return 0;
}