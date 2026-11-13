//
// Created by userdepinfo on 12/11/26.
//

#ifndef PARSING_NODE_LINKED_LIST_H
#define PARSING_NODE_LINKED_LIST_H

#include "parsing_node.h"

typedef struct ParsingNodeLinkedList {
    ParsingNode* head;
    ParsingNode* end;
} ParsingNodeLinkedList;

ParsingNodeLinkedList* ParsingNodeLinkedList_new(void);
void ParsingNodeLinkedList_add(ParsingNodeLinkedList* list, ParsingNode* node);
inline bool ParsingNodeLinkedList_is_empty(ParsingNodeLinkedList* list);
void ParsingNodeLinkedList_merge(ParsingNodeLinkedList* list, ParsingNodeLinkedList* to_add);
void ParsingNodeLinkedList_free(ParsingNodeLinkedList*);

#endif //PARSING_NODE_LINKED_LIST_H
