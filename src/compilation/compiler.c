//
// Created by sofiane on 06/09/2026.
//

#include "compiler.h"

#include "stdint.h"


#define copy_to_size_t(dest, src, type)     memcpy( ((char*) (&dest)) + (sizeof(size_t) - sizeof(src) ),  &src , sizeof(src));




bool greater_than(size_t value, size_t n) {
    return value>n;
}



size_t remove_from_str_size_t_hash_map_if_value_greater_than(str_size_t_hash_map *map, size_t n) {
    size_t removed_count = 0;
    for (size_t i = 0; i < map->capacity; i++) {
        str_size_t_entry *head = map->buckets[i];
        while (head != ((void *) 0) && greater_than(head->value, n)) {
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
                if (greater_than (head->next->value, n)) {
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

compiler *new_compiler(parsing_node *head) {
    compiler *res = calloc(1,sizeof(compiler));
    if (res!=NULL) {
        res->instructions_blocks_list=new_instructions_block_array_list();
        res->variables=new_str_size_t_hash_map(hash_str, equals_str, print_entry);
    }
    return res;
}


void compile_main_scope(compiler *compiler, parsing_node *node) {
    parsing_node *current=node;
    instructions_block *block=new_instructions_block();
    instructions_block *start=block;
    while (current!=NULL) {
        switch (current->type) {
            case OPENING_SCOPE_NODE:
                compile_scope(compiler,block, current);
                break;
            default:
                compile_instruction(compiler,block, current);
                break;
        }
        block=last_instruction(block);
        current=current->next;
    }
    add_instructions_block(compiler->instructions_blocks_list, *start);
}

void compile_scope(compiler *compiler,instructions_block *block, parsing_node *node) {
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
        block=last_instruction(block);
        current=current->next;
    }
    remove_from_str_size_t_hash_map_if_value_greater_than(compiler->variables, original_index-1);
}


void compile_const(compiler *compiler, instructions_block *block, parsing_node*node) {
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

void compile_variable(compiler *compiler, instructions_block *block, parsing_node *node) {
    const size_t *index = get_from_str_size_t_hash_map(compiler->variables, node->string_val);
    if (index!=NULL) {
        instruction res= {.type = OLOAD_INSTRUCTION , .operand1 = *index };
        add_instruction(block->instructions, res);
    }
    else {
        // to complete
    }
}
void compile_primary(compiler *compiler, instructions_block *block, parsing_node *node) {
    if (node->type==VARIABLE_NODE) {
        compile_variable(compiler, block, node);
    }
    else {
        compile_const(compiler, block, node);
    }
}


void compile_expression(compiler *compiler, instructions_block *block, parsing_node *node) {
    if (node==NULL) {return;}

    switch (node->type) {
        case BINARY_NODE:
            if (!is_logical_node(node)) {
                compile_binary(compiler, block, node);
            }
            else {
                instructions_block *b_true = new_boolean_block_push(true);
                instructions_block *b_false = new_boolean_block_push(false);

                b_false->next=b_true;


                compile_logical_expression(compiler, block, node, b_true, b_false);


            }
            break;
        default:
            compile_primary(compiler, block, node);
            break;
    }
}


HASH_MAP(parsing_node*, instructions_block*, p_node, instructions)




void compile_single_condition(compiler *compiler, instructions_block *block, parsing_node *node, bool inverse_condition) {
    instruction comparison= {0};

    if (!is_comparison_node(node)) {
        compile_primary(compiler, block, node);
        add_instruction(block->instructions, (instruction) {.type = ICONST_INSTRUCTION, .operand1 = 0});
        comparison.type= (inverse_condition) ? IF_CMPEQ : IF_CMPNE;
        add_instruction(block->instructions, comparison);
    }
    else {
        compile_expression(compiler, block, node->left);

        block=last_instruction(block);


        compile_expression(compiler, block, node->right);

        block=last_instruction(block);
        int operator = (inverse_condition) ? inverse_comparison_operator(node->operation) : node->operation;

        int type = comparison_operator_to_instruction_type(operator);
        comparison.type = type;
        add_instruction(block->instructions, comparison);
    }

}

instructions_block *map_conditions_instructions_block(p_node_instructions_hash_map *map, compiler *compiler, parsing_node *current , p_node_jump_hash_map *jumps, instructions_block *true_block, instructions_block *false_block) {
    instructions_block *result = new_instructions_block();

    const jump_infos *infos_ptr =  get_from_p_node_jump_hash_map(jumps, current);
    if (infos_ptr==NULL) {
        display_node(current);
    }
    jump_infos infos = *infos_ptr;


    compile_single_condition(compiler, result, current, !infos.jump_if);
    parsing_node *next=infos.next_node;
    if (next==NULL) {
        result->next=(infos.jump_if) ? false_block : true_block;
    }
    else {
        result->next=map_conditions_instructions_block(map, compiler, next, jumps, true_block, false_block);

    }

    parsing_node *jump = infos.jump_node;

    if (jump==NULL) {
        result->jump=(infos.jump_if) ? true_block : false_block;
    }
    else {
        const instructions_block **result_ptr = get_from_p_node_instructions_hash_map(map, jump);
        result->jump = (instructions_block *) *result_ptr;
    }

    put_to_p_node_instructions_hash_map(map, current, result);

    return result;






}

void print_node_instruction(parsing_node *node , instructions_block *block) {
    printf("node : ");
    display_node(node);

    printf("\ninstructions :\n");
    print_instruction_block(*block);

    printf("\n");
}

void compile_logical_expression(compiler *compiler, instructions_block *block, parsing_node *node, instructions_block *true_block, instructions_block *false_block)  {

    p_node_jump_hash_map *jumps = map_condition_jumps(node);
    p_node_instructions_hash_map *map_node_instructions_block = new_p_node_instructions_hash_map(hash_node_addr,equals_node_addr, print_node_instruction);

    parsing_node *most_left = node;
    while (  is_logical_node(most_left->left) ) {
        most_left=most_left->left;
    }
    most_left=most_left->left;
    block->next=map_conditions_instructions_block(map_node_instructions_block,compiler, most_left, jumps, true_block, false_block);

}

void compile_binary(compiler * compiler , instructions_block *block, parsing_node *node) {
    compile_expression(compiler,block, node->left);
    block=last_instruction(block);
    compile_expression(compiler,block, node->right);
    block=last_instruction(block);
    int instruction_type = binary_node_to_instruction_type(node);
    add_instruction(block->instructions, (instruction) {.type = instruction_type});
}




void compile_affectation(compiler *compiler, instructions_block *block,  parsing_node *node) {
    instructions_block *begin_block =block;
    str_size_t_hash_map *variables = compiler->variables;
    compile_expression(compiler, block, node->right);
    block=last_instruction(block);
    if (block!=begin_block) {
        block->next=new_instructions_block();
        block=block->next;
    }
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
    add_instruction(block->instructions, (instruction) {.type = OSTORE_INSTRUCTION, .operand1 = res_index });
}



void compile_instruction(compiler * compiler, instructions_block *block, parsing_node * node) {
    switch (node->type) {
        case AFFECTATION_NODE:
            compile_affectation(compiler, block, node);
            break;
    }
}










