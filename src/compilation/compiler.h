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
    int status;
} compiler;

typedef struct compilation_result {
    parsing_result parsing_res;
    instructions_block_array_list *instructions_block_arrays;
    char error_message[1024];
    int status;
} compilation_result;





compiler *new_compiler(instructions_block_array_list *result_list);
compilation_result compile_from_file(FILE *file);
void free_compilation_result_members(compilation_result );

void compile_code(compiler *compiler, parsing_node *node);
void compile_main_scope(compiler *compiler,  parsing_node *node);
void compile_scope(compiler *compiler, instruction_block *block,  parsing_node *node);
void compile_const(compiler *compiler,instruction_block *block, parsing_node *node);
void compile_variable_load(compiler *compiler, instruction_block *block, parsing_node *node);
void compile_primary(compiler *compiler, instruction_block *block,parsing_node *node);
void compile_logical_expression(compiler *compiler, instruction_block *block, parsing_node *node, instruction_block *true_block, instruction_block *false_block);
instruction_block *compile_expression(compiler *compiler,instruction_block *block, parsing_node *node);
void compile_arithmetic_binary(compiler *compiler,instruction_block *block, parsing_node *node);
void compile_arithmetic_unary(compiler *compiler,instruction_block *block, parsing_node *node);
void compile_affectation(compiler *compiler,instruction_block *block,  parsing_node *node);
instruction_block *compile_while(compiler *compiler, instruction_block *block, parsing_node *node);
void compile_instruction(compiler *compiler, instruction_block *block,parsing_node *node);
void link_instructions_blocks(compiler *compiler);
instruction_block *compile_if_statement(compiler *compiler, instruction_block *block, parsing_node *current);
void compile_(compiler *compiler);


#endif //MATEMATIX_COMPILATOR_H
