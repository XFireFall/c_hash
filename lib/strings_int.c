#include "strings_int.h"

//==========================__ISTEXT__==========================
// Checks if given code of a symbol is a letter of English or Russian alphabet
int istext(int n)
{
    if(n > (int) 'z')
        return 2;
    else
        return (((n >= (int) 'A') && (n <= (int) 'Z')) || ((n >= (int) 'a') && (n <= (int) 'z'))) ? 1 : 0;
}

//==========================__PRINTLINE__=======================
// Prints string (as integer array)
int printline(int* line)
{
    assert(line != NULL);

    if(!line)
        return 1;

    while(*line)
        printf("%c", (char) *line++);
    return 0;
}

//==========================__PRINTTEXT__=======================
// Prints given number of strings (as integer arrays) from array
int printtext(int** txt_arr, int num_of_lines)
{
    assert(txt_arr != NULL);

    if(!txt_arr)
        return 1;

    int sum_errors = 0;
    for(int i = 0; i < num_of_lines; ++i)
    {
        sum_errors += printline(txt_arr[i]);
        printf("\n");
    }

    return sum_errors;
}

//==========================__PRINT_ORIGINAL_TEXT__=============
// Prints given number of strings (as integer arrays) from the place that used to be a single string (as integer array)
int print_original_text(int* text, int num_of_lines)
{
    assert(text != NULL);

    if(!text)
        return 1;

    while(num_of_lines > 0)
    {
        printf("%c", (*text == (int) '\0') ? '\n' : (char) *text);
        ++text;
        if(*text == (int) '\0')
            --num_of_lines;
    }
    return 0;
}

//==========================__FGETFILE__========================
// Creates a string (as integer array) which contains entire given file
int* fgetfile(char* filename, int* num_of_lines)
{
    assert(filename != NULL);
    assert(num_of_lines != NULL);

    if(!filename)
        return NULL;
    if(!num_of_lines)
        return NULL;

    struct stat st = {};
    stat(filename, &st);

    int len = (int) st.st_size;
    if(!len)
    {
        printf("No file or it is empty\n");
        return NULL;
    }

    FILE *infile = fopen(filename, "r");
    int* text = (int*) calloc(len, sizeof(*text));
    int* txt_p = text - 1;

    int n;
    while((n = getc(infile)) != EOF)
    {
        *++txt_p = n;
        //printf("%c", n);
        if(n == (int) '\n')
            ++*num_of_lines;
    }

    *txt_p = (int) '\0';

    fclose(infile);
    return text;
}

//==========================__SLICEDTEXT__======================
// Slices given string (as integer array) into continuous set of strings (as integer arrays) by changing '\n' to '\0'
// Creates array of strings (as integer arrays) containing strings (as integer arrays) from the sliced one
int** slicedtext(int* text, int num_of_lines)
{
    assert(text != NULL);

    if(!text)
        return NULL;

    int** txt_arr= (int**) calloc(num_of_lines, sizeof(*txt_arr));
    int** txt_arr_p = txt_arr;

    *txt_arr_p++ = text;
    while(*text)
    {
        if(*text == (int) '\r')
            *text = (int) '\0';
        if(*text == (int) '\n')
        {
            *text = (int) '\0';
            *txt_arr_p++ = text + 1;
        }
        text++;
    }
    return txt_arr;
}

//==========================__LINE_COMP__=======================
// Compares strings (as integer arrays) from their beginning
// Ignores non-text symbols
int linecomp(int* a, int* b)
{
    assert(a != b);

    assert(a != NULL);
    assert(b != NULL);

    while(!istext(*a))
        a++;
    while(!istext(*b))
        b++;

    while(*a == *b)
    {
        if(*a == (int) '\0')
            return 0;

        do
        {
            ++a;
        } while(!istext(*a) && *a != (int) '\0');

        do
        {
            ++b;
        } while(!istext(*b) && *b != (int) '\0');
    }
    return *a - *b;
}

//==========================__REV_LINE_COMP__===================
// Compares strings (as integer arrays) from their ending
// Ignores non-text symbols
int rev_linecomp(int* a, int* b)
{
    assert(a != b);

    assert(a != NULL);
    assert(b != NULL);

    int* a0 = a;
    int* b0 = b;

    while(*a != (int) '\0')
        a++;
    while(*b != (int) '\0')
        b++;

    if (a == a0 || b == b0)
        return *a - *b;

    while(*a == *b)
    {
        do
        {
            --a;
        } while(!istext(*a) && a != a0);

        do
        {
            --b;
        } while(!istext(*b) && b != b0);

        if (a == a0 || b == b0)
            break;
    }
    return *a - *b;
}

//==========================__QSORT_TEXT_ARRAY__================
// Sorts some number of strings (as integer arrays) from given array by given comparing function
// Arg = '+' for detailed info
int qsort_text_array(int** txt_arr, int num_of_lines, int (*line_comp_func)(int*, int*), char arg)
{
    assert(txt_arr != NULL);
    assert(line_comp_func != NULL);

    if(!txt_arr || !line_comp_func)
        return 0;

    //**

    if(num_of_lines <= 1)
        return 1;

    int base = num_of_lines - 1;
    int check = 0;
    int num_of_self_recourses = 1;

    if(arg == '+')
    {
        printf("___\n"
               "-> new reference, <base> = (");
        printline(txt_arr[base]);
        printf("):\n");
        printtext(txt_arr, num_of_lines);
    }

    while(check < base)
        if((*line_comp_func)(txt_arr[check], txt_arr[base]) >= 0)
        {
            int*            t = txt_arr[check];
            txt_arr[check]    = txt_arr[base - 1];
            txt_arr[base - 1] = txt_arr[base];
            txt_arr[base]     = t;
            --base;
            if(arg == '+')
            {
                printline(txt_arr[check]);
                printf(" >= ");
                printline(txt_arr[base]);
                printf("\n");
                printtext(txt_arr, num_of_lines);
            }
        }
        else
            ++check;

    //**

    num_of_self_recourses += qsort_text_array(txt_arr, base, line_comp_func, arg);
    num_of_self_recourses += qsort_text_array(txt_arr + base + 1, num_of_lines - base - 1, line_comp_func, arg);
    return num_of_self_recourses;
}


