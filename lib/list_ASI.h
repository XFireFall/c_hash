#ifndef LIST_ASI_H_INCLUDED
#define LIST_ASI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "strings_int.h"

#define ECHO_TAB_LN( var ) (printf("\t%s = %d\n", strchr(#var, '>') + 1, var))

#define POISON (-1)
#define DOT_NODES_IN_LINE (4)
#define MAX_PRINTED_MEMORY (64)


enum {
    OK = 0,
    ERR_SIZE,
    ERR_CAPACITY,
    ERR_SIZE_CAPACITY,
    ERR_HEAD,
    ERR_TAIL,
    ERR_FREE,
    ERR_DATA,
    ERR_INDEX,
    ERR_POISON
    };

struct List_ASI_item
{
    int name[MAX_STR_LEN];
    int prev;
    int next;
};

struct List_ASI
{
    int ID;
    int size;
    int capacity;
    int head;
    int tail;
    int free;
    struct List_ASI_item* data;
};

//*****************************************************************

void linecopy(int* s, int* t);

int expand_data(struct List_ASI* this);

//*****************************************************************

int List_ASI_OK(struct List_ASI* this);

int List_ASI_dump_file(struct List_ASI* this);

int List_ASI_dump(struct List_ASI* this);

int List_ASI_makegraph(struct List_ASI* this);

//*****************************************************************

int List_ASI_remove(struct List_ASI* this, int idx);

int List_ASI_pop(struct List_ASI* this);

//*****************************************************************

int List_ASI_add_before(struct List_ASI* this, int idx, int* new_name);

int List_ASI_add_after(struct List_ASI* this, int idx, int* new_name);

int List_ASI_push(struct List_ASI* this, int* new_name);

int List_ASI_add_sorted(struct List_ASI* this, int* new_name);

//*****************************************************************

void List_ASI_constructor(struct List_ASI* this, int number);

void List_ASI_destructor(struct List_ASI* this);

#endif // LIST_ASI_H_INCLUDED
