//
// Created by sofiane on 06/09/2026.
//

#include "compiler.h"

#include "stdint.h"


#define copy_to_size_t(dest, src, type)     memcpy( ((char*) (&dest)) + (sizeof(size_t) - sizeof(src) ),  &src , sizeof(src));




HASH_MAP(instruction_block *, size_t, instructions_block, size_t)
HASH_MAP(char *, size_t, str, size_t)









size_t remove_from_str_size_t_hash_map_if_value_greater_than(str_size_t_hash_map *map, size_t n) {
    size_t removed_count = 0;
    for (size_t i = 0; i < map->capacity; i++) {
        str_size_t_entry *head = map->buckets[i];
        while (head != ((void *) 0) && head->value> n) {
            map->buckets[i] = head->next;
            free(head);
            head = map->buckets[i];
            map->size--;
            removed_count++;
        }
        if (head != ((void *) 0)) {
            str_size_t_entry *current = head;
            while (current->next != ((void *) 0)) {
                str_size_t_entry *next = current->next;
                if (head->next->value > n) {
                    current->next = current->next->next;
                    free(next);
                    map->size--;
                    removed_count++;
                } else { current = current->next; }
            }
        }
    }
    return removed_count;
}

bool equals_str(char *s1, char *s2) {
    return strcmp(s1,s2)==0;
}

long hash_str(char  *p) {
    char *str=p;
    long hash=0;
    int len=strlen(str);
    for (int i=0; i<len;i++) {
        int charI=str[i];
        hash=31*hash+charI;
    }
    return hash;
}

void print_entry(char * s, size_t i) {
    printf("%s : %zu", s, i);
}




compiler *new_compiler(instructions_block_array_list *result_list) {
    compiler *res = calloc(1,sizeof(compiler));
    if (res!=NULL) {
        res->instructions_blocks_list=result_list;
        res->variables=new_str_size_t_hash_map(hash_str, equals_str, print_entry);
    }
    return res;
}

void free_compiler(compiler *compiler) {
    free_str_size_t_hash_map(compiler->variables);
    free(compiler);
}

compilation_result compile_from_file(FILE *file) {
    parsing_result parsing_res = parse_from_file(file);
    lexing_result lexing_result = parsing_res.lexing_res;

    compilation_result compilation_res = {.parsing_res = parsing_res};
    if (lexing_result.lexing_status==LEXING_ERROR || parsing_res.parsing_status==PARSING_ERROR) {
        return compilation_res;
    }

    instructions_block_array_list *res_list= new_instructions_block_array_list();
    parsing_node *head = parsing_res.head;
    compiler *compiler = new_compiler(res_list);

    compile_main_scope(compiler, head);

    if (compiler->status==COMPILATION_ERROR) {
        compilation_res.status=COMPILATION_ERROR;
        memcpy(compilation_res.error_message, compiler->error_message, sizeof compiler->error_message);
    }
    else {
        compilation_res.status=COMPILATION_SUCCESS;
        compilation_res.instructions_block_arrays=compiler->instructions_blocks_list;
    }
    link_instructions_blocks(compiler);
    free_compiler(compiler);
    return compilation_res;

}

void free_compilation_result_members(compilation_result compilation_res) {
    free_parsing_result_members(compilation_res.parsing_res);
    if (compilation_res.instructions_block_arrays!=NULL) {
        apply_instructions_block_operation(compilation_res.instructions_block_arrays, free_instruction_block_recursive);
        free_instructions_block_array_list(compilation_res.instructions_block_arrays);
    }

}



void compile_code(compiler *compiler, parsing_node *node) {
    const int val=setjmp(compiler->error_jmp);
    if (val!=0) {
        compile_main_scope(compiler, node);
    }
    compiler->status=COMPILATION_ERROR;
}

void compile_main_scope(compiler *compiler, parsing_node *node) {

    parsing_node *current=node;
    instruction_block *block=new_instructions_block();
    instruction_block *start=block;
    while (current!=NULL) {
        switch (current->type) {
            case OPENING_SCOPE_NODE:
                compile_scope(compiler,block, current);
                break;
            default:
                compile_instruction(compiler,block, current);
                break;
        }
        block=last_instruction_block_from_instruction_block(block);
        if (current->type==IF_NODE) {
            current=skip_if_statement(current);
        }
        else {current=current->next;}
    }
    block->next=new_instructions_block();
    add_instruction(block->next->instructions, (instruction) {.type = RETURN_INSTRUCTION});


    add_instructions_block(compiler->instructions_blocks_list, start);
}

void compile_scope(compiler *compiler,instruction_block *block, parsing_node *node) {
    size_t original_index=compiler->variables->size;
    parsing_node *current=node->right;

    while (current !=NULL) {
        switch (current->type) {
            case OPENING_SCOPE_NODE:
                compile_scope(compiler, block , current);
                break;
            default:
                compile_instruction(compiler,block, current);
                break;
        }
        block=last_instruction_block_from_instruction_block(block);
        if (current->type==IF_NODE) {
            current=skip_if_statement(current);
        }
        else {current=current->next;}
    }
    remove_from_str_size_t_hash_map_if_value_greater_than(compiler->variables, original_index-1);
}



void compile_const(compiler *compiler, instruction_block *block, parsing_node*node) {
    int64_t operand=0;
    instruction_type type=0;
    switch (node->type) {
        case INT_NODE:
            type=ICONST_INSTRUCTION;
            memcpy(&operand, &node->int_val, sizeof(int));
            break;
        case DOUBLE_NODE:
            type=DCONST_INSTRUCTION;
            memcpy(&operand, &node->double_val, sizeof(double));
            break;
    }
    instruction result = {.type = type, .operand1 = operand};
    add_instruction(block->instructions, result);
}

void compile_variable_load(compiler *compiler, instruction_block *block, parsing_node *node) {
    const size_t *index = get_from_str_size_t_hash_map(compiler->variables, node->string_val);
    if (index!=NULL) {
        instruction res= {.type = OLOAD_INSTRUCTION , .operand1 = *index };
        add_instruction(block->instructions, res);
    }
    else {
        compiler->status=1;
        snprintf(compiler->error_message, sizeof compiler->error_message, "Variable %s used but not declared", node->string_val);
    }
}

void compile_primary(compiler *compiler, instruction_block *block, parsing_node *node) {
    if (node->type==VARIABLE_NODE) {
        compile_variable_load(compiler, block, node);
    }
    else {
        compile_const(compiler, block, node);
    }
}



instruction_block *compile_expression_nb(compiler *compiler, instruction_block *block, parsing_node *node) {
    instruction_block *end = compile_expression(compiler, block, node);
    if (end!=block) {
        end->next=new_instructions_block();
        end=end->next;
    }
    return end;
}
instruction_block  *compile_expression(compiler *compiler, instruction_block *block, parsing_node *node) {
    if (node==NULL) {return NULL;}
    const instruction_block *start_block = block;
    switch (node->type) {
        case BINARY_NODE:
            if (!is_logical_binary_node(node) && !is_binary_comparison_node(node)) {
                compile_arithmetic_binary(compiler, block, node);
            }
            else {
                instruction_block *b_true = new_boolean_block_push(true);
                instruction_block *b_false = new_boolean_block_push(false);
                b_false->type=FALSE_INSTRUCTION_CONST;
                add_instruction(b_false->instructions, (instruction) {.type = GOTO});
                b_false->next=b_true;
                compile_logical_expression(compiler, block, node, b_true, b_false);
            }
            break;
        case UNARY_NODE:
            if (node->operation==LOGICAL_NOT_OPERATOR) {
                instruction_block *b_true = new_boolean_block_push(true);
                instruction_block *b_false = new_boolean_block_push(false);
                b_true->type=FALSE_INSTRUCTION_CONST;
                add_instruction(b_true->instructions, (instruction) {.type = GOTO});
                b_true->next=b_false;
                compile_logical_expression(compiler, block, node->right, b_false, b_true);
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

void compile_affectation(compiler *compiler, instruction_block *block,  parsing_node *node) {
    str_size_t_hash_map *variables = compiler->variables;

    instruction_block *end=compile_expression_nb(compiler, block, node->right);

    char * var_name = node->left->string_val;


    const size_t *index=get_from_str_size_t_hash_map(variables, var_name);
    size_t res_index=0;
    if (index==NULL) {
        res_index = compiler-> variables->size;
        put_to_str_size_t_hash_map(variables, var_name, res_index);
    }
    else {
        res_index=*index;
    }

    add_instruction(end->instructions, (instruction) {.type = OSTORE_INSTRUCTION, .operand1 = res_index });
}




void compile_arithmetic_unary(compiler *compiler, instruction_block *block, parsing_node *node) {
    block=compile_expression_nb(compiler,block, node->right);
    int instruction_type = unary_arithmetic_node_to_instruction_type(node);
    add_instruction(block->instructions, (instruction) {.type = instruction_type});
}

void compile_arithmetic_binary(compiler * compiler , instruction_block *block, parsing_node *node) {
    block=compile_expression_nb(compiler,block, node->left);
    block=compile_expression_nb(compiler,block, node->right);
    int instruction_type = binary_arithmetic_node_to_instruction_type(node);
    add_instruction(block->instructions, (instruction) {.type = instruction_type});
}


HASH_MAP(parsing_node*, instruction_block*, p_node, instructions)




void compile_single_condition(compiler *compiler, instruction_block *block, parsing_node *node, bool inverse_condition) {
    instruction comparison= {0};
    if (is_binary_comparison_node(node)) {
        block=compile_expression_nb(compiler, block, node->left);
        block=compile_expression_nb(compiler, block, node->right);
        int operator = (inverse_condition) ? inverse_comparison_operator(node->operation) : node->operation;
        int type = comparison_operator_to_instruction_type(operator);
        comparison.type = type;
        add_instruction(block->instructions, comparison);
    }
    else if (node->type==UNARY_NODE && node->operation==LOGICAL_NOT_OPERATOR) {
        instruction_block *end =compile_expression_nb(compiler, block, node);
        add_instruction(end->instructions, (instruction) {.type = ICONST_INSTRUCTION, .operand1 = 0});
        comparison.type= (inverse_condition) ? IF_CMPEQ : IF_CMPNE;
        add_instruction(end->instructions, comparison);
    }
    else {
        instruction_block *end =compile_expression(compiler, block, node);
        add_instruction(end->instructions, (instruction) {.type = ICONST_INSTRUCTION, .operand1 = 0});
        comparison.type= (inverse_condition) ? IF_CMPEQ : IF_CMPNE;
        add_instruction(end->instructions, comparison);
    }

}

instruction_block *map_conditions_instructions_block(p_node_instructions_hash_map *map, compiler *compiler, parsing_node *current , p_node_jump_hash_map *jumps, instruction_block *true_block, instruction_block *false_block) {
    instruction_block *result = new_instructions_block();
    const jump_infos *infos_ptr =  get_from_p_node_jump_hash_map(jumps, current);

    jump_infos infos = *infos_ptr;

    compile_single_condition(compiler, result, current, !infos.jump_if);
    instruction_block *result_end=last_instruction_block_from_instruction_block(result);
    parsing_node *next=infos.next_node;

    instruction_block *next_block=NULL;

    if (next==NULL) {
        next_block=(infos.jump_if) ? false_block : true_block;
    }
    else {
        next_block=map_conditions_instructions_block(map, compiler, next, jumps, true_block, false_block);
    }


    parsing_node *jump = infos.jump_node;

    if (jump==NULL) {
        result_end->jump=(infos.jump_if) ? true_block : false_block;
    }
    else {
        const instruction_block **result_ptr = get_from_p_node_instructions_hash_map(map, jump);
        result_end->jump = (instruction_block *) *result_ptr;
    }

    if (result_end!=result) {
        result_end->next=new_instructions_block();
        result_end=result_end->next;
    }
    result_end->next=next_block;


    put_to_p_node_instructions_hash_map(map, current, result);

    return result;

}

void print_node_instruction(parsing_node *node , instruction_block *block) {
    printf("node : ");
    display_node(node);

    printf("\ninstructions :\n");
    print_instruction_block(block);

    printf("\n");
}


instruction_block *compile_const_push_logical_expression(compiler *compiler, instruction_block *block, parsing_node *node) {
    bool logical_not = node->type==UNARY_NODE && node->operation==LOGICAL_NOT_OPERATOR;

    instruction_block *true_block= logical_not ?  new_boolean_block_push(false) : new_boolean_block_push(true);
    instruction_block *false_block = logical_not ? new_boolean_block_push(true) : new_boolean_block_push(false);

    false_block->next=true_block;
    add_instruction(false_block->instructions, (instruction) { .type = GOTO} );
    false_block->type=FALSE_INSTRUCTION_CONST;


    compile_logical_expression(compiler, block, logical_not ? node->right : node, true_block, false_block);
    return false_block;


}

void compile_logical_expression(compiler *compiler, instruction_block *block, parsing_node *node, instruction_block *true_block, instruction_block *false_block)  {

    p_node_jump_hash_map *jumps = map_condition_jumps(node);
    p_node_instructions_hash_map *map_node_instructions_block = new_p_node_instructions_hash_map(hash_node_addr,equals_node_addr, print_node_instruction);

    parsing_node *most_left = node;
    while (  is_logical_binary_node(most_left) ) {
        most_left=most_left->left;
    }
    block->next=map_conditions_instructions_block(map_node_instructions_block,compiler, most_left, jumps, true_block, false_block);

    free_p_node_jump_hash_map(jumps);
    free_p_node_instructions_hash_map(map_node_instructions_block);
}









void compile_instruction(compiler * compiler, instruction_block *block, parsing_node * node) {
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

void print_instruction_block_size_t(instruction_block *b, size_t t) {
    if (b->instructions->size>1 && b->instructions->elements[0].type==ICONST_INSTRUCTION) {
        printf("\n\n\n  ------------------------\n");
        print_instruction_block(b);
        printf("%zu\n", t);
        printf("-------------------------------");
    }
}






instruction_block *compile_conditional_node(compiler *compiler, instruction_block *block, parsing_node *node, instruction_block *jump) {
    if (node->type==IF_NODE || node->type==ELIF_NODE) {
        instruction_block *true_branch=new_instructions_block();
        if (node->true_condition->type==OPENING_SCOPE_NODE) {
            compile_scope(compiler, true_branch, node->true_condition);
        }
        else {
            compile_instruction(compiler, true_branch, node->true_condition);
        }



        instruction_block *false_branch=new_instructions_block();





        compile_const_push_logical_expression(compiler, block, node->condition);
        instruction_block *end_block=last_instruction_block_from_instruction_block(block);



        instruction_block *comparison_block = new_instructions_block();
        add_instruction(comparison_block->instructions, (instruction) {.type = ICONST_INSTRUCTION, .operand1 = 0});
        add_instruction(comparison_block->instructions, (instruction) {.type = IF_CMPEQ});

        end_block->next=comparison_block;




        instruction_block *true_branch_end = last_instruction_block_from_instruction_block(true_branch);


        if (node->next && (node->next->type==ELIF_NODE || node->next->type==ELSE_NODE)) {
            add_instruction(true_branch_end->instructions, (instruction) {.type = GOTO});
            true_branch_end->jump=jump;

        }
        true_branch_end->next=false_branch;
        comparison_block->jump=false_branch;
        comparison_block->next=true_branch;

        return false_branch;
    }


    instruction_block *branch = new_instructions_block();
    // else case
    if (node->true_condition->type==OPENING_SCOPE_NODE) {
        compile_scope(compiler, branch, node->true_condition);
    }
    else {
        compile_instruction(compiler, branch, node->true_condition);
    }

    instruction_block *branch_end = last_instruction_block_from_instruction_block(branch);
    block->next=branch;
    branch_end->next=jump;
    return jump;

}

instruction_block *compile_if_statement(compiler *compiler, instruction_block *block, parsing_node *node) {
    instruction_block *jump = new_instructions_block();
    parsing_node *current=node;
    instruction_block *curr_instruction_block=compile_conditional_node(compiler, block, current, jump);

    current=current->next;
    while (current &&  (current->type==ELIF_NODE || current->type==ELSE_NODE)) {
        curr_instruction_block=compile_conditional_node(compiler, curr_instruction_block, current, jump);
        current=current->next;
    }

    curr_instruction_block=last_instruction_block_from_instruction_block(curr_instruction_block);
    if (curr_instruction_block!=jump) {
        curr_instruction_block->next=jump;
    }


    return jump;

}

instruction_block *compile_while(compiler *compiler, instruction_block *block, parsing_node *node) {


    instruction_block *condition=new_instructions_block();
    compile_const_push_logical_expression(compiler, condition, node->condition);
    instruction_block *condition_end = last_instruction_block_from_instruction_block(condition);

    instruction_block *verification = new_instructions_block();
    add_instruction(verification->instructions, (instruction) {.type = ICONST_INSTRUCTION, .operand1 = 0});
    add_instruction(verification->instructions, (instruction) {.type = IF_CMPEQ});


    instruction_block *true_block = new_instructions_block();
    compile_scope(compiler, true_block, node->true_condition);

    instruction_block *true_block_end =last_instruction_block_from_instruction_block(true_block);
    add_instruction(true_block_end->instructions, (instruction) {.type = GOTO});

    instruction_block *result=new_instructions_block();

    block->next=condition;
    condition_end->next=verification;
    verification->jump=result;
    verification->next=true_block;
    true_block_end->next=result;
    true_block_end->jump=condition;
    true_block_end->next=result;
    return result;
}






instructions_block_size_t_hash_map *map_instruction_block_index(instruction_block *start) {
    instructions_block_size_t_hash_map *map = new_instructions_block_size_t_hash_map( hash_instruction_block_address , equals_instruction_block_address, print_instruction_block_size_t );
    size_t count=0;
    instruction_block *current= start;
    while (current!=NULL) {
        put_to_instructions_block_size_t_hash_map(map, current, count);
        count+=current->instructions->size;
        current=current->next;

    }
    return map;
}


void link_instructions_blocks(compiler *compiler) {
    for (size_t i =0; i<compiler->instructions_blocks_list->size; i++) {
        instruction_block *start = compiler->instructions_blocks_list->elements[i];
        instructions_block_size_t_hash_map *indexs = map_instruction_block_index(start);
        instruction_block *current = start;
        while (current!=NULL) {
            if (current->type==FALSE_INSTRUCTION_CONST) {
                current->jump=current->next->next;
            }

            if (current->jump!=NULL) {
                const size_t *index_ptr =  get_from_instructions_block_size_t_hash_map(indexs, current->jump);

                if (index_ptr!=NULL) {
                    const size_t index = *index_ptr;
                    instruction_array_list *list = current->instructions;
                    (list->elements+list->size-1)->operand1=index;
                }
            }
            current=current->next;
        }
        free_instructions_block_size_t_hash_map(indexs);
    }
}








