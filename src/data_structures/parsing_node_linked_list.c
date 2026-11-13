//
// Created by userdepinfo on 12/11/26.
//

#include "parsing_node_linked_list.h"
#include "stdlib.h"
#include "stdio.h"

ParsingNodeLinkedList* ParsingNodeLinkedList_new() {
    ParsingNodeLinkedList* result = calloc(1, sizeof(ParsingNodeLinkedList));
    result->head = NULL;
    result->end = NULL;
    return result;
}

inline bool ParsingNodeLinkedList_is_empty(ParsingNodeLinkedList* list) {
    return list->head == NULL && list->end == NULL;
}

void ParsingNodeLinkedList_add(ParsingNodeLinkedList* list, ParsingNode* node) {
    if (node == NULL) {
        return;
    }
    if (ParsingNodeLinkedList_is_empty(list)) {
        list->head = node;
        list->end = node;
    }
    else {
        list->end->next = node;
        node->previous = list->end;
        list->end = node;
    }
}

void ParsingNodeLinkedList_merge(ParsingNodeLinkedList* list, ParsingNodeLinkedList* to_add) {
    if (list == NULL || to_add == NULL || (to_add->head == NULL && to_add->end == NULL)) {
        return;
    }

    if (ParsingNodeLinkedList_is_empty(list)) {
        list->head = to_add->head;
        list->end = to_add->end;
    }
    else {
        list->end->next = to_add->head;
        list->end = to_add->end;
    }
}

void ParsingNodeLinkedList_free(ParsingNodeLinkedList* list) {
    ParsingNode* current = list->head;
    while (current != NULL) {
        ParsingNode* to_free = current;
        current = current->next;
        ParsingNode_free_recursive(to_free, false);
    }
    free(list);
}

void print_parsing_node_linked_list(ParsingNodeLinkedList* list) {
    ParsingNode* current = list->head;
    while (current != NULL) {
        ParsingNode_print(current);
        current = current->next;
        if (current != NULL) {
            printf("  ");
        }
    }
    printf("\n");
}
