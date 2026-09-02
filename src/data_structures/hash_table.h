#ifndef HASH_H
#define HASH_H
#include <stdbool.h>
#include "array_list.h"

#define STR(string) #string 
#define PSTR(str) & (char*) { STR(str) }
#define PINT(i) & (int) { i }
#define PLONG(l) & (long) { l }
#define PDOUBLE( d ) & (double) { d }
#define PFLOAT ( f ) & (float ) { f }
#define PCHAR(c) & (char) { c }



typedef struct entry {
    void *key;
    void *value;
    struct entry *next;
} entry;


typedef struct hash_table {
    long capacity;
    long size;
    entry ** buckets;
    long (*hash) (void*);
    bool (*equal) (void*, void *);
    size_t key_size;
    size_t value_size;
} hash_table;

long hash_str(void *p);
bool equal_str(void *p1, void *p2);
void print_str(void *p);

long hash_int(void *p);
bool equal_int(void *p1, void *p2);
void print_int(void *p);

long hash_long(void *p);
bool equal_long(void *p1, void *p2);
void print_long(void *p);

long hash_float(void *p);
bool equal_float(void *p1, void *p2);
void print_float(void *p);

long hash_double(void *p);
bool equal_double(void *p1, void *p2);
void print_double(void *p);

long hash_char(void *p);
bool equal_char(void *p1, void *p2);
void print_char(void *p);









hash_table *new_hash_table(size_t key_size, size_t value_size, long (*hash) (void*), bool (*equal) (void*, void*) );


entry * new_entry(void *key, void *value, size_t key_size, size_t value_size);

void free_entry(entry * to_free);


void set_hash_table_capacity(hash_table * table, size_t new_capacity);

void put_entry(hash_table * table, void * key,  void* value);

void print_hash_table(hash_table *table, void (print_key_function) (void*), void (print_value_function) (void*) );

bool contains_key(hash_table * table, void *key);

void remove_key(hash_table *table, void * key);


void * get_value(hash_table * table, void *key);






#endif