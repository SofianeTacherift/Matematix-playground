#include "hash_table.h"
#include <stdio.h>

int main(void) {

hash_table *table = new_hash_table(sizeof(char *), sizeof(int), hash_str, equal_str);

put_entry(table, PSTR(chaine_1), PINT(1));
put_entry(table, PSTR(chainede), PINT(2));
put_entry(table, PSTR(chain), PINT(3));
put_entry(table, PSTR(chaine_4), PINT(4));
put_entry(table, PSTR(chainf), PINT(5));
put_entry(table, PSTR(chaine_6), PINT(6));
put_entry(table, PSTR(chainefr), PINT(7));
put_entry(table, PSTR(chaine_8), PINT(8));
put_entry(table, PSTR(chaine_9), PINT(9));
put_entry(table, PSTR(chaine_10), PINT(10));
put_entry(table, PSTR(chaine_11), PINT(11));

printf("table : ");
print_hash_table(table, &print_str, &print_int);

put_entry(table, PSTR(chaine_12), PINT(12)); 

printf("After resizing:\n");
print_hash_table(table, &print_str, &print_int);

return 0;
}