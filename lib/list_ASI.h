/** \file "list_ASI.h"
 *
 */

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


/** \brief Constant values for error cases
 *
 * \param OK                - if everything is OK
 * \param ERR_SIZE          - having troubles with list size (ex: size < 0)
 * \param ERR_CAPACITY      - having troubles with list capacity (ex: capacity < 1)
 * \param ERR_SIZE_CAPACITY - having troubles with both list size and capacity (ex: size > capacity)
 * \param ERR_HEAD          - having troubles with first item index (ex: head < 0)
 * \param ERR_TAIL          - having troubles with last item index (ex: tail < 0)
 * \param ERR_FREE          - having troubles with free space index (ex: free < 0)
 * \param ERR_DATA          - if for some reason data is NULL
 * \param ERR_INDEX         - having troubles with some item indexes (ex: prev < 0 or prev = next)
 * \param ERR_POISON        - if some item has value of POISON, which is actually abandoned from use
 * \param ERR_FILE          - if called file does not exist
 *
 */
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
    ERR_POISON,
    ERR_FILE
    };

/** \brief Item structure of list "List_ASI"
 *
 * \param name[MAX_STR_LEN] int - name of the item, as array of integers
 * \param prev int - previous item in array
 * \param next int - next item in array
 *
 */
struct List_ASI_item
{
    int name[MAX_STR_LEN];
    int prev;
    int next;
};

/** \brief List "List_ASI" - made in dynamic Array, using Strings, as array of Integers, as item name
 *
 * \param ID - identifier of the list
 * \param size - current number of items
 * \param capacity - current array size
 * \param head - first item index
 * \param tail - last item index
 * \param free - free space index
 * \param data struct List_ASI_item* - dynamic array of items
 *
 */
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

/** \brief Copies strings as array of integers
 *
 * \param s int* - first string as array of integers
 * \param t int* - second string as array of integers
 *
 */
void linecopy(int* s, int* t);

/** \brief Increases space for dynamic array by 2
 *
 * \param this struct List_ASI* - the list to expand
 * \return int - result of list verification
 *
 */
int expand_data(struct List_ASI* this);


//*****************************************************************

//==========================__OK__==============================
/** \brief List verification
 *
 * \param this struct List_ASI* [in] - list to verify
 * \return int - see error cases description
 *
 */
int List_ASI_OK         (struct List_ASI* this);

//==========================__DUMP_FILE__=======================
/** \brief Prints entire list information in dump file
 *
 * \param this struct List_ASI* [in] - list to print out
 * \return int - result of list verification
 *
 */
int List_ASI_dump_file  (struct List_ASI* this);

//==========================__DUMP_CONS__=======================
/** \brief Prints entire list information in console
 *
 * \param this struct List_ASI* [in] - list to print out
 * \return int - result of list verification
 *
 */
int List_ASI_dump       (struct List_ASI* this);

//==========================__MAKEGRAPH__=======================
/** \brief Creates a .dot file, visualizing the list array
 *
 * \param this struct List_ASI* [in] - list to make graph with
 * \return int - result of list verification
 *
 */
int List_ASI_makegraph  (struct List_ASI* this);


//*****************************************************************

//==========================__REMOVE__==========================
/** \brief Deletes item without destroying
 *
 * \param this struct List_ASI* [in] - list to delete from
 * \param idx int [in] - index of the item to delete
 * \return int - result of list verification
 *
 * \warning The index accords to the memory set, not to the actual array
 *
 */
int List_ASI_remove (struct List_ASI* this, int idx);

//==========================__POP__=============================
/** \brief Pops last item, just like in stack (according to actual array)
 *
 * \param this struct List_ASI* [in] - list to pop
 * \return int - result of list verification
 *
 */
int List_ASI_pop    (struct List_ASI* this);


//*****************************************************************

//==========================__ADD_BEFORE__======================
/** \brief Adds new item _before_ item on the given index
 *
 * \param this struct List_ASI* [in] - list to add to
 * \param new_name int* [in] - new item name, string as array of integers
 * \param idx int [in] - index of the item to add before
 * \return int - result of list verification
 *
 */
int List_ASI_add_before (struct List_ASI* this, int* new_name, int idx);

//==========================__ADD_AFTER__=======================
/** \brief Adds new item _after_ item on the given index
 *
 * \param this struct List_ASI* [in] - list to add to
 * \param new_name int* [in] - new item name, string as array of integers
 * \param idx int [in] - index of the item to add after
 * \return int - result of list verification
 *
 */
int List_ASI_add_after  (struct List_ASI* this, int* new_name, int idx);

//==========================__PUSH__============================
/** \brief Pops new item, just like in stack (according to actual array)
 *
 * \param this struct List_ASI* [in] - list to push to
 * \param new_name int* [in] - new item name, string as array of integers
 * \return int - result of list verification
 *
 */
int List_ASI_push       (struct List_ASI* this, int* new_name);

//==========================__ADD_SORTED__======================
/** \brief Adds new item in the way not to break the sorted state of the list
 *
 * \param this struct List_ASI* [in] - list to add to
 * \param new_name int* [in] - new item name, string as array of integers
 * \return int - result of list verification
 *
 */
int List_ASI_add_sorted (struct List_ASI* this, int* new_name);


//*****************************************************************

//==========================__CONSTRUCTOR__=====================
/** \brief Initializes list with correct start-up information
 *
 * \param this struct List_ASI* [in] - list to initialize
 * \param number int [in] - new ID for the list
 *
 * \warning Must be called _before_ any work with the list is done
 *
 */
void List_ASI_constructor   (struct List_ASI* this, int number);

//==========================__DESTRUCTOR__======================
/** \brief Deletes all the information of the list
 *
 * \param this struct List_ASI* [in] - list to destroy
 *
 * \warning Must be called _after_ all the work with the list is done
 *
 */
void List_ASI_destructor    (struct List_ASI* this);

//==========================__FIND__============================
/** \brief Finds first item with given name in the list
 *
 * \param this struct List_ASI* [in] - list to search in
 * \param name int* [in] - name of the item to search
 * \return int - 0 if there is no such item, else - index according to the memory set
 *
 */
int List_ASI_find(struct List_ASI* this, int* name);

#endif // LIST_ASI_H_INCLUDED
