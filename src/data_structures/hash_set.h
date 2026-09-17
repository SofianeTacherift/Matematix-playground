//
// Created by sofiane on 09/09/2026.
//



#ifndef MATEMATIX_HASH_SET_H
#define MATEMATIX_HASH_SET_H


    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdio.h>

    #define HASH_SET_TYPE_NAME(T) T##_hash_set

    #define CELL_TYPE_NAME(T) T##_cell


    #define create_cell(ELEMENT, TYPE_ALIAS) create_##TYPE_ALIAS##_cell(ELEMENT)



    #define HASH_SET(T, TYPE_ALIAS) \
     \
        typedef struct CELL_TYPE_NAME(TYPE_ALIAS) {\
            T value; \
            struct CELL_TYPE_NAME(TYPE_ALIAS) *next;\
        }  CELL_TYPE_NAME(TYPE_ALIAS);\
        \
        \
        static CELL_TYPE_NAME(TYPE_ALIAS) *create_##TYPE_ALIAS##_cell(T element) { \
        CELL_TYPE_NAME(TYPE_ALIAS) *result = calloc(1, sizeof(CELL_TYPE_NAME(TYPE_ALIAS)));\
        if (result!=NULL) {result->value=element;}\
        return result;\
        }\
        \
        \
        typedef struct HASH_SET_TYPE_NAME(TYPE_ALIAS) { \
        size_t size;   \
        size_t capacity; \
        CELL_TYPE_NAME(TYPE_ALIAS) ** buckets;\
        bool (*equals_function) ( T , T ) ;\
        long (*hash_function) (T);\
        void (*print_function) (T);\
        } HASH_SET_TYPE_NAME(TYPE_ALIAS);  \
        \
        static void print_##TYPE_ALIAS##_hash_set(HASH_SET_TYPE_NAME(TYPE_ALIAS) *set);\
        \
        static HASH_SET_TYPE_NAME(TYPE_ALIAS) *new_##TYPE_ALIAS##_hash_set( long (*hash) (T), bool (*equal) (T,T), void (*print) (T) ) {\
            HASH_SET_TYPE_NAME(TYPE_ALIAS) *result = calloc(1, sizeof(HASH_SET_TYPE_NAME(TYPE_ALIAS)));\
            if (result==NULL) {return NULL;}\
            result->capacity=16;\
            result->equals_function=equal;\
            result->hash_function=hash;\
            result->print_function=print;  \
            result->buckets=calloc(result->capacity, sizeof( CELL_TYPE_NAME(TYPE_ALIAS) *) );\
            return result;\
            }\
        \
        \
        static void double_##TYPE_ALIAS##_hash_set_capacity(HASH_SET_TYPE_NAME(TYPE_ALIAS) *set) {\
            size_t old_capacity=set->capacity;\
            set->capacity*=2;\
            set->buckets=realloc(set->buckets ,set->capacity * sizeof(CELL_TYPE_NAME(TYPE_ALIAS) *)); \
            for (size_t i=old_capacity; i<set->capacity; i++) {\
                set->buckets[i]=NULL;\
            }\
            CELL_TYPE_NAME(TYPE_ALIAS) *sentinel = malloc(sizeof(CELL_TYPE_NAME(TYPE_ALIAS))); \
            for (size_t i=0; i<old_capacity; i++) {\
                sentinel->next=set->buckets[i];\
                CELL_TYPE_NAME(TYPE_ALIAS) *current = sentinel;\
                while (current->next!=NULL ) {\
                    size_t new_index = set->hash_function(current->next->value) % set->capacity;\
                    if (new_index!=i) {\
                        CELL_TYPE_NAME(TYPE_ALIAS) *to_move = current->next;\
                        current->next=to_move->next;\
                        if (to_move==set->buckets[i]) {\
                            set->buckets[i]=to_move->next;\
                        }\
                        to_move->next=set->buckets[new_index];\
                        set->buckets[new_index]=to_move;\
                    }\
                    if (current->next!=NULL) {\
                    current=current->next;\
                    }\
                }\
            }\
            free(sentinel);\
            }\
        \
        \
        \
        static bool add_##TYPE_ALIAS##_to_hash_set( HASH_SET_TYPE_NAME(TYPE_ALIAS) *set, T element) { \
            long hash = set->hash_function(element);        \
            size_t index = hash % set->capacity;\
            CELL_TYPE_NAME(TYPE_ALIAS) *result = NULL; \
            if (set->buckets[index]==NULL) {\
                result=create_cell(element, TYPE_ALIAS); \
                set->buckets[index]=result;\
            }\
            else if (set->equals_function(set->buckets[index]->value, element)) {return false;} \
            else {\
                CELL_TYPE_NAME(TYPE_ALIAS) *current=set->buckets[index];\
                while (current->next!=NULL && !set->equals_function(element, current->next->value)) {current=current->next;}\
                if (current->next==NULL) {\
                    result=create_cell(element, TYPE_ALIAS);\
                    current->next=result;\
                }\
            }\
            if (result!=NULL) {set->size++;}\
            bool resize_needed=false;\
            size_t capacity=set->capacity;\
            size_t size = set->size;\
            if ( (( size >> ((sizeof(size_t)) -2) ) & 3L) == 0L) {\
                bool no_capacity_overflow = ( (capacity >> (sizeof(size_t) -1) ) & 1L ) == 0;\
                resize_needed= no_capacity_overflow && (size << 2)/capacity >= 3; \
            }\
            \
            if ( resize_needed ) {double_##TYPE_ALIAS##_hash_set_capacity(set);} \
            return result!=NULL;\
        }\
        \
        static bool remove_##TYPE_ALIAS##_from_hash_set(HASH_SET_TYPE_NAME(TYPE_ALIAS) *set, T element) {\
            long hash = set->hash_function(element);\
            size_t index = hash % set->capacity;\
            CELL_TYPE_NAME(TYPE_ALIAS) *removed=NULL; \
            CELL_TYPE_NAME(TYPE_ALIAS) * head = set->buckets[index];\
            if (head==NULL) {return false;}\
            if (head!=NULL && set->equals_function(head->value, element)) {\
                removed=head;\
                set->buckets[index]=removed->next;\
            }\
            else {\
                CELL_TYPE_NAME(TYPE_ALIAS) *current = head;\
                while (current->next!=NULL && !set->equals_function(current->next->value, element)) {current=current->next;}\
                if (current->next!=NULL) {\
                    removed=current->next;\
                    current->next=removed->next;\
                } \
            }\
            if (removed!=NULL) {set->size--; free(removed);}\
            return removed==NULL;\
        }\
        \
        static bool hash_set_contains_##TYPE_ALIAS (HASH_SET_TYPE_NAME(TYPE_ALIAS) *set, T element)  {\
            long hash = set->hash_function(element);\
            size_t index = hash % set->capacity;\
            CELL_TYPE_NAME(TYPE_ALIAS) * current = set->buckets[index];\
            while (current!=NULL && current->value!=element) {current=current->next;}\
            return current!=NULL;\
        }\
        \
        \
        static void free_##TYPE_ALIAS##_hash_set(HASH_SET_TYPE_NAME(TYPE_ALIAS) *set) {\
            for (size_t i=0; i<set->capacity; i++) {\
                CELL_TYPE_NAME(TYPE_ALIAS) *current = set->buckets[i];\
                while (current!=NULL) {\
                    CELL_TYPE_NAME(TYPE_ALIAS) *to_free=current;\
                    current=current->next;\
                    free(to_free);\
                }\
            }\
            free(set);\
        }\
        \
        \
        \
        \
        \
        \
        static void print_##TYPE_ALIAS##_hash_set(HASH_SET_TYPE_NAME(TYPE_ALIAS) *set) {\
            int c=0;   \
            if (set->print_function==NULL) {printf("{ NULL_PRINT_ELEMENT_FUNCTION }\n");}\
            else {\
               printf("set[ "); \
               printf("capacity:%zu - size:%zu - { ",set->capacity, set->size);\
               for (size_t i=0; i<set->capacity && c<set->size; i++ ) {\
                    CELL_TYPE_NAME(TYPE_ALIAS) *current = set->buckets[i];\
                    while (current!=NULL) {\
                        c++;\
                        set->print_function(current->value); \
                        if (c==set->size) {printf(" ");break;}\
                        if (c<set->size) {\
                        printf(" , "); \
                        } \
                        else {\
                        printf(" ");\
                        }\
                        \
                        \
                        current=current->next;\
                    }\
                }\
                printf(" }]\n");\
            }\
        }\




#endif //MATEMATIX_HASH_SET_H


