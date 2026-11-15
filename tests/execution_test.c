//
// Created by userdepinfo on 15/11/26.
//

#include "execution.h"

int main(int argc, char** argv){
    if (argc < 2) {
        printf("parsing_test <file>\n");
        return 1;
    }

    char* path = argv[1];

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Error : can't find file '%s'.\n", path);
        return 1;
    }

    execute_from_file(file);
    return 0;
}