#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/hash_table.h"

#define NUM_OF_HASHES (3)


/** \brief Fills array with sizes of lists of hash tables, initialized by the given array of words and hash function
 *
 * \param dict_arr int** [in] - array of words (strings as arrays of integers)
 * \param num_of_lines int [in] - number of words if the previous array
 * \param hash_count int* [in] - array to put size of lists into
 * \param hash int (*)(int*) [in] - pointer to the hash function (gets key as string as array of integers)
 *
 */
void hash_comp_fill(int** dict_arr, int num_of_lines, int* hash_count);

/** \brief Makes plots for each hash function and one for their comparison
 *
 * \param hash_comp[NUM_OF_HASHES][HASH_TABLE_SIZE] int [in] - all the array that were got from 'hash_comp_fill'
 *
 */
void hash_comp_makeplot(int hash_comp[NUM_OF_HASHES][HASH_TABLE_SIZE]);


/** \brief Hash function: ASCII-code of the first letter
 *
 * \param s int* - string (as array of integers) to calculate hash of
 * \return int - hash value
 *
 */
int hash_1(int* s)
{
    return *s % HASH_TABLE_SIZE;
}

/** \brief Hash function: sum of ASCII-codes of all letters
 *
 * \param s int* - string (as array of integers) to calculate hash of
 * \return int - hash value
 *
 */
int hash_2(int* s)
{
    int sum = 0;
    while(*s != (int) '\0' && *s != (int) ':')
        sum += *s++;
    return sum % HASH_TABLE_SIZE;
}

/** \brief Hash function: sum of ASCII-codes of all letters divided by the string length
 *
 * \param s int* - string (as array of integers) to calculate hash of
 * \return int - hash value
 *
 */
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

//*****************************************************************\\
//*****************************************************************\\

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

    DELETE(dict_arr);
    DELETE(dictionary);
    return 0;
}

//*****************************************************************\\
//*****************************************************************\\

void hash_comp_fill(int** dict_arr, int num_of_lines, int* hash_count, int (*hash)(int*))
{
    struct Hash_table hsh_tbl = {};
    Hash_table_constructor(&hsh_tbl);

    Hash_table_init(&hsh_tbl, dict_arr, num_of_lines, hash);

    for(int i = 0; i < HASH_TABLE_SIZE; ++i)
        hash_count[i] = hsh_tbl.table[i].size;

    Hash_table_dump(&hsh_tbl);

    Hash_table_destructor(&hsh_tbl);
    return;
}

void hash_comp_makeplot(int hash_comp[NUM_OF_HASHES][HASH_TABLE_SIZE])
{
    FILE* datafile = fopen("plots/hash_comp.txt", "w");

    for(int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        fprintf(datafile, "%5d", i);
        for(int j = 0; j < NUM_OF_HASHES; ++j)
            fprintf(datafile, " %5d", hash_comp[j][i]);
        fprintf(datafile, "\n");
    }

    fclose(datafile);

    //*******************************************************

    FILE* plotfile = fopen("plots/scen_hash_comp.plt", "w");

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

    return;
}

