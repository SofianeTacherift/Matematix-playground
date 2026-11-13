//
// Created by sofiane on 20/06/2026.
//

#ifndef SPDS_LIST_H
#define SPDS_LIST_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define max( x, y) ((x)>(y)) ? x : y; 

#define BASE_ARRAY_LIST_CAPACITY 16

#define ARRAY_LIST(T, ARRAY_LIST_NAME, NAMESPACE) \
    \
    typedef struct ARRAY_LIST_NAME { \
    size_t size; \
    size_t capacity; \
    T* elements; \
    } ARRAY_LIST_NAME; \
    \
    static void NAMESPACE##_init(ARRAY_LIST_NAME*  list ) { \
        list->size=0;  \
        list->capacity=16; \
        list->elements=malloc(sizeof(T)*list->capacity); \
    }\
    \
    static void NAMESPACE##_deinit(ARRAY_LIST_NAME *list) {\
        free(list->elements);\
        list->size=0;\
        list->capacity=BASE_ARRAY_LIST_CAPACITY;\
    }\
    \
    static ARRAY_LIST_NAME* NAMESPACE##_new() { \
        ARRAY_LIST_NAME *array_list = calloc(1,sizeof(ARRAY_LIST_NAME)); \
        NAMESPACE##_init(array_list);\
        return array_list; \
    }\
    \
    static void NAMESPACE##_free(ARRAY_LIST_NAME *list) {\
        NAMESPACE##_deinit(list);\
        free(list);\
    }\
    \
    static void NAMESPACE##_add(ARRAY_LIST_NAME* list, T element) { \
        size_t size = list->size;\
        if (size>=list->capacity) { \
            list->elements=realloc(list->elements, 2*list->capacity*sizeof(T));\
            list->capacity*=2; \
        }\
        list->elements[size]=element;\
        list->size++;\
    } \
    \
    static void NAMESPACE##_add_at(ARRAY_LIST_NAME*list, T element, size_t index) {\
        size_t size = list->size;\
        if (size>=list->capacity) { \
            list->elements=realloc(list->elements, 2*list->capacity*sizeof(T));\
            list->capacity*=2; \
        }\
        T* source=list->elements+index; \
        T* destination=source+1;\
        size_t copy_size = size-index; \
        memmove(destination, source, copy_size*sizeof(T) );\
        *(list->elements+index)=element; \
        list->size++; \
    }\
    \
    \
    static void NAMESPACE##_pop(ARRAY_LIST_NAME* list) { \
        list->size = max(list->size-1, 0); \
    } \
    \
    static void NAMESPACE##_remove(ARRAY_LIST_NAME * list, size_t index) { \
        if (index>=list->size || index<0) {return;} \
        size_t size = list->size-(index+1);\
        T* source = list->elements+(index+1); \
        T* destination =list->elements+index;\
        memmove(destination, source, size*sizeof(T)); \
        list->size--; \
    } \
    \
    static void NAMESPACE##_apply_operation(ARRAY_LIST_NAME* list, void (*ptr) (T) ) { \
        for (size_t i=0;i<list->size;i++) {\
            ptr(list->elements[i]);\
        }\
    }\
    \
    static bool NAMESPACE##_is_empty(ARRAY_LIST_NAME* list) {\
        return list->size==0;\
    }\
    \
    static void NAMESPACE##_print(ARRAY_LIST_NAME *list, void (print_function) (T)) {\
        printf("%s[ capacity : %zu - size : %zu - elements [{", #ARRAY_LIST_NAME, list->capacity, list->size);\
        T *elements = list->elements;\
        for (size_t i=0; i<list->size; i++) {\
            printf(" ");\
            print_function(elements[i]);\
            if (i<list->size-1) {\
                printf(" ,");\
            }\
        }\
        \
        \
        \
        \
        \
        printf(" } ]");\
    }\
    \
    static void NAMESPACE##_println(ARRAY_LIST_NAME *list, void (print_function) (T)) {\
        NAMESPACE##_print(list, print_function);\
        printf("\n");\
    }\

#endif 
