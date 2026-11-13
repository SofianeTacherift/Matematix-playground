//
// Created by sofiane on 09/09/2026.
//



#ifndef SPDS_HASH_SET_H
#define SPDS_HASH_SET_H


#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <spds/hash_utils.h>


#define BASE_HASH_SET_CAPACITY 16;
#define HASH_SET(T, HASH_SET_NAME, CELL_NAME, NAMESPACE) \
\
typedef struct CELL_NAME {\
    T value; \
    struct CELL_NAME *next;\
}  CELL_NAME;\
\
\
static CELL_NAME * CELL_NAME##_new(T element) { \
    CELL_NAME *result = calloc(1, sizeof(CELL_NAME));\
    if (result!=NULL) {result->value=element;}\
    return result;\
}\
\
\
typedef struct HASH_SET_NAME { \
    size_t size;   \
    size_t capacity; \
    CELL_NAME ** buckets;\
    bool (*equals_function) (const T , const T ) ;\
    long (*hash_function) (const T);\
} HASH_SET_NAME;  \
\
\
 static void NAMESPACE##_init(HASH_SET_NAME * set, long (*hash) (const T), bool (*equals) (const T,const T)) { \
       if (set==NULL) {return;}\
            set->capacity= BASE_HASH_SET_CAPACITY;\
            set->size=0;\
            set->equals_function=equals;\
            set->hash_function=hash;\
            set->buckets=calloc(set->capacity, sizeof( CELL_NAME *) );\
        }\
\
\
\
static HASH_SET_NAME * NAMESPACE##_new( long (*hash) (const T), bool (*equals) (const T,const T), void (*print) (const T) ) {\
        HASH_SET_NAME *result = malloc( sizeof(HASH_SET_NAME));\
        NAMESPACE##_init(result, hash, equals);\
        return result;\
}\
\
\
static void NAMESPACE##_deinit( HASH_SET_NAME *set) {\
    for (size_t i=0; i<set->capacity; i++) {\
            CELL_NAME *current_index = set->buckets[i];\
            while (current_index!=NULL) {\
                CELL_NAME *to_free=current_index;\
                current_index=current_index->next;\
                free(to_free);\
            }\
    }\
    set->capacity=BASE_HASH_SET_CAPACITY;\
    set->size=0;\
    set->hash_function=NULL;\
    set->equals_function=NULL;\
}\
\
\
\
\
static void NAMESPACE##_free (HASH_SET_NAME *set) {\
    for (size_t i=0; i<set->capacity; i++) {\
        CELL_NAME *current_index = set->buckets[i];\
        while (current_index!=NULL) {\
            CELL_NAME *to_free=current_index;\
            current_index=current_index->next;\
            free(to_free);\
        }\
    }\
    free(set);\
}\
static void NAMESPACE##_double_capacity(HASH_SET_NAME *set) {\
    size_t old_capacity=set->capacity;\
    set->capacity*=2;\
    set->buckets=realloc(set->buckets ,set->capacity * sizeof(CELL_NAME *)); \
    for (size_t i=old_capacity; i<set->capacity; i++) {\
        set->buckets[i]=NULL;\
    }\
    CELL_NAME *sentinel = malloc(sizeof(CELL_NAME)); \
    for (size_t i=0; i<old_capacity; i++) {\
        sentinel->next=set->buckets[i];\
        CELL_NAME *current_index = sentinel;\
        while (current_index->next!=NULL ) {\
            size_t new_index = set->hash_function(current_index->next->value) % set->capacity;\
            if (new_index!=i) {\
                CELL_NAME *to_move = current_index->next;\
                current_index->next=to_move->next;\
                if (to_move==set->buckets[i]) {\
                    set->buckets[i]=to_move->next;\
                }\
                to_move->next=set->buckets[new_index];\
                set->buckets[new_index]=to_move;\
            }\
            if (current_index->next!=NULL) {\
            current_index=current_index->next;\
            }\
        }\
    }\
    free(sentinel);\
    }\
\
\
\
static bool NAMESPACE##_add( HASH_SET_NAME *set, T element) { \
    long hash = set->hash_function(element);        \
    size_t index = hash % set->capacity;\
    CELL_NAME *result = NULL; \
    if (set->buckets[index]==NULL) {\
        result=CELL_NAME##_new(element); \
        set->buckets[index]=result;\
    }\
    else if (set->equals_function(set->buckets[index]->value, element)) {return false;} \
    else {\
        CELL_NAME *current_index=set->buckets[index];\
        while (current_index->next!=NULL && !set->equals_function(element, current_index->next->value)) {current_index=current_index->next;}\
        if (current_index->next==NULL) {\
            result=CELL_NAME##_new(element);\
            current_index->next=result;\
        }\
    }\
    if (result!=NULL) {set->size++;}\
    \
    if ( needs_resize(set->size, set->capacity) ) { NAMESPACE##_double_capacity(set);} \
    return result!=NULL;\
}\
\
static bool NAMESPACE##_remove(HASH_SET_NAME *set, T element) {\
    long hash = set->hash_function(element);\
    size_t index = hash % set->capacity;\
    CELL_NAME *removed=NULL; \
    CELL_NAME * head = set->buckets[index];\
    if (head==NULL) {return false;}\
    if (head!=NULL && set->equals_function(head->value, element)) {\
        removed=head;\
        set->buckets[index]=removed->next;\
    }\
    else {\
        CELL_NAME *current_index = head;\
        while (current_index->next!=NULL && !set->equals_function(current_index->next->value, element)) {current_index=current_index->next;}\
        if (current_index->next!=NULL) {\
            removed=current_index->next;\
            current_index->next=removed->next;\
        } \
    }\
    if (removed!=NULL) {set->size--; free(removed);}\
    return removed==NULL;\
}\
\
static bool NAMESPACE##_contains (HASH_SET_NAME *set, T element)  {\
    long hash = set->hash_function(element);\
    size_t index = hash % set->capacity;\
    CELL_NAME * current_index = set->buckets[index];\
    while (current_index!=NULL && current_index->value!=element) {current_index=current_index->next;}\
    return current_index!=NULL;\
}\
\
\
static void NAMESPACE##_print(HASH_SET_NAME *set, void (*print_function) (const T)) {\
    int c=0;   \
    if (print_function==NULL) {printf("{ NULL_PRINT_ELEMENT_FUNCTION }\n");}\
    else {\
       printf("%s[ capacity: %zu - size: %zu - elements : { ", #HASH_SET_NAME ,set->capacity, set->size);\
       for (size_t i=0; i<set->capacity && c<set->size; i++ ) {\
            CELL_NAME *current_index = set->buckets[i];\
            while (current_index!=NULL) {\
                c++;\
                print_function(current_index->value); \
                if (c==set->size) {printf(" ");break;}\
                if (c<set->size) {\
                    printf(" , "); \
                } \
                else {\
                    printf(" ");\
                }\
                \
                \
                current_index=current_index->next;\
            }\
        }\
        printf("}]");\
    }\
}\
\
void NAMESPACE##_println(HASH_SET_NAME *set, void (print_function) (const T) ) {\
    NAMESPACE##_print(set, print_function);\
    printf("\n");\
}\
\


#endif //MATEMATIX_HASH_SET_H


