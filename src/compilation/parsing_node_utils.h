//
// Created by userdepinfo on 12/11/26.
//

#ifndef PARSING_NODE_UTILS_H
#define PARSING_NODE_UTILS_H

#include "../data_structures/parsing_node.h"
#include "../../lib/spds/hash_map.h"

typedef struct JumpInfos {
    ParsingNode* jump_node;
    ParsingNode* next_node;
    bool jump_if;
    bool invert_condition;
} JumpInfos;

HASH_MAP(ParsingNode*, JumpInfos, ParsingNodeJumpHashMap, ParsingNodeJumpEntry, ParsingNodeJumpHashMap)

ParsingNodeJumpHashMap* map_condition_jumps(ParsingNode* root);

#endif //PARSING_NODE_UTILS_H
