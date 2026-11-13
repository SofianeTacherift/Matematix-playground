//
// Created by sofiane on 11/09/2026.
//


#ifndef SPDS_HASH_MAP_H
#define SPDS_HASH_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <spds/hash_utils.h>

#define HASH_MAP(K, V, HASH_MAP_NAME, ENTRY_NAME, NAMESPACE) \
    \
    \
    typedef struct ENTRY_NAME {\
        K key;\
        V value;\
        struct ENTRY_NAME *next;\
    } ENTRY_NAME;\
    \
    static ENTRY_NAME * ENTRY_NAME##_new(K key, V value) {\
        ENTRY_NAME *res = calloc(1, sizeof ( ENTRY_NAME ));\
        \
        if (res!=NULL) {\
        res->key=key;\
        res->value=value;\
        res->next=NULL;\
        }\
        return res;\
    }\
    \
    \
    \
    \
    \
    typedef struct HASH_MAP_NAME {\
        size_t capacity;\
        size_t size;\
        ENTRY_NAME **buckets;\
        long (*hash_function) ( const K);\
        bool (*equals_function) ( const K, const K);\
    } HASH_MAP_NAME;\
    \
    \
    \
    static void NAMESPACE##_init(HASH_MAP_NAME *map, long ( *hash_func) (const  K),  bool ( *equals_func) ( const K, const K) ) { \
        if (map==NULL) return;\
        map->capacity=16;\
        map->size=0;\
        map->hash_function=hash_func;\
        map->equals_function=equals_func;\
        map->buckets=calloc(map->capacity, sizeof (ENTRY_NAME*));\
    }\
    \
    \
    static HASH_MAP_NAME * NAMESPACE##_new ( long ( *hash_func) ( const K),  bool (*equals_func) ( const K, const K)) {\
        HASH_MAP_NAME *res=calloc(1, sizeof (HASH_MAP_NAME));\
        NAMESPACE##_init(res, hash_func, equals_func);\
        return res;\
    }\
    \
    \
    static void NAMESPACE##_deinit(HASH_MAP_NAME *map){\
        for (size_t i=0; i<map->capacity ; i++) {\
            ENTRY_NAME *current = map->buckets[i];\
            while (current!=NULL) {\
                ENTRY_NAME *next=current->next;\
                free(current);\
                current=next;\
            }\
        }\
        free(map->buckets);\
        map->capacity=0;\
        map->size=0;\
    }\
    static void NAMESPACE##_free(HASH_MAP_NAME *map){\
        NAMESPACE##_deinit(map);\
        free(map);\
    }\
    static void NAMESPACE##_put ( HASH_MAP_NAME *map, K key, V value);\
    static void NAMESPACE##_double_capacity( HASH_MAP_NAME *);\
    \
    \
    \
    static void NAMESPACE##_put ( HASH_MAP_NAME *map, K key, V value) { \
        long hash=map->hash_function(key);\
        size_t index=hash%map->capacity;\
        if (map->buckets[index]==NULL) {\
            map->buckets[index]=ENTRY_NAME##_new(key,value);\
            map->size++;\
        }\
        else if (map->equals_function(map->buckets[index]->key, key ) ) {map->buckets[index]->value=value;}\
        else {\
            ENTRY_NAME *current_index=map->buckets[index];\
            while (current_index->next !=NULL && !map->equals_function(key, current_index->key) ) {\
                current_index=current_index->next;\
            }\
            if (map->equals_function(key, current_index->key)) {current_index->key=key;}\
            else {\
                current_index->next=ENTRY_NAME##_new(key, value);\
                map->size++;\
                }\
        }\
        \
        if ( needs_resize(map->size, map->capacity) ) {NAMESPACE##_double_capacity(map);} \
    }\
    \
    static const V * NAMESPACE##_get  (HASH_MAP_NAME *map, K key) {\
        long hash = map->hash_function(key);\
        size_t index = hash % map->capacity;\
        ENTRY_NAME *current_index = map->buckets[index];\
        while (current_index!=NULL && !map->equals_function(key, current_index->key)) {\
            current_index=current_index->next;\
        }\
        return (current_index==NULL) ? NULL : (const V*) (&current_index->value);\
    }\
    \
    static const V NAMESPACE##_get_or_default(HASH_MAP_NAME *map, K key, V default_value) {\
        long hash = map->hash_function(key);\
        size_t index = hash % map->capacity;\
        ENTRY_NAME *current_index = map->buckets[index];\
        while (current_index!=NULL && !map->equals_function(key, current_index->key)) {\
        current_index=current_index->next;\
        }\
        return (current_index==NULL) ? default_value : (const V) (current_index->value);\
    }\
    \
    static const ENTRY_NAME * NAMESPACE##_get_entry  (HASH_MAP_NAME *map, K key) {\
        long hash = map->hash_function(key);\
        size_t index = hash % map->capacity;\
        ENTRY_NAME *current_index = map->buckets[index];\
        while (current_index!=NULL && !map->equals_function(key, current_index->key)) {\
            current_index=current_index->next;\
        }\
        return (current_index==NULL) ? NULL : (const ENTRY_NAME*) (&current_index);\
    }\
    \
    static bool NAMESPACE##_remove (HASH_MAP_NAME *map, K key) {\
        long hash = map->hash_function(key);\
        size_t index = hash % map->capacity;\
        ENTRY_NAME *head = map->buckets[index];\
        ENTRY_NAME *removed=NULL;\
        if (head==NULL) {\
            return false;\
        }\
        else if (map->equals_function(head->key, key)) {\
            map->buckets[index]=head->next;\
            removed=head;\
        }\
        else {\
            ENTRY_NAME *current_index = head;\
            while (current_index->next!=NULL && !map->equals_function(current_index->next->key, key)) {\
                current_index=current_index->next;\
            }\
            if (current_index->next!=NULL) {\
                current_index->next=current_index->next->next;\
                removed=current_index->next;\
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
    static void NAMESPACE##_double_capacity( HASH_MAP_NAME *map) {\
        size_t old_capacity=map->capacity;\
        map->capacity*=2;\
        map->buckets=realloc(map->buckets, map->capacity * sizeof(ENTRY_NAME*));\
        memset(map->buckets+old_capacity, 0, (map->capacity - old_capacity) * sizeof(ENTRY_NAME *)  ) ;\
        ENTRY_NAME *sentinel=calloc(1, sizeof( ENTRY_NAME));\
        for (size_t i =0 ; i<old_capacity; i++) {\
            sentinel->next = map->buckets[i];\
            ENTRY_NAME *current_entry=sentinel;\
            while (current_entry->next!=NULL) {\
                ENTRY_NAME *next_entry = current_entry->next;\
                size_t index = map->hash_function(next_entry->key) % map->capacity;\
                if (index!=i) {\
                    if (next_entry==map->buckets[i]) {\
                        map->buckets[i]=next_entry->next;\
                    }\
                    current_entry->next=next_entry->next;\
                    next_entry->next=map->buckets[index];\
                    map->buckets[index]=next_entry;\
                }\
                else if (current_entry->next!=NULL) {\
                    current_entry=current_entry->next;\
                }\
            }\
            sentinel->next=NULL;\
        }\
        free(sentinel);\
    }\
    \
    static void NAMESPACE##_print( HASH_MAP_NAME *map,  void (*print_function) (const ENTRY_NAME*) ) {\
        printf("%s[ capacity : %lu - size : %lu  { ", #HASH_MAP_NAME, map->capacity, map->size);\
        size_t count=0;\
        for (size_t i=0; count<map->size && i<map->capacity; i++) {\
            ENTRY_NAME *current_index = map->buckets[i];\
            while (count<map->size && current_index!=NULL) {\
                count++;\
                print_function(current_index);\
                printf(" ");\
                if (count<map->size) {printf(" - ")  ;}\
                \
                \
                current_index=current_index->next;\
            }\
        }\
        \
        printf("} ]");\
    }\
    \
    static void NAMESPACE##_println(HASH_MAP_NAME *map,void (*print_function) (const ENTRY_NAME*)) { \
        NAMESPACE##_print(map, print_function);\
        printf("\n");\
    }\
    \
    \
    \
    \
    \

#endif
//MATEMATIX_HASH_TABLE_H

