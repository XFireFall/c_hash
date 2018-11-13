#include "list_ASI.h"


void linecopy(int* s, int* t)
{
    assert(t != NULL);
    assert(s != NULL);
    assert(s != t);

    for( ; *t != 0; s++, t++)
        *s = *t;
    *s = 0;
    return;
}

int expand_data(struct List_ASI* this)
{
    assert(this != NULL);

    if(List_ASI_OK(this))
        return List_ASI_OK(this);

    this->data = (struct List_ASI_item*) realloc(this->data, this->capacity * 2 * sizeof(struct List_ASI_item));
    int i = this->capacity;
    this->capacity *= 2;
    for( ; i < this->capacity; ++i)
    {
        *this->data[i].name = POISON;
        this->data[i].prev = POISON;
        this->data[i].next = i + 1;
    }
    this->data[i - 1].next = 0;
    this->free = this->capacity / 2;

    return List_ASI_OK(this);
}

//*****************************************************************

//==========================__OK__==============================
int List_ASI_OK(struct List_ASI* this)
{
    assert(this != NULL);

    if(this->size < 0)
        return ERR_SIZE;
    if(this->capacity < 1)
        return ERR_CAPACITY;
    if(this->size >= this->capacity)
        return ERR_SIZE_CAPACITY;

    if(this->head < 0 || this->head > this->size || (this->head == 0 && this->size > 0))
        return ERR_HEAD;
    if(this->tail < 0 || this->tail > this->size || (this->tail == 0 && this->size > 0))
        return ERR_TAIL;
    if(this->free < 0)
        return ERR_FREE;
    if(this->data == NULL)
        return ERR_DATA;

    for(int i = 1; i < this->capacity; ++i)
    {
        if(*this->data[i].name != POISON && (this->data[i].prev == this->data[i].next || this->data[i].prev < 0 || this->data[i].prev > this->size || this->data[i].next < 0 || this->data[i].prev > this->size))
            return ERR_INDEX;

        if(this->data[i].prev != POISON && *this->data[i].name == POISON)
            return ERR_POISON;
    }

    return OK;
}

//==========================__DUMP_FILE__=======================
int List_ASI_dump_file(struct List_ASI* this)
{
    assert(this != NULL);

    int error = List_ASI_OK(this);

    char dumpfilename[MAX_STR_LEN] = "";
    sprintf(dumpfilename, "dumps/list_ASI_dump_%d.txt", this->ID);
    FILE* dumpfile = fopen(dumpfilename, "aw");
    #define DUMP_TAB_LN( var ) (fprintf(dumpfile, "\t%s = %d\n", strchr(#var, '>') + 1, var))

    fprintf(dumpfile, "List_ASI #%d dump:\n"
                      "{\n", this->ID);

    if(error == ERR_SIZE || error == ERR_SIZE_CAPACITY)
        fprintf(dumpfile, "!: ");
    DUMP_TAB_LN(this->size);

    if(error == ERR_CAPACITY || error == ERR_SIZE_CAPACITY)
        fprintf(dumpfile, "!: ");
    DUMP_TAB_LN(this->capacity);

    if(error == ERR_HEAD)
        fprintf(dumpfile, "!: ");
    DUMP_TAB_LN(this->head);

    if(error == ERR_TAIL)
        fprintf(dumpfile, "!: ");
    DUMP_TAB_LN(this->tail);

    if(error == ERR_FREE)
        fprintf(dumpfile, "!: ");
    DUMP_TAB_LN(this->free);

    #undef DUMP_TAB_LN

    if(error == ERR_DATA)
    {
        fprintf(dumpfile, "\tDATA ARRAY IS CRASHED !\n");
        fprintf(dumpfile, "}\n\n");

        fclose(dumpfile);
        return error;
    }

    fprintf(dumpfile, "\tmemory set\n"
                      "\t{\n");

    if(error == ERR_INDEX)
        fprintf(dumpfile, REDALERT"\tTHERE IS A BAD INDEX SOMEWHERE !\n"RESET);

    if(error == ERR_POISON)
        fprintf(dumpfile, REDALERT"\tTHERE IS A BAD DATA SOMEWHERE: NO NAME MUST BE \"-1\" !\n"RESET);

    fprintf(dumpfile, "\t\tindex | prev | next | index | name\n");

    for(int i = 0; i < this->capacity; ++i)
    {
        fprintf(dumpfile, "\t\t%5d |%5d |%5d | %5d | ", i, this->data[i].prev, this->data[i].next, i);
        if(*this->data[i].name != POISON)
        {
            fprintf(dumpfile, "\"");
            for(int k = 0; this->data[i].name[k] != 0; k++)
                fprintf(dumpfile, "%c", this->data[i].name[k]);
            fprintf(dumpfile, "\"\n");
        }
        else
            fprintf(dumpfile, "-1\n");
    }

    fprintf(dumpfile, "\t}\n");

    if(this->size)
    {
        fprintf(dumpfile, "\tactual list\n"
               "\t{\n");

        if(error == ERR_INDEX)
            fprintf(dumpfile, "\tTHE LIST MAY BE BAD !\n");

        fprintf(dumpfile, "\t\tindex | name\n");

        for(int i = this->head; i > 0; i = this->data[i].next)
        {
            fprintf(dumpfile, "\t\t%5d | ", i);
            if(*this->data[i].name != POISON)
            {
                fprintf(dumpfile, "\"");
                for(int k = 0; this->data[i].name[k] != 0; k++)
                    fprintf(dumpfile, "%c", this->data[i].name[k]);
                fprintf(dumpfile, "\"\n");
            }
            else
                fprintf(dumpfile, "-1\n");
        }

        fprintf(dumpfile, "\t}\n");
    }
    else
        fprintf(dumpfile, "\tactual list is empty\n");

    fprintf(dumpfile, "}\n\n");

    fclose(dumpfile);
    return error;
}

//==========================__DUMP_CONS__=======================
int List_ASI_dump(struct List_ASI* this)
{
    assert(this != NULL);

    int error = List_ASI_OK(this);

    printf("List_ASI #%d dump:\n"
           "{\n", this->ID);

    if(error == ERR_SIZE || error == ERR_SIZE_CAPACITY)
        printf(REDALERT);
    ECHO_TAB_LN(this->size);
    printf(RESET);

    if(error == ERR_CAPACITY || error == ERR_SIZE_CAPACITY)
        printf(REDALERT);
    ECHO_TAB_LN(this->capacity);
    printf(RESET);

    if(error == ERR_HEAD)
        printf(REDALERT);
    ECHO_TAB_LN(this->head);
    printf(RESET);

    if(error == ERR_TAIL)
        printf(REDALERT);
    ECHO_TAB_LN(this->tail);
    printf(RESET);

    if(error == ERR_FREE)
        printf(REDALERT);
    ECHO_TAB_LN(this->free);
    printf(RESET);

    if(error == ERR_DATA)
    {
        printf("\tDATA ARRAY IS CRASHED !\n");
        printf("}\n");

        return error;
    }

    printf("\tmemory set\n"
           "\t{\n");

    if(error == ERR_INDEX)
        printf(REDALERT"\tTHERE IS A BAD INDEX SOMEWHERE !\n"RESET);

    if(error == ERR_POISON)
        printf(REDALERT"\tTHERE IS A BAD DATA SOMEWHERE: NO NAME MUST BE \"-1\" !\n"RESET);

    printf(    BLUE"\t\tindex "RESET"|"GREEN" prev "RESET"|"RED" next "RESET"|"BLUE" index "RESET"| name\n");

    for(int i = 0; i < this->capacity; ++i)
    {
        if(i == MAX_PRINTED_MEMORY)
        {
            printf("\t\t  ... |  ... |  ... |   ... | ...\n"
                   "\t\t   for full list see dump file\n");
            break;
        }
        printf(BLUE  "\t\t%5d "RESET"|"GREEN  "%5d "RESET"|"RED  "%5d "RESET"|"BLUE  " %5d "RESET"| ", i, this->data[i].prev, this->data[i].next, i);
        if(*this->data[i].name != POISON)
        {
            printf("\"");
            for(int k = 0; this->data[i].name[k] != 0; k++)
                printf("%c", this->data[i].name[k]);
            printf("\"\n");
        }
        else
            printf("-1\n");
    }

    printf("\t}\n");

    if(this->size)
    {
        printf("\tactual list\n"
               "\t{\n");

        if(error == ERR_INDEX)
            printf(REDALERT"\tTHE LIST MAY BE BAD !\n"RESET);

        printf(BLUE"\t\tindex "RESET"| name\n");

        for(int i = this->head; i > 0; i = this->data[i].next)
        {
            printf(BLUE  "\t\t%5d "RESET"| ", i);
            if(*this->data[i].name != POISON)
            {
                printf("\"");
                for(int k = 0; this->data[i].name[k] != 0; k++)
                    printf("%c", this->data[i].name[k]);
                printf("\"\n");
            }
            else
                printf("-1\n");
        }

        printf("\t}\n");
    }
    else
        printf("\tactual list is empty\n");

    printf("}\n");

    List_ASI_dump_file(this);

    return error;
}

//==========================__MAKEGRAPH__=======================
int List_ASI_makegraph(struct List_ASI* this)
{
    assert(this != NULL);

    if(List_ASI_OK(this))
        return List_ASI_OK(this);

    char outfilename[MAX_STR_LEN] = "";
    sprintf(outfilename, "graphs/list_ASI_graph_%d.dot", this->ID);
    FILE* outfile = fopen(outfilename, "w");

    fprintf(outfile, "digraph list_ASI_graph_%d\n"
                      "{\n"
                      "rankdir = TB;\n"
                      "node [shape = Mrecord];\n"
                      "nodesep = 1.5\n", this->ID);

    int cur_idx = 0;
    int items_in_line = 0;
    int line[DOT_NODES_IN_LINE] = {};
    for( ; cur_idx < this->capacity; cur_idx++)
    {
        if(*this->data[cur_idx].name == POISON)
            continue;

        fprintf(outfile, "%d [label = \"<idx>%d | {<value> ", cur_idx, cur_idx);
        for(int k = 0; this->data[cur_idx].name[k] != 0; k++)
            fprintf(outfile, "%c", this->data[cur_idx].name[k]);
        fprintf(outfile, " | {<prev> %d | <next> %d}}\"];\n", this->data[cur_idx].prev, this->data[cur_idx].next);

        line[items_in_line++] = cur_idx;
        if(items_in_line == DOT_NODES_IN_LINE)
        {
            fprintf(outfile, "{rank = same");
            for(int i = 0; i < items_in_line; ++i)
                fprintf(outfile, "; %d", line[i]);
            fprintf(outfile, "}\n");
            items_in_line = 0;
        }
    }
    if(items_in_line > 0)
    {
        fprintf(outfile, "{rank = same");
        for(int i = 0; i < items_in_line; ++i)
            fprintf(outfile, "; %d", line[i]);
        fprintf(outfile, "}\n");
    }


    cur_idx = 0;
    while(*this->data[cur_idx].name == POISON)
        cur_idx++;

    fprintf(outfile, "%d", cur_idx);
    cur_idx++;

    for( ; cur_idx < this->capacity; cur_idx++)
    {
        if(*this->data[cur_idx].name == POISON)
            continue;
        fprintf(outfile, " -> %d", cur_idx);
    }
    fprintf(outfile, " [style = dotted] \n");

    cur_idx = this->data[this->head].next;
    while(cur_idx != 0)
    {
        fprintf(outfile, "%d:next -> %d:prev [color = blue penwidth = 2.0]\n", this->data[cur_idx].prev, cur_idx);
        cur_idx = this->data[cur_idx].next;
    }

    fprintf(outfile, "}\n");

    fclose(outfile);

    return List_ASI_OK(this);
}

//*****************************************************************

//==========================__REMOVE__==========================
int List_ASI_remove(struct List_ASI* this, int idx)
{
    assert(this != NULL);

    if(List_ASI_OK(this))
        return List_ASI_OK(this);

    if(idx < 0 || idx >= this->capacity || *this->data[idx].name == POISON)
    {
        printf(RED"LIST_DATA[%d]: NO SUCH ITEM\n"RESET, idx);
        return ERR_INDEX;
    }

    if(this->size == 1)
    {
        *this->data[idx].name = POISON;
        this->data[idx].next = this->free;
        this->free = idx;
        this->data[idx].prev = POISON;

        this->size = 0;
        this->tail = this->head = 0;
        return List_ASI_OK(this);
    }

    if(idx == this->head)
    {
        this->data[this->data[idx].next].prev = 0;
        this->head = this->data[idx].next;
    }
    else if(idx == this->tail)
    {
        this->data[this->data[idx].prev].next = 0;
        this->tail = this->data[idx].prev;
    }
    else
    {
        this->data[this->data[idx].prev].next = this->data[idx].next;
        this->data[this->data[idx].next].prev = this->data[idx].prev;
    }

    *this->data[idx].name = POISON;
    this->data[idx].next = this->free;
    this->free = idx;
    this->data[idx].prev = POISON;

    this->size--;

    return List_ASI_OK(this);
}

//==========================__POP__=============================
int List_ASI_pop(struct List_ASI* this)
{
    return List_ASI_remove(this, this->tail);
}

//*****************************************************************

//==========================__ADD_BEFORE__======================
int List_ASI_add_before(struct List_ASI* this, int* new_name, int idx)
{
    assert(this != NULL);
    assert(new_name != NULL);

    if(List_ASI_OK(this))
        return List_ASI_OK(this);

    if(idx < 0 || idx >= this->capacity || (idx > 0 && *this->data[idx].name == POISON))
    {
        printf(RED"LIST[%d]: NO SUCH ITEM\n"RESET, idx);
        return ERR_INDEX;
    }

    if(!this->free)
        expand_data(this);

    int new_place = this->free;
    this->free = this->data[this->free].next;

    linecopy(this->data[new_place].name, new_name);

    if(this->size == 0)
    {
        this->data[new_place].prev = this->data[new_place].next = 0;
        this->head = this->tail = new_place;
        this->size = 1;
        return List_ASI_OK(this);
    }

    if(idx == this->head)
    {
        this->data[new_place].prev = 0;
        this->data[new_place].next = this->head;

        this->data[idx].prev = new_place;
        this->head = new_place;
        this->size++;
        return List_ASI_OK(this);
    }

    this->data[new_place].prev = this->data[idx].prev;
    this->data[new_place].next = idx;

    this->data[this->data[idx].prev].next = new_place;
    this->data[idx].prev = new_place;

    this->size++;

    return List_ASI_OK(this);
}

//==========================__ADD_AFTER__=======================
int List_ASI_add_after(struct List_ASI* this, int* new_name, int idx)
{
    assert(this != NULL);
    assert(new_name != NULL);

    if(List_ASI_OK(this))
        return List_ASI_OK(this);

    if(idx < 0 || idx >= this->capacity || (idx > 0 && *this->data[idx].name == POISON))
    {
        printf(RED"LIST[%d]: NO SUCH ITEM\n"RESET, idx);
        return ERR_INDEX;
    }

    if(idx == 0)
        return List_ASI_add_before(this, new_name, 0);

    if(!this->free)
        expand_data(this);

    int new_place = this->free;
    this->free = this->data[this->free].next;

    linecopy(this->data[new_place].name, new_name);

    if(idx == this->tail)
    {
        this->data[new_place].prev = this->tail;
        this->data[new_place].next = 0;

        this->data[idx].next = new_place;
        this->tail = new_place;
        this->size++;
        return List_ASI_OK(this);
    }

    this->data[new_place].next = this->data[idx].next;
    this->data[new_place].prev = idx;

    this->data[this->data[idx].next].prev = new_place;
    this->data[idx].next = new_place;

    this->size++;

    return List_ASI_OK(this);
}

//==========================__PUSH__============================
int List_ASI_push(struct List_ASI* this, int* new_name)
{
    return List_ASI_add_after(this, new_name, (this->size != 0) ? this->tail : 0);
}

//==========================__ADD_SORTED__======================
int List_ASI_add_sorted(struct List_ASI* this, int* new_name)
{
    assert(this != NULL);
    assert(new_name != NULL);

    if(List_ASI_OK(this))
        return List_ASI_OK(this);

    int i = this->head;
    for( ; i != 0; i = this->data[i].next)
        if(linecomp(this->data[i].name, new_name) > 0)
            break;

    if(i != 0)
        List_ASI_add_before(this, new_name, i);
    else
        List_ASI_push(this, new_name);

    return List_ASI_OK(this);
}

//*****************************************************************

//==========================__CONSTRUCTOR__=====================
void List_ASI_constructor(struct List_ASI* this, int number)
{
    assert(this != NULL);

    char dumpfilename[MAX_STR_LEN] = "";
    sprintf(dumpfilename, "dumps/list_ASI_dump_%d.txt", number);
    FILE* dumpfile = fopen(dumpfilename, "w");
    fclose(dumpfile);

    this->ID = number;
    this->size = 0;
    this->capacity = 1;
    this->head = 0;
    this->tail = 0;
    this->free = 0;

    this->data = (struct List_ASI_item*) malloc(this->capacity * sizeof(struct List_ASI_item));
    *this->data[0].name = POISON;
    this->data[0].next = this->data[0].prev = 0;
    return;
}

//==========================__DESTRUCTOR__======================
void List_ASI_destructor(struct List_ASI* this)
{
    assert(this != NULL);

    this->ID = POISON;
    this->size = POISON;
    this->capacity = POISON;
    this->head = POISON;
    this->tail = POISON;
    this->free = POISON;

    free(this->data);
    this->data = NULL;
    return;
}

