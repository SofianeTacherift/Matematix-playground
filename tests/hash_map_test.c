//
// Created by sofiane on 10/09/2026.
//

#include "hash_table.h"

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


HASH_TABLE(char *, int, str, integer)

int main(void) {

    str_integer_hash_table *map = new_str_integer_hash_table(hash_str, equals_str,print_entry );

    put_to_str_integer_hash_table(map, "s1", 6);

    put_to_str_integer_hash_table(map, "s2", 10);


    put_to_str_integer_hash_table(map, "s3", 6);

    put_to_str_integer_hash_table(map, "s4", 10);

    put_to_str_integer_hash_table(map, "s6", 6);


    put_to_str_integer_hash_table(map, "s7", 10);

    put_to_str_integer_hash_table(map, "s8", 6);

    put_to_str_integer_hash_table(map, "s9", 10);

    put_to_str_integer_hash_table(map, "s10", 6);

    put_to_str_integer_hash_table(map, "s11", 10);

    put_to_str_integer_hash_table(map, "s12", 6);

    print_str_integer_hash_table(map);

    put_to_str_integer_hash_table(map, "s13", 10);

    put_to_str_integer_hash_table(map, "s14", 10);


    print_str_integer_hash_table(map);

    put_to_str_integer_hash_table(map, "s2", 11);

    print_str_integer_hash_table(map);

    remove_from_str_integer_hash_table(map,"bonjour1");


    /** a/b>0.75 4a/b> 3  **/


    return 0;



}
