//
// Created by sofiane on 15/10/2026.
//

#include "execution.h"

int main(int argc, char** argv) {
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

    ExecutionContext *context = ExecutionContext_new_from_file(file);
    InstructionArrayList_apply_operation(&context->function, Instruction_print_readable);
    ExecutionContext_deinit(context);
    return 0;
}
