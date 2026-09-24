//
// Created by sofiane on 06/09/2026.
//


#ifndef MATEMATIX_COMPILATOR_H
#define MATEMATIX_COMPILATOR_H


#include <stdint.h>
#include "parser.h"
#include "hash_set.h"
#include "hash_map.h"
#include "instructions.h"

#include "instructions_block.h"


HASH_MAP(char *, size_t, str, size_t)
HASH_SET(char*, str)


ARRAY_LIST(instructions_block, instructions_block)

typedef struct compiler {
    str_size_t_hash_map *variables;
    instructions_block_array_list *instructions_blocks_list;
    char error_message[1024];
} compiler;





compiler *new_compiler(parsing_node *head);
void compile_main_scope(compiler *compiler,  parsing_node *node);
void compile_scope(compiler *compiler, instructions_block *block,  parsing_node *node);
void compile_const(compiler *compiler,instructions_block *block, parsing_node *node);
void compile_variable(compiler *compiler, instructions_block *block, parsing_node *node);
void compile_primary(compiler *compiler, instructions_block *block,parsing_node *node);
void compile_logical_expression(compiler *compiler, instructions_block *block, parsing_node *node, instructions_block *true_block, instructions_block *false_block);
instructions_block *compile_expression(compiler *compiler,instructions_block *block, parsing_node *node);
void compile_arithmetic_binary(compiler *compiler,instructions_block *block, parsing_node *node);
void compile_affectation(compiler *compiler,instructions_block *block,  parsing_node *node);
void compile_instruction(compiler *compiler, instructions_block *block,parsing_node *node);
void link_instructions_blocks(compiler *compiler);
instructions_block *compile_if_statement(compiler *compiler, instructions_block *block, parsing_node *node);
void compile_(compiler *compiler);


#endif //MATEMATIX_COMPILATOR_H
