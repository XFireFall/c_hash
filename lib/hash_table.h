#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED

#include "strings_int.h"
#include "constants.h"
#include "list_ASI.h"

#define HASH_TABLE_SIZE (256)


struct Hash_table
{
    struct List_ASI table[HASH_TABLE_SIZE];
};

void Hash_table_init(struct Hash_table* this, int** dict_arr, int num_of_lines, int (*hash)(int*));

void Hash_table_dump(struct Hash_table* this);

void Hash_table_constructor(struct Hash_table* this);

void Hash_table_destructor(struct Hash_table* this);

#endif // HASH_TABLE_H_INCLUDED
