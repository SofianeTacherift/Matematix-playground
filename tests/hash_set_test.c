//
// Created by sofiane on 10/09/2026.
//

#include "hash_set.h"

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

HASH_SET(int, int)
HASH_SET(char*, str)

int main(void) {




    int_hash_set *int_set = new_int_hash_set(hash_int, equals_int, print_int);

    add_int_to_hash_set(int_set, 10);
    add_int_to_hash_set(int_set, 10);


    print_int_hash_set(int_set);

    remove_int_from_hash_set(int_set, 10);
    print_int_hash_set(int_set);

    add_int_to_hash_set(int_set, 1);
    add_int_to_hash_set(int_set, 10);
    add_int_to_hash_set(int_set, 109);
    add_int_to_hash_set(int_set, 90);
    add_int_to_hash_set(int_set, 1910);
    add_int_to_hash_set(int_set, 20);

    double_int_hash_set_capacity(int_set);
    add_int_to_hash_set(int_set, 891);

    print_int_hash_set(int_set);


    free_int_hash_set(int_set);

    str_hash_set *str_set = new_str_hash_set(hash_str, equals_str, print_str);

    add_str_to_hash_set(str_set, "bonjour");
    add_str_to_hash_set(str_set, "bo");
    add_str_to_hash_set(str_set, "bonj");

    add_str_to_hash_set(str_set, "bonjour0");
    add_str_to_hash_set(str_set, "bonjour5");
    add_str_to_hash_set(str_set, "bo1");
    add_str_to_hash_set(str_set, "bon3");

    add_str_to_hash_set(str_set, "bonjour2");
    add_str_to_hash_set(str_set, "bonjour10");
    add_str_to_hash_set(str_set, "bonjour20");
    add_str_to_hash_set(str_set, "bonjour21");


    print_str_hash_set(str_set);

    /** a/b>0.75 4a/b> 3  **/


    return 0;



}
