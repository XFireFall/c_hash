#include "hash_table.h"


void Hash_table_constructor(struct Hash_table* this)
{
    for(int i = 0; i < HASH_TABLE_SIZE; ++i)
        List_ASI_constructor(&this->table[i], i);
    return;
}

void Hash_table_destructor(struct Hash_table* this)
{
    for(int i = 0; i < HASH_TABLE_SIZE; ++i)
        List_ASI_destructor(&this->table[i]);
    return;
}

void Hash_table_init(struct Hash_table* this, int** dict_arr, int num_of_lines, int (*hash)(int*))
{
    for(int i = 0; i < num_of_lines; i++)
        List_ASI_add_sorted(&this->table[hash(dict_arr[i])], dict_arr[i]);
    return;
}

void Hash_table_dump(struct Hash_table* this)
{
    for(int i = 0; i < HASH_TABLE_SIZE; i++)
        List_ASI_dump_file(&this->table[i]);
    return;
}

