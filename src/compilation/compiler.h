//
// Created by sofiane on 06/09/2026.
//


#ifndef MATEMATIX_COMPILATOR_H
#define MATEMATIX_COMPILATOR_H


#include <stdint.h>
#include "parser.h"
#include "hash_set.h"
#include "hash_map.h"
#include "instruction.h"
#include <setjmp.h>

#include "instruction_block.h"


#define COMPILATION_SUCCESS 0;
#define COMPILATION_ERROR 1



ARRAY_LIST(instruction_block*, instructions_block)


typedef struct compiler {
    struct str_size_t_hash_map *variables;
    instructions_block_array_list *instructions_blocks_list;
    char error_message[1024];
    jmp_buf error_jmp;
    struct tmp_ptr_array_list *tmp_ptr_list;
    int status;
} compiler;

typedef struct compilation_result {
    parsing_result parsing_res;
    instructions_block_array_list *instructions_block_arrays;
    char error_message[1024];
    int compilation_status;
} compilation_result;





compiler *new_compiler(instructions_block_array_list *result_list);
compilation_result compile_from_file(FILE *file);
void free_compilation_result_members(compilation_result );
void save_instruction_block_list(const instructions_block_array_list *list, const FILE *file) ;

bool has_compilation_errors(compilation_result compilation_res);

void print_compilation_errors(compilation_result compilation_res, FILE *file);




#endif //MATEMATIX_COMPILATOR_H
