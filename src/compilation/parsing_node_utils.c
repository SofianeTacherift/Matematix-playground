//
// Created by userdepinfo on 12/11/26.
//

#include "parsing_node_utils.h"

//HASH_MAP(ParsingNode *, size_t, p_node_size_t_hash_map, p_node_size_t_entry)

HASH_MAP(ParsingNode *, ParsingNode *, PNodePNodeHashMap, PNodePNodeEntry, PNodePNodeHashMap)
HASH_MAP(ParsingNode *, size_t, PNodeSizeTHashMap, PNodeSizeTEntry, PNodeSizeTHashMap)

void map_most_not_logical_node_left(ParsingNode* condition, PNodePNodeHashMap* map);
ParsingNodeJumpHashMap* map_condition_jumps(ParsingNode* root);

void print_jump_infos(JumpInfos jump_inf) {
    printf(" JumpInfos[ jumpif : %s - jump_node : ", jump_inf.jump_if ? "TRUE" : "FALSE");
    ParsingNode_print(jump_inf.jump_node);
    printf(" - next_condition : ");
    ParsingNode_print(jump_inf.next_node);
    printf(" ]\n");
}

void print_node_jump_entry(ParsingNode* node, JumpInfos infos) {
    printf("{");
    ParsingNode_print(node);
    printf(" : ");
    print_jump_infos(infos);
    printf("}");
}

ParsingNode* most_left_non_logical_operand_node(ParsingNode* node, PNodePNodeHashMap* map) {
    if (node == NULL) {
        return NULL;
    }

    if (node->type != BINARY_NODE || (node->operation != LOGICAL_OR_OPERATOR && node->operation !=
        LOGICAL_AND_OPERATOR)) {
        return node;
    }

    ParsingNode* res = most_left_non_logical_operand_node(node->left, map);
    PNodePNodeHashMap_put(map, node, res);
    if (node->right->type == BINARY_NODE && (node->right->operation == LOGICAL_OR_OPERATOR || node->right->operation ==
        LOGICAL_AND_OPERATOR)) {
        most_left_non_logical_operand_node(node->right, map);
    }
    else {
        PNodePNodeHashMap_put(map, node->right, node->right);
    }

    return res;
}

void map_most_not_logical_node_left(ParsingNode* condition, PNodePNodeHashMap* map) {
    most_left_non_logical_operand_node(condition, map);
}

void map_condition_jumps_recursive(ParsingNode* node, ParsingNode* last_or, ParsingNode* last_and, ParsingNode* parent,
                                   PNodePNodeHashMap* most_left_node_map, PNodeSizeTHashMap* levels,
                                   ParsingNodeJumpHashMap* jumps_map) {
    PNodeSizeTHashMap_put(levels, node, *(PNodeSizeTHashMap_get(levels, parent)) + 1);

    if (node->type == BINARY_NODE && (node->operation == LOGICAL_OR_OPERATOR || node->operation ==
        LOGICAL_AND_OPERATOR)) {
        ParsingNode* or = (node->operation == LOGICAL_OR_OPERATOR) ? node : last_or;
        ParsingNode* and = (node->operation == LOGICAL_AND_OPERATOR) ? node : last_and;
        map_condition_jumps_recursive(node->left, or, and, node, most_left_node_map, levels, jumps_map);
        map_condition_jumps_recursive(node->right, last_or, last_and, node, most_left_node_map, levels, jumps_map);
    }
    else {
        bool jump_if = parent == NULL || parent->operation == LOGICAL_OR_OPERATOR;
        // if parent type is logical or, we suppose we will jump if the condition if true
        ParsingNode* last_checkpoint = (jump_if) ? last_and : last_or;
        ParsingNode* next_condition_root = (jump_if) ? last_or : last_and;

        ParsingNode** jump_node_ptr = (ParsingNode**)(last_checkpoint == NULL
                                                          ? NULL
                                                          : PNodePNodeHashMap_get(
                                                              most_left_node_map, last_checkpoint->right));
        ParsingNode* jump_node = jump_node_ptr ? *jump_node_ptr : NULL;

        ParsingNode** next_node = (ParsingNode**)(next_condition_root == NULL
                                                      ? NULL
                                                      : PNodePNodeHashMap_get(
                                                          most_left_node_map, next_condition_root->right));
        ParsingNode* next_condition = (next_condition_root) ? *next_node : NULL;

        JumpInfos infos = {0};
        const size_t* jump_node_level_ptr = PNodeSizeTHashMap_get(levels, last_checkpoint);
        const size_t* next_condition_level_ptr = PNodeSizeTHashMap_get(levels, next_condition_root);
        size_t jump_node_level = jump_node_level_ptr ? *jump_node_level_ptr : 0;
        size_t next_condition_level = next_condition_level_ptr ? *next_condition_level_ptr : 0;
        if (jump_node_level < next_condition_level || (jump_node_level == next_condition_level && jump_if)) {
            infos = (JumpInfos){.jump_if = jump_if, .jump_node = jump_node, .next_node = next_condition};
            // However sometimes, it is better to jump if the condition is false for a logical or
        }
        else {
            infos = (JumpInfos){.jump_if = !jump_if, .jump_node = next_condition, .next_node = jump_node};
        }
        ParsingNodeJumpHashMap_put(jumps_map, node, infos);
    }
}

ParsingNodeJumpHashMap* map_condition_jumps(ParsingNode* root) {
    PNodePNodeHashMap* most_left = PNodePNodeHashMap_new(ParsingNode_hash_address, ParsingNode_equals_address);
    map_most_not_logical_node_left(root, most_left);
    ParsingNodeJumpHashMap* res = ParsingNodeJumpHashMap_new(ParsingNode_hash_address, ParsingNode_equals_address);
    PNodeSizeTHashMap* levels = PNodeSizeTHashMap_new(ParsingNode_hash_address, ParsingNode_equals_address);
    PNodeSizeTHashMap_put(levels, NULL, 0);
    map_condition_jumps_recursive(root, NULL, NULL, NULL, most_left, levels, res);
    PNodePNodeHashMap_free(most_left);
    PNodeSizeTHashMap_free(levels);
    return res;
}
