//
// Created by sofiane on 06/09/2026.
//

#include "compiler.h"
#include "parsing_node_utils.h"


void compile_code(Compiler* compiler, ParsingNode* node);
void compile_main_scope(Compiler* compiler, ParsingNode* node);
void compile_scope(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_const(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_variable_load(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_primary(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_logical_expression(Compiler* compiler, InstructionBlock* block, ParsingNode* node,InstructionBlock* true_block, InstructionBlock* false_block);
InstructionBlock* compile_expression(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_arithmetic_binary(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_arithmetic_unary(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_affectation(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
InstructionBlock* compile_while(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_instruction(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void link_instructions_blocks(Compiler* compiler);
InstructionBlock* compile_if_statement(Compiler* compiler, InstructionBlock* block, ParsingNode* node);
void compile_(Compiler* compiler);
InstructionBlock* compile_const_push_logical_expression(Compiler* compiler, InstructionBlock* block, ParsingNode* node);


#define copy_to_size_t(dest, src, type)     memcpy( ((char*) (&dest)) + (sizeof(size_t) - sizeof(src) ),  &src , sizeof(src));


typedef struct tmp_ptr {
    void* ptr;
    void (*free_func)(void*);
} tmp_ptr;


ARRAY_LIST(tmp_ptr, TmpPtrArrayList, TmpPtrArrayList)
HASH_MAP(ParsingNode*, InstructionBlock*, PNodeBlockHashMap, PNodeBlockEntry, PNodeBlockHashMap)
HASH_MAP(InstructionBlock *, size_t, InstructionBlockSizeTHashMap, InstructionBlockSizeTEntry,
         InstructionBlockSizeTHashMap)
HASH_MAP(char *, size_t, StringSizeTHashMap, StringSizeTEntry, StringSizeTHashMap)

#define GENERIC_FREE(DATA_STRUCTURE, PARAM, CODE) void DATA_STRUCTURE##_generic_free(void * PARAM ) { CODE }

GENERIC_FREE(PNodeBlockHashMap, map, PNodeBlockHashMap_free(map) ;)
GENERIC_FREE(InstructionBlock, block, InstructionBlock_free_recursive(block) ;)
//void generic_free_p_node_jump_hash_map(void *map) { free_p_node_jump_hash_map(map);}
GENERIC_FREE(ParsingNodeJumpHashMap, map, ParsingNodeJumpHashMap_free(map);)

void free_tmp_ptr(tmp_ptr t) {
    t.free_func(t.ptr);
}


void save_instruction_block_list(const InstructionBlockArrayList* list, const FILE* file) {
    if (file == NULL) {
        return;
    }
    for (size_t i = 0; i < list->size; i++) {
        InstructionBlock_save_recursive(list->elements[i], file);
    }
}


size_t remove_from_str_size_t_hash_map_if_value_greater_than(StringSizeTHashMap* map, size_t n) {
    size_t removed_count = 0;
    for (size_t i = 0; i < map->capacity; i++) {
        StringSizeTEntry* head = map->buckets[i];
        while (head != NULL && head->value > n) {
            map->buckets[i] = head->next;
            free(head);
            head = map->buckets[i];
            map->size--;
            removed_count++;
        }
        if (head != NULL) {
            StringSizeTEntry* current = head;
            while (current->next != ((void*)0)) {
                StringSizeTEntry* next = current->next;
                if (head->next->value > n) {
                    current->next = current->next->next;
                    free(next);
                    map->size--;
                    removed_count++;
                }
                else { current = current->next; }
            }
        }
    }
    return removed_count;
}

bool equals_str(const char* s1, const char* s2) {
    return strcmp(s1, s2) == 0;
}

long hash_str(const char* p) {
    char* str = p;
    long hash = 0;
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        int charI = (int)str[i];
        hash = 31 * hash + charI;
    }
    return hash;
}

void print_entry(char* s, size_t i) {
    printf("%s : %zu", s, i);
}


Compiler* new_compiler(InstructionBlockArrayList* result_list) {
    Compiler* res = calloc(1, sizeof(Compiler));
    if (res != NULL) {
        res->instructions_blocks_list = result_list;
        res->variables = StringSizeTHashMap_new(hash_str, equals_str);
        res->tmp_ptr_list = TmpPtrArrayList_new();
    }
    return res;
}

void free_compiler(Compiler* compiler) {
    StringSizeTHashMap_free(compiler->variables);
    TmpPtrArrayList_apply_operation(compiler->tmp_ptr_list, free_tmp_ptr);
    TmpPtrArrayList_free(compiler->tmp_ptr_list);
    free(compiler);
}


CompilationResult compile_from_file(FILE* file) {
    ParsingResult parsing_res = parse_from_file(file);
    LexingResult lexing_result = parsing_res.lexing_res;

    CompilationResult compilation_res = {.parsing_res = parsing_res};
    if (lexing_result.lexing_status == LEXING_ERROR || parsing_res.parsing_status == PARSING_ERROR) {
        return compilation_res;
    }

    InstructionBlockArrayList* res_list = InstructionBlockArrayList_new();
    ParsingNode* head = parsing_res.head;
    Compiler* compiler = new_compiler(res_list);

    compile_code(compiler, head);
    compilation_res.instructions_block_arrays = compiler->instructions_blocks_list;
    if (compiler->status == COMPILATION_ERROR) {
        compilation_res.compilation_status = COMPILATION_ERROR;
        memcpy(compilation_res.error_message, compiler->error_message, sizeof compiler->error_message);
    }
    else {
        compilation_res.compilation_status = COMPILATION_SUCCESS;
        link_instructions_blocks(compiler);
    }

    free_compiler(compiler);
    return compilation_res;
}

void CompilationResult_deinit(CompilationResult compilation_res) {
    ParsingResult_deinit(compilation_res.parsing_res);
    if (compilation_res.instructions_block_arrays != NULL) {
        InstructionBlockArrayList_apply_operation(compilation_res.instructions_block_arrays,
                                                  InstructionBlock_free_recursive);
        InstructionBlockArrayList_free(compilation_res.instructions_block_arrays);
    }
}

bool CompilationResult_has_error(CompilationResult compilation_res) {
    return compilation_res.compilation_status == COMPILATION_ERROR || compilation_res.parsing_res.parsing_status ==
        PARSING_ERROR || compilation_res.parsing_res.lexing_res.lexing_status == LEXING_ERROR;
}

void CompilationResult_print_errors(CompilationResult compilation_res, FILE* file) {
    if (file == NULL) return;

    ParsingResult parsing_res = compilation_res.parsing_res;
    LexingResult lexing_res = parsing_res.lexing_res;
    if (lexing_res.lexing_status == LEXING_ERROR) {
        fprintf(file, "%s\n", lexing_res.error_message);
    }
    else if (parsing_res.parsing_status == PARSING_ERROR) {
        for (size_t i = 0; i < parsing_res.errors->size; i++) {
            ParsingError error = parsing_res.errors->elements[i];
            fprintf(file, "parsing error at line %d character %d %s\n", error.token.line + 1, error.token.character + 1,
                    error.message);
        }
    }
    else if (compilation_res.compilation_status == COMPILATION_ERROR) {
        fprintf(file, "%s\n", compilation_res.error_message);
    }
}


void compile_code(Compiler* compiler, ParsingNode* node) {
    const int val = setjmp(compiler->error_jmp);
    if (val == 0) {
        compile_main_scope(compiler, node);
    }
    else { compiler->status = COMPILATION_ERROR; }
}

void compile_main_scope(Compiler* compiler, ParsingNode* node) {
    ParsingNode* current = node;
    InstructionBlock* block = InstructionBlock_new();
    InstructionBlock* start = block;
    InstructionBlockArrayList_add(compiler->instructions_blocks_list, start);
    while (current != NULL) {
        switch (current->type) {
        case OPENING_SCOPE_NODE:
            compile_scope(compiler, block, current);
            break;
        default:
            compile_instruction(compiler, block, current);
            break;
        }
        block = last_instruction_block_from_instruction_block(block);
        if (current->type == IF_NODE) {
            current = skip_if_statement(current);
        }
        else { current = current->next; }
    }
    block->next = InstructionBlock_new();
    InstructionArrayList_add(block->next->instructions, (Instruction){.type = RETURN_INSTRUCTION});
}

void compile_scope(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    size_t original_index = compiler->variables->size;
    ParsingNode* current = node->right;

    while (current != NULL) {
        switch (current->type) {
        case OPENING_SCOPE_NODE:
            compile_scope(compiler, block, current);
            break;
        default:
            compile_instruction(compiler, block, current);
            break;
        }
        block = last_instruction_block_from_instruction_block(block);
        if (current->type == IF_NODE) {
            current = skip_if_statement(current);
        }
        else { current = current->next; }
    }
    remove_from_str_size_t_hash_map_if_value_greater_than(compiler->variables, original_index - 1);
}


void compile_const(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    int64_t operand = 0;
    InstructionType type = 0;
    switch (node->type) {
    case INT_NODE:
        type = ICONST_INSTRUCTION;
        memcpy(&operand, &node->int_val, sizeof(int));
        break;
    case DOUBLE_NODE:
        type = DCONST_INSTRUCTION;
        memcpy(&operand, &node->double_val, sizeof(double));
        break;
    }
    Instruction result = {.type = type, .operand1 = operand};
    InstructionArrayList_add(block->instructions, result);
}

void compile_variable_load(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    const size_t* index = StringSizeTHashMap_get(compiler->variables, node->name);
    if (index != NULL) {
        Instruction res = {.type = OLOAD_INSTRUCTION, .operand1 = *index};
        InstructionArrayList_add(block->instructions, res);
    }
    else {
        compiler->status = COMPILATION_ERROR;
        snprintf(compiler->error_message, sizeof compiler->error_message, "Variable %s used but not declared",
                 node->name);
        longjmp(compiler->error_jmp, 2);
    }
}

void compile_primary(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    if (node->type == VARIABLE_NODE) {
        compile_variable_load(compiler, block, node);
    }
    else {
        compile_const(compiler, block, node);
    }
}


InstructionBlock* compile_expression_nb(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    InstructionBlock* end = compile_expression(compiler, block, node);
    if (end != block) {
        end->next = InstructionBlock_new();
        end = end->next;
    }
    return end;
}

InstructionBlock* compile_expression(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    switch (node->type) {
    case BINARY_NODE:
        if (!ParsingNode_is_logical_binary(node) && !ParsingNode_is_binary_comparison(node)) {
            compile_arithmetic_binary(compiler, block, node);
        }
        else {
            compile_const_push_logical_expression(compiler, block, node);
        }
        break;
    case UNARY_NODE:
        if (node->operation == LOGICAL_NOT_OPERATOR) {
            compile_const_push_logical_expression(compiler, block, node);
        }
        else {
            compile_arithmetic_unary(compiler, block, node);
        }
        break;
    default:
        compile_primary(compiler, block, node);
        break;
    }
    return last_instruction_block_from_instruction_block(block);
}


void compile_affectation(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {

    StringSizeTHashMap* variables = compiler->variables;


    InstructionBlock* end = compile_expression_nb(compiler, block, node->right);

    char* var_name = node->left->name;



    const size_t* index = StringSizeTHashMap_get(variables, var_name);
    size_t res_index = 0;
    if (index == NULL) {
        res_index = compiler->variables->size;
        StringSizeTHashMap_put(variables, var_name, res_index);
    }
    else {
        res_index = *index;
    }

    InstructionArrayList_add(end->instructions, (Instruction){.type = OSTORE_INSTRUCTION, .operand1 = res_index});
}


void compile_arithmetic_unary(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    block = compile_expression_nb(compiler, block, node->right);
    int instruction_type = InstructionType_from_unary_arithmetic_node(node);
    InstructionArrayList_add(block->instructions, (Instruction){.type = instruction_type});
}

void compile_arithmetic_binary(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    block = compile_expression_nb(compiler, block, node->left);
    block = compile_expression_nb(compiler, block, node->right);

    int instruction_type = InstructionType_from_binary_arithmetic_node(node);
    InstructionArrayList_add(block->instructions, (Instruction){.type = instruction_type});
}


void compile_single_condition(Compiler* compiler, InstructionBlock* block, ParsingNode* node, bool inverse_condition) {
    Instruction comparison = {0};
    if (ParsingNode_is_binary_comparison(node)) {
        block = compile_expression_nb(compiler, block, node->left);
        block = compile_expression_nb(compiler, block, node->right);
        int operator = (inverse_condition) ? Operator_inverse_comparison(node->operation) : node->operation;
        int type = InstructionType_from_comparison_operator(operator);
        comparison.type = type;
        InstructionArrayList_add(block->instructions, comparison);
    }
    else if (node->type == UNARY_NODE && node->operation == LOGICAL_NOT_OPERATOR) {
        InstructionBlock* end = compile_expression_nb(compiler, block, node);
        InstructionArrayList_add(end->instructions, (Instruction){.type = ICONST_INSTRUCTION, .operand1 = 0});
        comparison.type = (inverse_condition) ? IF_CMPEQ : IF_CMPNE;
        InstructionArrayList_add(end->instructions, comparison);
    }
    else {
        InstructionBlock* end = compile_expression(compiler, block, node);
        InstructionArrayList_add(end->instructions, (Instruction){.type = ICONST_INSTRUCTION, .operand1 = 0});
        comparison.type = (inverse_condition) ? IF_CMPEQ : IF_CMPNE;
        InstructionArrayList_add(end->instructions, comparison);
    }
}

void map_conditions_instructions_block(InstructionBlock* prev, PNodeBlockHashMap* map, Compiler* compiler,
                                       ParsingNode* current, ParsingNodeJumpHashMap* jumps,
                                       InstructionBlock* true_block, InstructionBlock* false_block) {
    InstructionBlock* result = InstructionBlock_new();
    prev->next = result;
    const JumpInfos* infos_ptr = ParsingNodeJumpHashMap_get(jumps, current);
    JumpInfos infos = *infos_ptr;

    compile_single_condition(compiler, result, current, !infos.jump_if);
    // if we jump when the condition is false, we inverse the condition

    InstructionBlock* result_end = last_instruction_block_from_instruction_block(result);

    ParsingNode* next_node = infos.next_node;

    if (next_node == NULL) {
        result_end->next = (infos.jump_if) ? false_block : true_block;
    }
    else {
        map_conditions_instructions_block(result_end, map, compiler, next_node, jumps, true_block, false_block);
    }


    ParsingNode* jump = infos.jump_node;

    if (jump == NULL) {
        result_end->jump = (infos.jump_if) ? true_block : false_block;
    }
    else {
        const InstructionBlock** result_ptr = PNodeBlockHashMap_get(map, jump);
        result_end->jump = (InstructionBlock*)*result_ptr;
    }


    PNodeBlockHashMap_put(map, current, result);
}


InstructionBlock*
compile_const_push_logical_expression(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    bool logical_not = node->type == UNARY_NODE && node->operation == LOGICAL_NOT_OPERATOR;

    InstructionBlock* true_block = logical_not
                                       ? InstructionBlock_new_boolean_push(false)
                                       : InstructionBlock_new_boolean_push(true);
    InstructionBlock* false_block = logical_not
                                        ? InstructionBlock_new_boolean_push(true)
                                        : InstructionBlock_new_boolean_push(false);

    false_block->next = true_block;


    InstructionArrayList_add(false_block->instructions, (Instruction){.type = GOTO});

    false_block->type = FALSE_INSTRUCTION_CONST;

    size_t original_tmp_ptr_list_size = compiler->tmp_ptr_list->size;
    TmpPtrArrayList_add(compiler->tmp_ptr_list,
                        (tmp_ptr){.ptr = false_block, .free_func = InstructionBlock_generic_free});

    compile_logical_expression(compiler, block, logical_not ? node->right : node, true_block, false_block);

    compiler->tmp_ptr_list->size = original_tmp_ptr_list_size;
    return false_block;
}

void compile_logical_expression(Compiler* compiler, InstructionBlock* block, ParsingNode* node,
                                InstructionBlock* true_block, InstructionBlock* false_block) {
    size_t start_ptr_list_size = compiler->tmp_ptr_list->size;
    ParsingNodeJumpHashMap* jumps = map_condition_jumps(node);
    TmpPtrArrayList_add(compiler->tmp_ptr_list,
                        (tmp_ptr){.ptr = jumps, .free_func = ParsingNodeJumpHashMap_generic_free});

    PNodeBlockHashMap* node_instruction_block_map = PNodeBlockHashMap_new(
        ParsingNode_hash_address, ParsingNode_equals_address);
    TmpPtrArrayList_add(compiler->tmp_ptr_list, (tmp_ptr){
                            .ptr = node_instruction_block_map, .free_func = PNodeBlockHashMap_generic_free
                        });

    ParsingNode* most_left = node;
    while (ParsingNode_is_logical_binary(most_left)) {
        most_left = most_left->left;
    }
    map_conditions_instructions_block(block, node_instruction_block_map, compiler, most_left, jumps, true_block,
                                      false_block);

    ParsingNodeJumpHashMap_free(jumps);
    PNodeBlockHashMap_free(node_instruction_block_map);
    compiler->tmp_ptr_list->size = start_ptr_list_size;
}


void compile_instruction(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    switch (node->type) {
    case AFFECTATION_NODE:
        compile_affectation(compiler, block, node);
        break;
    case IF_NODE:
        compile_if_statement(compiler, block, node);
        break;
    case WHILE_NODE:
        compile_while(compiler, block, node);
    default:
        break;
    }
}

InstructionBlock* compile_conditional_node(Compiler* compiler, InstructionBlock* block, ParsingNode* node,
                                           InstructionBlock* jump) {
    if (node->type == IF_NODE || node->type == ELIF_NODE) {
        compile_const_push_logical_expression(compiler, block, node->condition);
        InstructionBlock* end_block = last_instruction_block_from_instruction_block(block);

        InstructionBlock* comparison_block = InstructionBlock_new();
        end_block->next = comparison_block;

        InstructionArrayList_add(comparison_block->instructions,
                                 (Instruction){.type = ICONST_INSTRUCTION, .operand1 = 0});
        InstructionArrayList_add(comparison_block->instructions, (Instruction){.type = IF_CMPEQ});


        InstructionBlock* true_branch = InstructionBlock_new();
        comparison_block->next = true_branch;
        if (node->true_condition->type == OPENING_SCOPE_NODE) {
            compile_scope(compiler, true_branch, node->true_condition);
        }
        else {
            compile_instruction(compiler, true_branch, node->true_condition);
        }

        InstructionBlock* true_branch_end = last_instruction_block_from_instruction_block(true_branch);

        InstructionBlock* false_branch = InstructionBlock_new();
        true_branch_end->next = false_branch;


        if (node->next && (node->next->type == ELIF_NODE || node->next->type == ELSE_NODE)) {
            InstructionArrayList_add(true_branch_end->instructions, (Instruction){.type = GOTO});
            true_branch_end->jump = jump;
        }

        comparison_block->jump = false_branch;

        return false_branch;
    }


    InstructionBlock* branch = InstructionBlock_new();
    block->next = branch;

    // else case
    if (node->true_condition->type == OPENING_SCOPE_NODE) {
        compile_scope(compiler, branch, node->true_condition);
    }
    else {
        compile_instruction(compiler, branch, node->true_condition);
    }

    InstructionBlock* branch_end = last_instruction_block_from_instruction_block(branch);

    branch_end->next = jump;
    return jump;
}

InstructionBlock* compile_if_statement(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    InstructionBlock* jump = InstructionBlock_new();
    size_t original_tmp_ptr_list_size = compiler->tmp_ptr_list->size;
    TmpPtrArrayList_add(compiler->tmp_ptr_list, (tmp_ptr){jump, InstructionBlock_generic_free});

    ParsingNode* current = node;
    InstructionBlock* curr_instruction_block = compile_conditional_node(compiler, block, current, jump);

    current = current->next;
    while (current && (current->type == ELIF_NODE || current->type == ELSE_NODE)) {
        curr_instruction_block = compile_conditional_node(compiler, curr_instruction_block, current, jump);
        current = current->next;
    }

    curr_instruction_block = last_instruction_block_from_instruction_block(curr_instruction_block);
    if (curr_instruction_block != jump) {
        // if we compile else statement, the result is jump
        curr_instruction_block->next = jump;
    }

    compiler->tmp_ptr_list->size = original_tmp_ptr_list_size;

    return jump;
}

InstructionBlock* compile_while(Compiler* compiler, InstructionBlock* block, ParsingNode* node) {
    InstructionBlock* condition = InstructionBlock_new();
    block->next = condition;

    compile_const_push_logical_expression(compiler, condition, node->condition);
    InstructionBlock* condition_end = last_instruction_block_from_instruction_block(condition);

    InstructionBlock* verification = InstructionBlock_new();
    condition_end->next = verification;
    InstructionArrayList_add(verification->instructions, (Instruction){.type = ICONST_INSTRUCTION, .operand1 = 0});
    InstructionArrayList_add(verification->instructions, (Instruction){.type = IF_CMPEQ});


    InstructionBlock* true_block = InstructionBlock_new();
    verification->next = true_block;

    compile_scope(compiler, true_block, node->true_condition);

    InstructionBlock* true_block_end = last_instruction_block_from_instruction_block(true_block);
    InstructionArrayList_add(true_block_end->instructions, (Instruction){.type = GOTO});

    InstructionBlock* result = InstructionBlock_new();
    true_block_end->next = result;

    verification->jump = result;
    true_block_end->jump = condition;
    return result;
}


InstructionBlockSizeTHashMap* map_instruction_block_index(InstructionBlock* start) {
    InstructionBlockSizeTHashMap* map = InstructionBlockSizeTHashMap_new(
        InstructionBlock_hash_address, InstructionBlock_equals_address);
    size_t count = 0;
    InstructionBlock* current = start;
    while (current != NULL) {
        InstructionBlockSizeTHashMap_put(map, current, count);
        count += current->instructions->size;
        current = current->next;
    }
    return map;
}


void link_instructions_blocks(Compiler* compiler) {
    for (size_t i = 0; i < compiler->instructions_blocks_list->size; i++) {
        InstructionBlock* start = compiler->instructions_blocks_list->elements[i];
        InstructionBlockSizeTHashMap* indexs = map_instruction_block_index(start);
        InstructionBlock* current = start;
        while (current != NULL) {
            if (current->type == FALSE_INSTRUCTION_CONST) {
                current->jump = current->next->next;
            }

            if (current->jump != NULL) {
                const size_t* index_ptr = InstructionBlockSizeTHashMap_get(indexs, current->jump);

                if (index_ptr != NULL) {
                    const size_t index = *index_ptr;
                    InstructionArrayList* list = current->instructions;
                    (list->elements + list->size - 1)->operand1 = index;
                }
            }
            current = current->next;
        }
        InstructionBlockSizeTHashMap_free(indexs);
    }
}
