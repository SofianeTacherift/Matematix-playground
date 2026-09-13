//
// Created by sofiane on 20/06/2026.
//

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define max( x, y) ((x)>(y)) ? x : y; 

#define list_type_name(T) T##_array_list

#define ARRAY_LIST(T, TYPE_ALIAS) \
    \
    typedef struct list_type_name(TYPE_ALIAS) { \
    int size; \
    int capacity; \
    T* elements; \
    } list_type_name(TYPE_ALIAS); \
    \
    static void init_##TYPE_ALIAS##_list(list_type_name(TYPE_ALIAS)*  list ) { \
    list->size=0;  \
    list->elements=malloc(sizeof(T)); \
    list->capacity=1; \
    }\
    \
    static list_type_name(TYPE_ALIAS)* new_##TYPE_ALIAS##_array_list() { \
        list_type_name(TYPE_ALIAS) *ARRAY_LIST = malloc(sizeof(list_type_name(TYPE_ALIAS))); \
        init_##TYPE_ALIAS##_list(ARRAY_LIST);\
        return ARRAY_LIST; \
    }\
    static void free_##TYPE_ALIAS##_array_list( list_type_name(TYPE_ALIAS) * list) { \
        free(list->elements);\
        free(list); \
    } \
    static void add_##TYPE_ALIAS(list_type_name(TYPE_ALIAS)* list, T element) { \
        int size = list->size;\
        if (size>=list->capacity) { \
            list->elements=realloc(list->elements, 2*list->capacity*sizeof(T));\
            list->capacity*=2; \
        }\
        list->elements[size]=element;\
        list->size++;\
    } \
    \
    static void add_##TYPE_ALIAS##_at(list_type_name(TYPE_ALIAS)*list, T element, int index) {\
        int size = list->size;\
        if (size>=list->capacity) { \
            list->elements=realloc(list->elements, 2*list->capacity*sizeof(T));\
            list->capacity*=2; \
        }\
        T* source=list->elements+index; \
        T* destination=source+1;\
        int copy_size = size-index; \
        memmove(destination, source, copy_size*sizeof(T) );\
        *(list->elements+index)=element; \
        list->size++; \
    }\
    \
    \
    static void pop_##TYPE_ALIAS(list_type_name(TYPE_ALIAS)* list) { \
        list->size = max(list->size-1, 0); \
    } \
    \
    static void delete_##TYPE_ALIAS(list_type_name(TYPE_ALIAS) * list, int index) { \
        if (index>=list->size || index<0) {return;} \
        int size = list->size-(index+1);\
        T* source = list->elements+(index+1); \
        T* destination =list->elements+index;\
        memmove(destination, source, size*sizeof(T)); \
        list->size--; \
    } \
    \
    static void apply_##TYPE_ALIAS##_operation(list_type_name(TYPE_ALIAS)* list, void (*ptr) (T) ) { \
        for (int i=0;i<list->size;i++) {\
            ptr(list->elements[i]);\
        }\
    }\
    \
    static bool TYPE_ALIAS##_array_list_empty(list_type_name(TYPE_ALIAS)* list) {\
        return list->size==0;\
    }\
    \
    static T *last_##TYPE_ALIAS##_from(list_type_name(TYPE_ALIAS) *list) { \
        if (list->size<=0) {\
            return NULL; \
        }\
        return list->elements+(list->size-1);\
    }\

#endif 
