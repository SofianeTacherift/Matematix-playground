//
// Created by sofiane on 11/09/2026.
//


#ifndef MATEMATIX_HASH_TABLE_H
#define MATEMATIX_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ENTRY_NAME(K,V) K##_##V##_entry
#define HASH_MAP_NAME(K, V) K##_##V##_hash_map

#define NEW_ENTRY(K_ALIAS, V_ALIAS, KEY,VALUE) new_##K_ALIAS##_##V_ALIAS##_entry(KEY,VALUE)

#define CONCAT(X, Y) X##Y
#define CONCAT_MACROS(A,B) CONCAT(A,B)



#define HASH_MAP(K, V, K_ALIAS, V_ALIAS) \
    \
    \
    typedef struct ENTRY_NAME(K_ALIAS, V_ALIAS) {\
        K key;\
        V value;\
        struct ENTRY_NAME(K_ALIAS, V_ALIAS) *next;\
    } ENTRY_NAME(K_ALIAS, V_ALIAS);\
    \
    static ENTRY_NAME(K_ALIAS, V_ALIAS) *new_##K_ALIAS##_##V_ALIAS##_entry(K key, V value) {\
        ENTRY_NAME(K_ALIAS, V_ALIAS) *res = calloc(1, sizeof ( ENTRY_NAME(K_ALIAS, V_ALIAS) ));\
        \
        if (res!=NULL) {\
        res->key=key;\
        res->value=value;\
        }\
        return res;\
    }\
    \
    \
    \
    \
    \
    \
    typedef struct HASH_MAP_NAME(K_ALIAS, V_ALIAS) {\
        size_t capacity;\
        size_t size;\
        ENTRY_NAME(K_ALIAS, V_ALIAS) **buckets;\
        long (*hash_function) (K);\
        bool (*equals_function) (K, K);\
        void (*print_function) (K,V);\
    } HASH_MAP_NAME(K_ALIAS, V_ALIAS);\
    \
    \
static size_t CONCAT_MACROS( CONCAT_MACROS(remove_from_,HASH_MAP_NAME(K_ALIAS, V_ALIAS)), _if_condition) (HASH_MAP_NAME(K_ALIAS, V_ALIAS) *map,  bool (*condition_function) (K, V) ) {\
size_t original_size=map->size;\
size_t removed_count=0;\
for (size_t i=0 ; i<map->capacity; i++) {\
ENTRY_NAME(K_ALIAS, V_ALIAS) *head=map->buckets[i];\
while (head!=NULL && condition_function(head->key, head->value) ) {\
map->buckets[i]=head->next;\
free(head);\
head=map->buckets[i];\
map->size--;\
removed_count++;\
}\
if (head!=NULL) {\
ENTRY_NAME(K_ALIAS, V_ALIAS) *current=head;\
while (current->next!=NULL) {\
ENTRY_NAME(K_ALIAS, V_ALIAS) *next=current->next;\
if (condition_function(head->next->key, head->next->value)) {\
current->next=current->next->next;\
free(next);\
map->size--;\
removed_count++;\
}\
else {\
current=current->next;\
}\
}\
}\
}\
return removed_count;\
}\
    static void CONCAT_MACROS(put_to_, HASH_MAP_NAME(K_ALIAS, V_ALIAS)) ( HASH_MAP_NAME(K_ALIAS, V_ALIAS) *map, K key, V value);\
    static void double_##K_ALIAS##_##V_ALIAS##_hash_map_capacity( HASH_MAP_NAME(K_ALIAS, V_ALIAS) *);\
    \
    \
    static HASH_MAP_NAME(K_ALIAS,V_ALIAS) *CONCAT_MACROS(new_,HASH_MAP_NAME(K_ALIAS, V_ALIAS)) ( long (*hash_func) (K), bool (*equals_func) (K,K), void (*print_func) (K,V)) {\
        HASH_MAP_NAME(K_ALIAS, V_ALIAS) *res=calloc(1, sizeof (HASH_MAP_NAME(K_ALIAS, V_ALIAS)));\
        if (res!=NULL) {\
            res->capacity=16;\
            res->hash_function=hash_func;\
            res->equals_function=equals_func;\
            res->print_function=print_func;\
            res->buckets=calloc(res->capacity, sizeof (ENTRY_NAME(K_ALIAS, V_ALIAS)*));\
        }\
        return res;\
    }\
    \
    static void CONCAT_MACROS(put_to_, HASH_MAP_NAME(K_ALIAS, V_ALIAS)) ( HASH_MAP_NAME(K_ALIAS, V_ALIAS) *map, K key, V value) { \
        long hash=map->hash_function(key);\
        size_t index=hash%map->capacity;\
        if (map->buckets[index]==NULL) {\
            map->buckets[index]=NEW_ENTRY(K_ALIAS, V_ALIAS, key, value);\
            map->size++;\
        }\
        else if (map->equals_function(map->buckets[index]->key, key ) ) {map->buckets[index]->value=value;}\
        else {\
            ENTRY_NAME(K_ALIAS,V_ALIAS) *current=map->buckets[index];\
            while (current->next !=NULL && !map->equals_function(key, current->key) ) {\
                current=current->next;\
            }\
            if (map->equals_function(key, current->key)) {current->key=key;}\
            else {\
                current->next=NEW_ENTRY(K_ALIAS,V_ALIAS,key,value);\
                map->size++;\
                }\
        }\
        \
        bool resize_needed=false;\
        size_t capacity=map->capacity;\
        size_t size = map->size;\
        if ( (( size >> ((sizeof(size_t)) -2) ) & 3L) == 0L) {\
            bool no_capacity_overflow = ( (capacity >> (sizeof(size_t) -1) ) & 1L ) == 0;\
            resize_needed= no_capacity_overflow && (size << 2)/capacity >= 3; \
            }\
        \
        if ( resize_needed ) {double_##K_ALIAS##_##V_ALIAS##_hash_map_capacity(map);} \
    }\
    \
    static const V *CONCAT_MACROS(get_from_, HASH_MAP_NAME(K_ALIAS, V_ALIAS))  (HASH_MAP_NAME(K_ALIAS, V_ALIAS) *map, K key) {\
        long hash = map->hash_function(key);\
        size_t index = hash % map->capacity;\
        ENTRY_NAME(K_ALIAS, V_ALIAS) *current = map->buckets[index];\
        while (current!=NULL && !map->equals_function(key, current->key)) {\
            current=current->next;\
        }\
        return (current==NULL) ? NULL : (const V*) (&current->value);\
    }\
    \
    \
    static const ENTRY_NAME(K_ALIAS, V_ALIAS) *CONCAT_MACROS(get_entry_from_, HASH_MAP_NAME(K_ALIAS, V_ALIAS))  (HASH_MAP_NAME(K_ALIAS, V_ALIAS) *map, K key) {\
        long hash = map->hash_function(key);\
        size_t index = hash % map->capacity;\
        ENTRY_NAME(K_ALIAS, V_ALIAS) *current = map->buckets[index];\
        while (current!=NULL && !map->equals_function(key, current->key)) {\
            current=current->next;\
        }\
        return (current==NULL) ? NULL : (const ENTRY_NAME(K_ALIAS, V_ALIAS)*) (&current);\
    }\
    \
    static bool CONCAT_MACROS(remove_from_, HASH_MAP_NAME(K_ALIAS, V_ALIAS)) (HASH_MAP_NAME(K_ALIAS, V_ALIAS) *map, K key) {\
        long hash = map->hash_function(key);\
        size_t index = hash % map->capacity;\
        ENTRY_NAME(K_ALIAS, V_ALIAS) *head = map->buckets[index];\
        ENTRY_NAME(K_ALIAS, V_ALIAS) *removed=NULL;\
        if (head==NULL) {\
            return false;\
        }\
        else if (map->equals_function(head->key, key)) {\
            map->buckets[index]=head->next;\
            removed=head;\
        }\
        else {\
            ENTRY_NAME(K_ALIAS, V_ALIAS) *current = head;\
            while (current->next!=NULL && !map->equals_function(current->next->key, key)) {\
                current=current->next;\
            }\
            if (current->next!=NULL) {\
                current->next=current->next->next;\
                removed=current->next;\
            }\
        }\
        if (removed!=NULL) {\
            free(removed);\
            map->size--;\
            return true;\
        }\
        return false;\
    }\
    \
    \
    \
    static void double_##K_ALIAS##_##V_ALIAS##_hash_map_capacity( HASH_MAP_NAME(K_ALIAS, V_ALIAS) *map) {\
        size_t old_capacity=map->capacity;\
        map->capacity*=2;\
        map->buckets=realloc(map->buckets, map->capacity * sizeof(ENTRY_NAME(K_ALIAS, V_ALIAS)*));\
        memset(map->buckets+old_capacity, 0, map->capacity - old_capacity);\
        ENTRY_NAME(K_ALIAS, V_ALIAS) *sentinel=calloc(1, sizeof( ENTRY_NAME(K_ALIAS, V_ALIAS)));\
        for (size_t i =0 ; i<old_capacity; i++) {\
            sentinel->next = map->buckets[i];\
            ENTRY_NAME(K_ALIAS, V_ALIAS) *current=sentinel;\
            while (current->next!=NULL) {\
                ENTRY_NAME(K_ALIAS, V_ALIAS) *next_entry = current->next;\
                size_t index = map->hash_function(next_entry->key) % map->capacity;\
                if (index!=i) {\
                    if (next_entry==map->buckets[i]) {\
                        map->buckets[i]=next_entry->next;\
                    }\
                    current->next=next_entry->next;\
                    next_entry->next=map->buckets[index];\
                    map->buckets[index]=next_entry;\
                }\
                else if (current->next!=NULL) {\
                    current=current->next;\
                }\
            }\
            sentinel->next=NULL;\
        }\
        free(sentinel);\
    }\
    \
    static void CONCAT_MACROS(print_, HASH_MAP_NAME(K_ALIAS, V_ALIAS)) (HASH_MAP_NAME(K_ALIAS, V_ALIAS) *map) {\
        printf("hash_table[ capacity : %lu - size : %lu  { ", map->capacity, map->size);\
        size_t count=0;\
        for (size_t i=0; count<map->size && i<map->capacity; i++) {\
            ENTRY_NAME(K_ALIAS, V_ALIAS) *current = map->buckets[i];\
            while (count<map->size && current!=NULL) {\
                count++;\
                map->print_function(current->key, current->value);\
                printf(" ");\
                if (count<map->size) {printf(" - ")  ;}\
                \
                \
                current=current->next;\
            }\
        }\
        \
        printf("} ]\n");\
    }\
    \
    static void CONCAT_MACROS(free_, HASH_MAP_NAME(K_ALIAS, V_ALIAS) ) ( HASH_MAP_NAME(K_ALIAS, V_ALIAS) * map ){\
        for (size_t i=0; i<map->capacity ; i++) {\
            ENTRY_NAME(K_ALIAS, V_ALIAS) *current = map->buckets[i];\
            while (current!=NULL) {\
                ENTRY_NAME(K_ALIAS, V_ALIAS) *next=current->next;\
                free(current);\
                current=current->next;\
            }\
        }\
        free(map);\
    }\
    \
    \
    \
    \
    \
    \
    \
    \
    \



#endif //MATEMATIX_HASH_TABLE_H
