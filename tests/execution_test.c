//
// Created by sofiane on 15/10/2026.
//

#include "execution.h"


int main(int argc, char **argv) {

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
        perror("open input file");
        return 1;
    }

    execution_infos *infos = read_byte_code(file);
    apply_instruction_operation(infos->instructions ,print_instruction_readable);






    return 0;
}