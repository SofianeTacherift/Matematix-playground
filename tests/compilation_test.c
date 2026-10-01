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


    printf("path= \"\"\"%s\"\"\"\n\n", path);

    compilation_result compilation_res = compile_from_file(file);
    parsing_result parsing_res = compilation_res.parsing_res;
    lexing_result lexing_res = parsing_res.lexing_res;

    if (lexing_res.lexing_status==LEXING_ERROR) {
        fprintf(stderr, "%s\n",lexing_res.error_buffer);
        free_compilation_result_members(compilation_res);
        return 1;
    }
    if (parsing_res.parsing_status==PARSING_ERROR) {
        for (size_t i=0; i<parsing_res.errors->size; i++) {
            parsing_error error = parsing_res.errors->elements[i];
            fprintf(stderr, "parsing error at line %d character %d %s\n", error.token.line+1, error.token.character+1, error.message );

        }
        free_compilation_result_members(compilation_res);
        return 2;
    }
    if (compilation_res.status==COMPILATION_ERROR) {
        fprintf(stderr, "%s\n", compilation_res.error_message);
        free_compilation_result_members(compilation_res);
        return 3;
    }


    print_instruction_block_recursive(compilation_res.instructions_block_arrays->elements[0]);
    print_instruction_block_readable(compilation_res.instructions_block_arrays->elements[0], 0);


    free_compilation_result_members(compilation_res);



    return 0;

}