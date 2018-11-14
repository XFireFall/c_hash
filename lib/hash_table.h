/** \file "hash_table.h"
 *
 */

#ifndef HASH_TABLE_H_INCLUDED
#define HASH_TABLE_H_INCLUDED

#include "strings_int.h"
#include "constants.h"
#include "list_ASI.h"

#define HASH_TABLE_SIZE (256)


/** \brief Constant values for error cases
 *
 * \param OK       - if everything is OK
 * \param ERR_LIST - having troubles with some list
 *
 */
enum {
    ERR_LIST = 1
    };

/** \brief Hash table
 *
 * \param table[HASH_TABLE_SIZE] struct List_ASI table - array of lists "List_ASI"
 *
 */
struct Hash_table
{
    struct List_ASI table[HASH_TABLE_SIZE];
};


//*****************************************************************

/** \brief Checks if second string (array of integers) is a part of the first one from its beginning
 *
 * \param a int* [in] - first string
 * \param b int* [in] - second string
 * \return int - 0 if second string is a part of the first one
 *
 */
int linesearch(int* a, int* b);


//*****************************************************************

//==========================__OK__==============================
/** \brief Hash table verification
 *
 * \param this struct Hash_table* [in] - list to verify
 * \return int - see error cases description
 *
 */
int Hash_table_OK(struct Hash_table* this);

//==========================__DUMP_FILE__=======================
/** \brief Dumps all lists in dump files
 *
 * \param this struct Hash_table* [in] - hash table to print out
 * \return int - result of hash table verification
 *
 */
int Hash_table_dump(struct Hash_table* this);


//*****************************************************************

//==========================__INIT__============================
/** \brief Initializes hash table with given words and hash function
 *
 * \param this struct Hash_table* [in] - list to initialize
 * \param dict_arr int** [in] - array of words (strings as arrays of integers)
 * \param num_of_lines int [in] - number of words if the previous array
 * \param hash int (*)(int*) [in] - pointer to the hash function (gets key as string as array of integers)
 * \return int - result of hash table verification
 *
 */
int Hash_table_init(struct Hash_table* this, int** dict_arr, int num_of_lines, int (*hash)(int*));


//*****************************************************************

//==========================__CONSTRUCTOR__=====================
/** \brief Initializes all lists
 *
 * \param this struct Hash_table* [in] - hash table to initialize
 *
 * \warning Must be called _before_ any work with the list is done
 *
 */
void Hash_table_constructor(struct Hash_table* this);

//==========================__DESTRUCTOR__======================
/** \brief Destroys list
 *
 * \param this struct Hash_table* [in] - hash table to destroy
 *
 * \warning Must be called _after_ all the work with the list is done
 *
 */
void Hash_table_destructor(struct Hash_table* this);


#endif // HASH_TABLE_H_INCLUDED
