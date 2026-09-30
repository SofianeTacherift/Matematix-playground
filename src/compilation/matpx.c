//
// Created by sofiane on 25/09/2026.
//


#include "compiler.h"
#include "lexer.h"
#include "parser.h"


int main(int argc, char **argv) {
    if (argc<3) {
        printf("matpx <file.mpx> <file.bmpx>\n");
        return 1;
    }

    char * input_file_name = argv[1];
    char *output_file_name=argv[2];


    FILE *input_file = fopen(input_file_name, "r");

    if (input_file==NULL) {
        perror("open input file");
        return 1;
    }

    FILE *output_file = fopen(output_file_name, "a");

    if (output_file==NULL) {
        perror("open output file");
        return 2;
    }


    lexer *lexer = new_lexer();

    char code[10000];
    lex_code_from_file(lexer,code, sizeof code,input_file);

    if (lexer->tokens_list==NULL) {
        fprintf(stderr, "Error during lexing\n");
    }

    parser *parser = new_parser(lexer->tokens_list);
    parsing_node *head = parse_main_scope(parser);

    if (parser->parsing_errors->size!=0) {
        print_parser_errors(parser);
        return 3;
    }

    compiler *compiler = new_compiler(head);

    compile_code(compiler, head);

    if (compiler->status!=0) {
        fprintf(stderr, "%s\n", compiler->error_message);
        return 4;
    }


    save_compiler_result(compiler, output_file);

    fclose(input_file);
    fclose(output_file);







    return 0;
}


