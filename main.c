/** \file "main.c"
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/hash_table.h"

#define NUM_OF_HASHES (3)


//==========================__FILL__============================
/** \brief Fills array with sizes of lists of hash tables, initialized by the given array of words and hash function
 *
 * \param dict_arr int** [in] - array of words (strings as arrays of integers)
 * \param num_of_lines int [in] - number of words if the previous array
 * \param hash_count int* [in] - array to put size of lists into
 * \param hash int (*)(int*) [in] - pointer to the hash function (gets key as string as array of integers)
 *
 */
void hash_comp_fill(int** dict_arr, int num_of_lines, int* hash_count, int (*hash)(int*));

//==========================__MAKEPLOTS__=======================
/** \brief Makes plots for each hash function and one for their comparison
 *
 * \param hash_comp[NUM_OF_HASHES][HASH_TABLE_SIZE] int [in] - all the array that were got from 'hash_comp_fill'
 *
 */
void hash_comp_makeplot(int hash_comp[NUM_OF_HASHES][HASH_TABLE_SIZE]);


//==========================__HASH_DED__========================
/** \brief Hash function: ASCII-code of the first letter
 *
 * \param s int* - string (as array of integers) to calculate hash of
 * \return int - hash value
 *
 */
int hash_1(int* s);

//==========================__HASH_SUM__========================
/** \brief Hash function: sum of ASCII-codes of all letters
 *
 * \param s int* - string (as array of integers) to calculate hash of
 * \return int - hash value
 *
 */
int hash_2(int* s);

//==========================__HASH_UNIT_CODE__==================
/** \brief Hash function: sum of ASCII-codes of all letters divided by the string length
 *
 * \param s int* - string (as array of integers) to calculate hash of
 * \return int - hash value
 *
 */
int hash_3(int* s);

//*****************************************************************
//*****************************************************************
//==========================__MAIN__===============================
//*****************************************************************
//*****************************************************************

int main()
{
    char infilename[] = "dictfull.txt";

    //*************************************************************

    int num_of_lines = 0;

    int* dictionary = fgetfile(infilename, &num_of_lines);
    if(!dictionary)
    {
        printf(RED ALERT"FATAL ERROR"RESET);
        return 1;
    }

    int** dict_arr = slicedtext(dictionary, num_of_lines);
    if(!dict_arr)
    {
        printf(RED ALERT"FATAL ERROR"RESET);
        DELETE(dictionary);
        return 1;
    }

    //******************

    int hash_comp[NUM_OF_HASHES][HASH_TABLE_SIZE] = {};

    int (*hash_arr[NUM_OF_HASHES])(int*) = {hash_1, hash_3, hash_2};

    for(int i = 0; i < NUM_OF_HASHES; ++i)
        hash_comp_fill(dict_arr, num_of_lines, hash_comp[i], hash_arr[i]);

    hash_comp_makeplot(hash_comp);

    //******************

    struct Hash_table hsh_tbl = {};
    Hash_table_constructor(&hsh_tbl);

    if(Hash_table_init(&hsh_tbl, dict_arr, num_of_lines, hash_2))
    {
        printf("["RED"FAILED"RESET"]\n");
        Hash_table_destructor(&hsh_tbl);
        return 1;
    }

    char word[MAX_STR_LEN] = "";
    int iword[MAX_STR_LEN] = {};
    while(1)
    {
        printf("Write a word to search:\n==>\t"BLUE);
        scanf("%s", word);
        printf(RESET);

        int k = 0;
        for( ; word[k] != '\0'; ++k)
            iword[k] = (int) word[k];
        iword[k] = 0;

        int key = hash_2(iword);

        for(int i = hsh_tbl.table[key].head; i != 0; i = hsh_tbl.table[key].data[i].next)
            if(!linesearch(hsh_tbl.table[key].data[i].name, iword))
            {
                printline(hsh_tbl.table[key].data[i].name);
                break;
            }


        break;
    }
    Hash_table_dump(&hsh_tbl);

    Hash_table_destructor(&hsh_tbl);

    //******************

    DELETE(dict_arr);
    DELETE(dictionary);
    return 0;
}

//*****************************************************************
//*****************************************************************

//==========================__HASH_DED__========================
int hash_1(int* s)
{
    return *s % HASH_TABLE_SIZE;
}

//==========================__HASH_SUM__========================
int hash_2(int* s)
{
    int sum = 0;
    while(*s != (int) '\0' && *s != (int) ':')
        sum += *s++;
    return sum % HASH_TABLE_SIZE;
}

//==========================__HASH_UNIT_CODE__==================
int hash_3(int* s)
{
    int sum = 0;
    int len = 0;
    while(*s != '\0' && *s != ':')
    {
        sum += (int) *s++;
        len++;
    }
    return ((len) ? sum / len : sum) % HASH_TABLE_SIZE;
}

//*****************************************************************
//*****************************************************************

//==========================__FILL__============================
void hash_comp_fill(int** dict_arr, int num_of_lines, int* hash_count, int (*hash)(int*))
{
    assert(dict_arr != NULL);
    assert(hash_count != NULL);
    assert(hash != NULL);

    printf("Getting data for plots ...\t");

    struct Hash_table hsh_tbl = {};
    Hash_table_constructor(&hsh_tbl);

    if(Hash_table_init(&hsh_tbl, dict_arr, num_of_lines, hash))
    {
        printf("["RED"FAILED"RESET"]\n");
        Hash_table_destructor(&hsh_tbl);
        return;
    }


    for(int i = 0; i < HASH_TABLE_SIZE; ++i)
        hash_count[i] = hsh_tbl.table[i].size;

    Hash_table_dump(&hsh_tbl);

    Hash_table_destructor(&hsh_tbl);

    printf("[  "GREEN"OK"RESET"  ]\n");

    return;
}

//==========================__MAKEPLOTS__=======================
void hash_comp_makeplot(int hash_comp[NUM_OF_HASHES][HASH_TABLE_SIZE])
{
    assert(hash_comp != NULL);

    printf("Loading data for plots in folder 'plots'...\t");

    FILE* datafile = fopen("plots/hash_comp.txt", "w");

    if(datafile == NULL)
    {
        printf("["RED"FAILED"RESET"]\n");
        printf(BLUE"\tCheck if folder 'plots' exists in the 'main.c' directory\n"RESET);
        return;
    }

    for(int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        fprintf(datafile, "%5d", i);
        for(int j = 0; j < NUM_OF_HASHES; ++j)
            fprintf(datafile, " %5d", hash_comp[j][i]);
        fprintf(datafile, "\n");
    }

    fclose(datafile);

    printf("[  "GREEN"OK"RESET"  ]\n");

    //*******************************************************

    printf("Loading gnuplot scenario in folder 'plots'...\t");

    FILE* plotfile = fopen("plots/scen_hash_comp.plt", "w");

    if(plotfile == NULL)
    {
        printf("["RED"FAILED"RESET"]\n");
        printf(BLUE"\tCheck if folder 'plots' exists in the 'main.c' directory\n"RESET);
        return;
    }

    fprintf(plotfile, "set output 'hash_comp.png'\n"
                      "set ylabel \"Count of words\"\n"
                      "set xlabel \"Hash value\"\n"
                      "set grid ytics\n"
                      "set style data histograms\n"
                      "set boxwidth 1 absolute\n"
                      "set style fill solid 1\n");

    fprintf(plotfile, "set yrange [0:100]\n"
                      "set xrange [-1:%d]\n", HASH_TABLE_SIZE - 1);

    fprintf(plotfile, "set terminal png font \"Verdana,50\" size %d, %d\n", 10000, 1000);
    fprintf(plotfile, "plot 'hash_comp.txt' u 2 ti \"hash 1\"");
    for(int i = 1; i < NUM_OF_HASHES; ++i)
        fprintf(plotfile, ", '' u %d ti \"hash %d\"", i + 2, i + 1);
    fprintf(plotfile, "\n");

    fprintf(plotfile, "set terminal png font \"Verdana,10\" size %d, %d\n", 1000, 800);
    for(int i = 0; i < NUM_OF_HASHES; ++i)
        fprintf(plotfile, "set output 'hash_%d.png'\n"
                         "plot '' u %d ti \"hash %d\" lt rgb 'blue'\n", i + 1, i + 2, i + 1);

    fclose(plotfile);

    printf("[  "GREEN"OK"RESET"  ]\n");

    return;
}

