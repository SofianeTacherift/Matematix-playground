//
// Created by sofiane on 06/09/2026.
//

#ifndef MATEMATIX_COMPILATOR_H
#define MATEMATIX_COMPILATOR_H

#include <stdint.h>
#include "parser.h"
#include "spds/hash_map.h"
#include "spds/hash_set.h"

#include "instruction.h"
#include <setjmp.h>

#include "instruction_block.h"

#define COMPILATION_SUCCESS 0;
#define COMPILATION_ERROR 1

ARRAY_LIST(InstructionBlock*, InstructionBlockArrayList, InstructionBlockArrayList)

typedef struct Compiler {
    struct StringSizeTHashMap* variables;
    InstructionBlockArrayList* instructions_blocks_list;
    char error_message[1024];
    jmp_buf error_jmp;
    struct TmpPtrArrayList* tmp_ptr_list;
    int status;
} Compiler;

typedef struct CompilationResult {
    ParsingResult parsing_res;
    InstructionBlockArrayList* instructions_block_arrays;
    char error_message[1024];
    int compilation_status;
} CompilationResult;

CompilationResult compile_from_file(FILE* file);
void CompilationResult_deinit(CompilationResult);
bool CompilationResult_has_error(CompilationResult compilation_res);
void CompilationResult_print_errors(CompilationResult compilation_res, FILE* file);

#endif //MATEMATIX_COMPILATOR_H
