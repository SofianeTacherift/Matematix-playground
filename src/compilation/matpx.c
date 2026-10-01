//
// Created by sofiane on 01/10/2026.
//

#include "compiler.h"






int main(int argc, char **argv) {
    if (argc<3) {
        printf("%s <file.mxp> <file.bmpx>\n");
        return 1;
    }
    char *input_file_name=argv[1];
    char *output_file_name=argv[2];

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

    



}