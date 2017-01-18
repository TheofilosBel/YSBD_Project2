//
// Created by theofilos on 17/1/2017.
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../BF/linux/BF.h"
#include "util.h"

int lenfinder(int x) {
    /* -------------------------
     * Find the length of a num.
     * ------------------------- */

    if(x>=1000000000) return 10;
    if(x>=100000000) return 9;
    if(x>=10000000) return 8;
    if(x>=1000000) return 7;
    if(x>=100000) return 6;
    if(x>=10000) return 5;
    if(x>=1000) return 4;
    if(x>=100) return 3;
    if(x>=10) return 2;
    return 1;
}

void swap_records(Record *record_array, int r1, int r2){
    /* -----------------------
     * Swap Records r1 and r2.
     * ----------------------- */

    Record temp;

    /* Keep r1 */
    temp.id = record_array[r1].id;
    strcpy(temp.name, record_array[r1].name);
    strcpy(temp.surname, record_array[r1].surname);
    strcpy(temp.city, record_array[r1].city);

    /* replace r2 with r1 */
    record_array[r1].id = record_array[r2].id;
    strcpy(record_array[r1].name, record_array[r2].name);
    strcpy(record_array[r1].surname, record_array[r2].surname);
    strcpy(record_array[r1].city, record_array[r2].city);

    /* replace r2 with keep */
    record_array[r2].id = temp.id;
    strcpy(record_array[r2].name, temp.name);
    strcpy(record_array[r2].surname, temp.surname);
    strcpy(record_array[r2].city, temp.city);

}

int int_partition( Record *record_array, int l, int r) {
    /* ------------------------------
     * Partition the array , and put
     * everything in the right place.
     * ------------------------------ */
    int i, j, pivot;

    i = l; j = r+1;
    pivot = record_array[l].id;

    while( 1)
    {
        do i++; while( record_array[i].id < pivot  && i <= r );
        do j--; while( record_array[j].id > pivot);
        if( i >= j ) break;

        swap_records(record_array, i, j);
    }
    swap_records(record_array, l, j);

    return j;
}

void int_quickSort( Record *record_array, int l, int r)
{
    int j;

    if( l < r )
    {
        // divide and conquer
        j = int_partition( record_array, l, r);
        int_quickSort( record_array, l, j-1);
        int_quickSort( record_array, j+1, r);
    }

}


int string_partition( Record *record_array, int l, int r) {
/* ------------------------------
     * Partition the array , and put
     * everything in the right place.
     * ------------------------------ */
    int i, j;
    char *pivot;

    i = l; j = r+1;

    pivot = record_array[l].surname;

    while( 1)
    {
        do i++; while( strcmp(record_array[i].surname, pivot) <= 0  && i <= r );
        do j--; while( strcmp(record_array[j].surname, pivot) > 0 );
        if( i >= j ) break;

        swap_records(record_array, i, j);
    }
    swap_records(record_array, l, j);

    return j;
}

void string_quickSort( Record *record_array, int l, int r)
{
    int j;

    if( l < r )
    {
        // divide and conquer
        j = string_partition( record_array, l, r);
        string_quickSort( record_array, l, j-1);
        string_quickSort( record_array, j+1, r);
    }

}

char* make_file_name(int stage, int file_num) {
    /* ---------------------------------
     * Makes a file name in the format :
     * <stage>"temp"<file_num>.
     * --------------------------------- */

    char *file_name;

    /* Malloc space */
    if ((file_name = malloc(sizeof(char)*(lenfinder(stage)+4+lenfinder(file_num)+1))) == NULL) {
        printf("Error , in allocating mem\n");
    }

    /* Make the file name */
    sprintf(file_name, "%dtemp%d", stage, file_num);

    return file_name;
}


/*-=-=-=-=-=-=-=-=-=- Merging Functions -=-=-=-=-=-=-=-=-=-*/
char *merge_files(char *file_name1, char *file_name2, int fieldNo) {
    /* ------------------------------
     * Merge the 2 files into 1 and
     * return the file_name of the
     * newly created file.
     * -----------------------------*/
    int file_desc1, file_desc2, file_desc_new;
    int old_stage, old_num;
    char *file_name;

    /* Open the 2 files
    if ((file_desc1 = BF_OpenFile(file_name1)) < 0) {
        BF_PrintError("Error at merge_files, when opening file: ");
        exit(-1);
    }

    if ((file_desc2 = BF_OpenFile(file_name2)) < 0) {
        BF_PrintError("Error at merge_files, when opening file: ");
        exit(-1);
    }*/

    /* Create the new file
     * The format will be :
     * <stage>"temp"<File_number> ,
     * and the number of chars will be
     * len(stage) chars + 4 chars + len(file_num) chars.
     * Also :
     * new_stage is stage of filename2 plus 1
     * new_file_num is the num of filename2 / 2
     */

    /* Take stage and num from file 2 */
    sscanf(file_name2, "%dtemp%d",&old_stage, &old_num );

    file_name = make_file_name(old_stage+1, old_num/2);

    return file_name;
}
