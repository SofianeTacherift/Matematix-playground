#include "parser.h"
#include "lexer.h"


int main(int argc, char ** argv) {

    if (argc<2) {
        printf("test_parser <statement>\n");
        return 1;
    }
    char * code = argv[1];
    printf("code = '''%s'''", code);
    token_array_list *list = lexe_code(code);

    printf("\ntokens list : ");
    print_token_list(list);
    printf("\n");



    parser *parse= new_parser(list);
    node * res = begin_parsement(parse);
    if (parse->parsing_status==PARSING_ERROR) {
        printf("%s\n", parse->parsing_error_buffer);
        return 2;
    }
    printf("parsing result : ");
    display_tree_node(res);
    printf("\n");





    return 0;
}