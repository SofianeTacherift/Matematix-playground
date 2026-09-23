//
// Created by sofiane on 13/09/2026.
//

#include "parser.h"
#include "lexer.h"
#include "compiler.h"



int main(int argc, char ** argv) {
    if (argc<2) {
        printf("parsing_test <file>\n");
        return 1;
    }

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


    lexer *lexer = new_lexer();

    lex_code_from_file(lexer, buffer, sizeof(buffer), file);

    token_array_list * list = lexer->tokens_list;


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
            printf("error during parsing line %d character %d : %s \n", error.token.line+1, error.token.character+1, error.message);
        }
        return 2;
    }

    


    compiler *compiler = new_compiler(res);
    compile_main_scope(compiler, res);
    link_instructions_blocks(compiler);



    printf("\n\n\ncompilation result (%d):\n", compiler->instructions_blocks_list->size);

    instructions_block_array_list *instructions_block = compiler->instructions_blocks_list;
    for (size_t i = 0 ; i<instructions_block->size; i++) {
        print_instruction_block_recursive(instructions_block->elements[i]);
    }
    for (size_t i = 0 ; i<instructions_block->size; i++) {
        print_instructions_block_readable(instructions_block->elements[i],0);
    }
    free_tree_node(res, true);
    free_token_array_list(list);
    return 0;

}