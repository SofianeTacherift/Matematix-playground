//
// Created by sofiane on 10/09/2026.
//

#include "hash_map.h"

#include <string.h>

void print_int(int i) {
    printf("%d", i);
}

bool equals_int(int i1, int i2) {
    return i1==i2;
}

long hash_int(int i) {
    return i;
}

void print_str(char *s) {
    printf("%s", s);
}

bool equals_str(char *s1, char *s2) {
    return strcmp(s1,s2)==0;
}

long hash_str(char  *p) {
    char *str=p;
    long hash=0;
    int len=strlen(str);
    for (int i=0; i<len;i++) {
        int charI=str[i];
        hash=31*hash+charI;
    }
    return hash;
}

void print_entry(char * s, int i) {
    printf("%s : %d", s, i);
}


HASH_MAP(char *, int, str, integer)

#define copy_to_size_t(dest, src, type)     memcpy( ((char*) (&dest)) + (sizeof(size_t) - sizeof(src) ),  &src , sizeof(src))


int main(void) {

    int a=1;
    size_t t=0;
    copy_to_size_t(t, a, int);
    char *c = (char*) &t;
    printf("res : %zu %p %p %d  %d \n", t,  &t, ((char*) (&t)) + (sizeof(size_t) - sizeof(a) ), *(int*) &t , *( (int*) (c+4) )  );

    str_integer_hash_map *map = new_str_integer_hash_map(hash_str, equals_str,print_entry );

    put_to_str_integer_hash_map(map, "s1", 6);

    put_to_str_integer_hash_map(map, "s2", 10);


    put_to_str_integer_hash_map(map, "s3", 6);

    put_to_str_integer_hash_map(map, "s4", 10);

    put_to_str_integer_hash_map(map, "s6", 6);


    put_to_str_integer_hash_map(map, "s7", 10);

    put_to_str_integer_hash_map(map, "s8", 6);

    put_to_str_integer_hash_map(map, "s9", 10);

    put_to_str_integer_hash_map(map, "s10", 6);

    put_to_str_integer_hash_map(map, "s11", 10);

    put_to_str_integer_hash_map(map, "s12", 6);

    print_str_integer_hash_map(map);

    put_to_str_integer_hash_map(map, "s13", 100020920);

    put_to_str_integer_hash_map(map, "s14", 10);


    print_str_integer_hash_map(map);

    put_to_str_integer_hash_map(map, "s2", 10);

    printf("%d\n", *get_from_str_integer_hash_map(map, "s13"));
    print_str_integer_hash_map(map);

    remove_from_str_integer_hash_map(map,"bonjour1");


    /** a/b>0.75 4a/b> 3  **/


    return 0;



}
