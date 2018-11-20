#include "hash_table.h"


int linesearch(int* a, int* b)
{
    assert(a != b);

    assert(a != NULL);
    assert(b != NULL);

    for( ; *b != 0; a++, b++)
        if(*b != *a)
            return *a - *b;
    return 0;
}

//*****************************************************************


//==========================__OK__==============================
int Hash_table_OK(struct Hash_table* this)
{
    for(int i = 0; i < HASH_TABLE_SIZE; i++)
        if(List_ASI_OK(&this->table[i]))
            return ERR_LIST;
    return OK;
}

//==========================__DUMP_FILE__=======================
int Hash_table_dump(struct Hash_table* this)
{
    for(int i = 0; i < HASH_TABLE_SIZE; i++)
        if(List_ASI_dump_file(&this->table[i]))
            return ERR_LIST;
    return OK;
}


//*****************************************************************

//==========================__INIT__============================
int Hash_table_set(struct Hash_table* this, int** dict_arr, int num_of_lines, int (*hash)(int*))
{
    for(int i = 0; i < num_of_lines; i++)
        if(List_ASI_add_sorted(&this->table[hash(dict_arr[i]) % HASH_TABLE_SIZE], dict_arr[i]))
            return ERR_LIST;
    return OK;
}


//*****************************************************************

//==========================__CONSTRUCTOR__=====================
void Hash_table_constructor(struct Hash_table* this)
{
    for(int i = 0; i < HASH_TABLE_SIZE; ++i)
        List_ASI_constructor(&this->table[i], i);
    return;
}

//==========================__DESTRUCTOR__======================
void Hash_table_destructor(struct Hash_table* this)
{
    for(int i = 0; i < HASH_TABLE_SIZE; ++i)
        List_ASI_destructor(&this->table[i]);
    return;
}

