//
// Created by sofiane on 06/09/2026.
//

#include "compiler.h"


#define copy_to_size_t(dest, src, type)     memcpy( ((char*) (&dest)) + (sizeof(size_t) - sizeof(src) ),  &src , sizeof(src));


static long hash_node_addr(parsing_node *node) {
    return  (long) node;
}

static bool equals_node_add(parsing_node *n1, parsing_node *n2) {
    return n1==n2;
}


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

    while (current!=NULL) {
        switch (current->type) {
            case OPENING_SCOPE_NODE:
                compile_scope(compiler,block, current);
                break;
            default:
                compile_instruction(compiler,block, current);
                break;
        }
        current=current->next;
    }
    add_instructions_block(compiler->instructions_blocks_list, *block);
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
    switch (node->type) {
        case BINARY_NODE:
            compile_binary(compiler, block, node);
            break;
        default:
            compile_primary(compiler, block, node);
            break;
    }
}


void compile_binary(compiler * compiler , instructions_block *block, parsing_node *node) {
    compile_expression(compiler,block, node->left);
    compile_expression(compiler,block, node->right);
    int instruction_type = binary_node_to_instruction_type(node);
    add_instruction(block->instructions, (instruction) {.type = instruction_type});
}




void compile_affectation(compiler *compiler, instructions_block *block,  parsing_node *node) {
    str_size_t_hash_map *variables = compiler->variables;
    compile_expression(compiler, block, node->right);
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

void compile_condition(compiler *compiler, parsing_node *node) {
    
}








