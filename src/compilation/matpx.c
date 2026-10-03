//
// Created by sofiane on 01/10/2026.
//

#include "compiler.h"



bool test_valid_files_names(char *input_file_name, char *output_file_name) {
    size_t len_output_name = strlen(output_file_name);
    for (size_t i=0; i<len_output_name; i++) {
        if (output_file_name[i]=='.') return false;
    }

    size_t len_input_name = strlen(input_file_name);
    if (len_input_name<4) return false;

    if (strcmp(".mxp", input_file_name+len_input_name-4)!=0) return false;
    return true;
}


int main(int argc, char **argv) {
    char *args_error_message = "matpx <file.mxp> <file.bmpx>\n";
    if (argc<3) {
        printf("%s\n", args_error_message);
        return 1;
    }
    char *input_file_name=argv[1];
    char *output_file_name=argv[2];

    if (!test_valid_files_names(input_file_name, output_file_name)) {
        printf("%s\n", args_error_message);
        return 1;
    }


    FILE *input_file = fopen(input_file_name, "r");

    if (input_file==NULL) {
        perror("open input file");
        return 2;
    }

    FILE *output_file = fopen(output_file_name, "w");

    if (output_file==NULL) {
        perror("open output file");
        return 3;
    }

    compilation_result compilation_res = compile_from_file(input_file);

    if (has_compilation_errors(compilation_res)) {
        print_compilation_errors(compilation_res, stderr);
        return 4;
    }

    save_instruction_block_list(compilation_res.instructions_block_arrays, output_file);

    free_compilation_result_members(compilation_res);





}