#include <stdio.h>
#include "hash_table.h"
#include <stdbool.h>
#include <math.h>


long hash_str(void *p) {
    char **strp=p;
    char *str=*strp;

    long hash=0;
    int len=strlen(str);
    for (int i=0; i<len;i++) {
        int charI=str[i];
        hash=31*hash+charI;
    }
    return hash;
}

bool equal_str(void *p1, void *p2) {
    char **strp1=p1, **strp2=p2;

    int cmp=strcmp(*strp1,  *strp2);
    return cmp==0;
}

void print_str(void *p) {
    char **strp=p;
    printf("%s", *strp);
}

long hash_int(void *p) {
    int val = *(int*)p;
    return (long)val;
}

bool equal_int(void *p1, void *p2) {
    return *(int*)p1 == *(int*)p2;
}

void print_int(void *p) {
    printf("%d", *(int*)p);
}

long hash_long(void *p) {
    return *(long*)p;
}

bool equal_long(void *p1, void *p2) {
    return *(long*)p1 == *(long*)p2;
}

void print_long(void *p) {
    printf("%ld", *(long*)p);
}

long hash_float(void *p) {
    float val = *(float*)p;
    return (long)val;
}

bool equal_float(void *p1, void *p2) {
    return *(float*)p1 == *(float*)p2;
}

void print_float(void *p) {
    printf("%f", *(float*)p);
}

long hash_double(void *p) {
    double val = *(double*)p;
    return (long)val;
}

bool equal_double(void *p1, void *p2) {
    return *(double*)p1 == *(double*)p2;
}

void print_double(void *p) {
    printf("%f", *(double*)p);
}

long hash_char(void *p) {
    char val = *(char*)p;
    return (long)val;
}

bool equal_char(void *p1, void *p2) {
    return *(char*)p1 == *(char*)p2;
}

void print_char(void *p) {
    printf("%c", *(char*)p);
}


void *copy_ptr(void *ptr, size_t size) {
    void * copy = malloc(size);
    memcpy(copy, ptr, size);
    return copy;
}


hash_table *new_hash_table(size_t key_size, size_t value_size, long (*hash) (void*), bool (*equal) (void*, void*) ) {
    if (hash ==NULL || equal==NULL) {return NULL;}
    hash_table * res = malloc(sizeof(hash_table));
    size_t capacity=16;
    res->capacity=capacity;
    res->buckets=calloc(capacity,sizeof(entry*));
    res->size=0;
    res->key_size=key_size;
    res->value_size=value_size;
    res->equal=equal;
    res->hash=hash;
    return res;
}


entry * new_entry(void *key, void *value, size_t key_size, size_t value_size) {
    if (key==NULL || value==NULL) {return NULL;}
    entry * res=malloc(sizeof(entry));
    void * k = malloc(key_size);
    void * v = malloc (value_size);
    memcpy(k, key, key_size);
    memcpy(v, value, value_size);
    res->key=k;
    res->value=v;
    res->next=NULL;
    return res;
}


void free_entry(entry *to_free) {
    entry tfe = *to_free;
    free(tfe.key);
    free(tfe.value);
    free(to_free);

}
void set_hash_table_capacity(hash_table * table, size_t new_capacity) {
    size_t old_capacity=table->capacity;
    int count=0;
    table->capacity=new_capacity;
    table->buckets=realloc(table->buckets, new_capacity*sizeof(entry*));
    for (int i=old_capacity; i<new_capacity; i++) {
        table->buckets[i]=NULL;
    }



    for (int i=0; i<old_capacity; i++) {
        entry * current = table->buckets[i];
        table->buckets[i]=NULL;
        while (current!=NULL) {
            


            entry * copy = current;

            current=current->next;

            long hash = table->hash(copy->key);
            long  new_index=hash & (table->capacity-1);
            entry * start = table->buckets[new_index];
            table->buckets[new_index]=copy;
            

            copy->next=start;




        }


    }


}

void put_entry(hash_table * table, void * key,  void* value) {
    if (key==NULL || value==NULL) {return;}
    long hash = table->hash(key);
    int index= hash & (table->capacity-1);


    entry * start = table->buckets[index];
    entry sentinel = {.next=start};
    entry *current = &sentinel;

    while (current->next!=NULL && !table->equal(current->next->key, key)) {
        current=current->next;
    }
    if (current->next==NULL) {
        entry * new=new_entry(key, value, table->key_size, table->value_size );
        if (current->next==start) {
            table->buckets[index]=new;       
        }
        else {
            current->next=new;
        }
        table->size++;
    }
    else {
        memcpy(current->next->value, value, table->value_size);
    }

    double capacity = (double) table->capacity;
    double size = (double) table->size;
    if (size/capacity>=0.75) {
        set_hash_table_capacity(table, table->capacity*2);
    }

    

}




void print_hash_table(hash_table *table, void (print_key_function) (void*), void (print_value_function) (void*) ) {
    if (print_key_function==NULL || print_value_function==NULL) {return;}
    printf("hash_table size=%ld capacity = %ld - elements = { ", table->size, table->capacity);
    int count=0;
    for (int i=0; i<table->capacity; i++) {
        entry *current=(table->buckets[i]);
        while (current!=NULL) {
            count++;
            printf("index=%d ", i);
            print_key_function(current->key);
            printf(" : ");
            print_value_function(current->value);
            if (count>=table->size) {
                printf(" }\n");
                return;
            }
            else {
                printf(" , ");
            }
            current=current->next;
        }
    }
    printf("}\n");
}

bool contains_key(hash_table * table, void *key) {
    if (key==NULL) {return NULL;}
    long hash=table->hash(key);
    int index = hash & (table->capacity-1);

    entry *current=(table->buckets[index]);
    while (current!=NULL) {
        if (table->equal(key, current->key)) {
            return true;
        }
        current=current->next;
    }
    return false;
}

void * get_value(hash_table * table, void *key) {
    if (key==NULL) {return NULL;}
    long hash=table->hash(key);
    int index = hash & (table->capacity-1);
    entry *current=(table->buckets[index]);
    while (current!=NULL) {
        if (table->equal(key, current->key)) {
            return current->value;
        }
        current=current->next;
    }
    return NULL;
}


void remove_key(hash_table *table, void * key) {
    if (key==NULL) {return;}
    long hash=table->hash(key);
    int index = hash & (table->capacity-1);
    entry sentinel = (entry) {.next=table->buckets[index]};
    entry *current = &sentinel;
    
    while (current->next !=NULL && !table->equal(current->next->key, key)) {
        current=current->next;
    }
    if (current->next!=NULL) {
        entry * removed = current->next;
        current->next=current->next->next;
        if (removed==table->buckets[index]) {
            table->buckets[index]=removed->next;
        }
        free_entry(removed);
        table->size --;
    }

}



