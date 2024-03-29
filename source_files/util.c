#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../BF/linux/BF.h"
#include "util.h"

/* Prints a single record */
void printRecord(Record* recordptr) {
    printf("id: %d\n", recordptr->id);
    printf("Name: %s\n", recordptr->name);
    printf("Surname: %s\n", recordptr->surname);
    printf("City: %s\n\n", recordptr->city);
}

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

/*
 * Checks if an array of records is sorted based on attribute fieldNo
 * Returns 0 if it is sorted and -1 if not
 */
int isSortedArray(Record *record_array, int records, int fieldNo) {
    int i;

    for (i = 0; i < records-1; i++) {
        if (compare_records(record_array[i], record_array[i+1], fieldNo) == 1) {
            return -1;
        }
    }

    return 0;
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

int compare_records(Record record1, Record record2, int fieldNo) {
    /* --------------------------------
     * Compare record1 with record2
     * depending on the filedNo
     * Return : -1 if record1 < record2
     * Return : 0 if record1 = record2
     * Return : 1 if record1 > record2
     * -------------------------------- */

    /* if filedNo == 0 compare ints else compare strings */
    if (fieldNo == 0){
        if (record1.id > record2.id) return 1;
        else if (record1.id == record2.id) return 0;
        else return -1;
    } else if (fieldNo == 1) {
        if (strcmp(record1.name, record2.name) > 0) return 1;
        else if (strcmp(record1.name, record2.name) == 0) return 0;
        else return -1;
    } else if (fieldNo == 2) {
        if (strcmp(record1.surname, record2.surname) > 0) return 1;
        else if (strcmp(record1.surname, record2.surname) == 0) return 0;
        else return -1;
    } else { /* if (fieldNo == 3) */
        if (strcmp(record1.city, record2.city) > 0) return 1;
        else if (strcmp(record1.city, record2.city) == 0) return 0;
        else return -1;
    }
}

int compare_value_to_record(void *value, Record record, int fieldNo) {
    /* --------------------------------
     * Compare value with record2
     * depending on the filedNo
     * Return : -1 if value < record2
     * Return : 0 if value = record2
     * Return : 1 if value > record2
     * -------------------------------- */

    /* Cast the value to an int or string */
    int int_value;
    char str_value[25];

    if (fieldNo == 0) {
        int_value = *((int *)value);
    }
    else {
        strcpy(str_value, (char *)value);
    }

    /* if fieldNo == 0 compare ints else compare strings */
    if (fieldNo == 0){
        if (int_value > record.id) return 1;
        else if (int_value == record.id) return 0;
        else return -1;
    } else if (fieldNo == 1) {
        if (strcmp(str_value, record.name) > 0) return 1;
        else if (strcmp(str_value, record.name) == 0) return 0;
        else return -1;
    } else if (fieldNo == 2) {
        if (strcmp(str_value, record.surname) > 0) return 1;
        else if (strcmp(str_value, record.surname) == 0) return 0;
        else return -1;
    } else {
        if (strcmp(str_value, record.city) > 0) return 1;
        else if (strcmp(str_value, record.city) == 0) return 0;
        else return -1;
    }
}

void copy_record(Record *dest, Record *src) {
    /* -----------------
     * Copy src to dest.
     * ----------------- */

    dest->id = src->id;
    strcpy(dest->name, src->name);
    strcpy(dest->surname, src->surname);
    strcpy(dest->city, src->city);
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


int string_partition( Record *record_array, int l, int r, int fieldNo) {
/* ------------------------------
     * Partition the array , and put
     * everything in the right place.
     * ------------------------------ */
    int i, j;
    char *pivot;

    i = l; j = r+1;

    if (fieldNo == 1) {
        pivot = record_array[l].name;
    } else if (fieldNo == 2) {
        pivot = record_array[l].surname;
    } else { /* if (fieldNo == 3) */
        pivot = record_array[l].city;
    }

    while( 1)
    {

        if (fieldNo == 1) {
            do i++; while( strcmp(record_array[i].name, pivot) <= 0  && i <= r );
            do j--; while( strcmp(record_array[j].name, pivot) > 0 );
            if( i >= j ) break;

        } else if (fieldNo == 2) {
            do i++; while( strcmp(record_array[i].surname, pivot) <= 0  && i <= r );
            do j--; while( strcmp(record_array[j].surname, pivot) > 0 );
            if( i >= j ) break;

        } else { /* if (fieldNo == 3) */
            do i++; while( strcmp(record_array[i].city, pivot) <= 0  && i <= r );
            do j--; while( strcmp(record_array[j].city, pivot) > 0 );
            if( i >= j ) break;
        }

        swap_records(record_array, i, j);
    }
    swap_records(record_array, l, j);

    return j;
}

void string_quickSort( Record *record_array, int l, int r, int fieldNo)
{
    int j;

    if( l < r )
    {
        // divide and conquer
        j = string_partition( record_array, l, r, fieldNo);
        string_quickSort( record_array, l, j-1, fieldNo);
        string_quickSort( record_array, j+1, r, fieldNo);
    }

}

char* make_file_name(int stage, int file_num) {
    /* ---------------------------------
     * Makes a file name in the format :
     * <stage>"temp"<file_num>.
     * and the number of chars will be
     * len(stage) chars + 4 chars + len(file_num) chars.
     * --------------------------------- */

    char *file_name;

    /* Malloc space */
    if ((file_name = malloc(sizeof(char)*(lenfinder(stage)+4+lenfinder(file_num)+1))) == NULL) {
        printf("*Error* , in allocating mem\n");
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

    Record *array_f1, *array_f2, *merged_array;
    BlockInfo *blockInfo;
    int file_desc1, file_desc2, file_desc_new;
    int block_index_f1, block_index_f2 ;
    int old_stage, old_num, size1 = 0, size2 = 0;
    char *file_name;
    void *block1, *block2;

    /* Initialize blockInfo*/
    if ((blockInfo = malloc(sizeof(BlockInfo))) == NULL) {
        perror("Error in merge_files , when allocating mem");
    }

    /* Open the 2 files */
    file_desc1 = Sorted_OpenFile(file_name1);
    file_desc2 = Sorted_OpenFile(file_name2);

    if ( file_desc1 == -1 || file_desc2 == -1){
        printf("Error at reading files 1,2 %d %d\n",file_desc1, file_desc2 );
    }

    /* Create the new file name ,
     * new_stage is stage of filename2 plus 1
     * new_file_num is the num of filename2 / 2
     */

    /* Take stage and num from file 2 */
    sscanf(file_name2, "%dtemp%d",&old_stage, &old_num );

    /* Make the file name */
    file_name = make_file_name(old_stage+1, old_num/2);

    /* Create a new file - open the new file */
    Sorted_CreateFile(file_name);

    file_desc_new = Sorted_OpenFile(file_name);


    /* Initialize */
    int end_of_blocks_f1 = 0, end_of_blocks_f2 = 0;
    int recs_writen_fromf1 = 0, recs_writen_fromf2 = 0, recs_in_merged = 0;
    int block_f1_done = 0 , block_f2_done = 0;

    block_index_f1 = 1;
    block_index_f2 = 1;

    //printf("+-+-+-+-+-+-File %s , blocks :%d + File %s blocks :%d+-+-+-+-+-+-+-+\n",file_name1,BF_GetBlockCounter(file_desc1) ,file_name2, BF_GetBlockCounter(file_desc2));
    //printDebug(file_desc1);
    //printDebug(file_desc2);
    //printf("+-+-+-+-+-+-+-+||+-+-+-+-+-+-+-+\n");

    /* Initialize vars for the loop */
    end_of_blocks_f1 = 0;
    end_of_blocks_f2 = 0;

    /* Merge all the blocks from both files */
    while(block_index_f1 <= BF_GetBlockCounter(file_desc1)-1 || block_index_f2 <= BF_GetBlockCounter(file_desc2)-1 ){

        /* Initialize sizes to zero */
        size1 = 0 ; size2 = 0;

        /* Initialize flags to zero */
        block_f1_done = 0; block_f2_done = 0;


        /* Read block from file 1 it its exists */
        if ( block_index_f1 <= BF_GetBlockCounter(file_desc1) - 1) {

            if (BF_ReadBlock(file_desc1, block_index_f1, &block1) < 0) {
                BF_PrintError("Error at merge_files, when getting blocks: ");
                exit(-1);
            }

            /* Initializing - Casting for the arrays */
            array_f1 = (Record*)(block1 + sizeof(BlockInfo));

            /* Find out the size of the array */
            memcpy(blockInfo, block1, sizeof(BlockInfo));
            size1 = (blockInfo->bytesInBlock - sizeof(BlockInfo)) / sizeof(Record);

        } else {
            end_of_blocks_f1 = 1;
        }

        /* Read block from file 2 it it exists */
        if (block_index_f2 <= BF_GetBlockCounter(file_desc2) - 1) {


            if (BF_ReadBlock(file_desc2, block_index_f2, &block2) < 0) {
                BF_PrintError("Error at merge_files, when getting blocks: ");
                exit(-1);
            }

            /* Initializing - Casting for the arrays */
            array_f2 = (Record*)(block2 + sizeof(BlockInfo));


            /* Find out the size of the arrays */
            memcpy(blockInfo, block2, sizeof(BlockInfo));
            size2 = (blockInfo->bytesInBlock - sizeof(BlockInfo)) / sizeof(Record);

        } else {
            end_of_blocks_f2 = 1;
        }


        /* Merge in a new array and add it to file */

        /* Make an array to hold size1 + size 2*/
        /* Initialize the merge array */
        if ((merged_array = malloc(sizeof(Record)*(size1+size2))) == NULL) {
            perror("Error in merge_files , when allocating mem");
        }

        //printf("Going to loop !! with :r1 %d , r2 %d , end1 %d , end2 %d\n", recs_writen_fromf1, recs_writen_fromf2,
        //end_of_blocks_f1, end_of_blocks_f2);

        while (recs_writen_fromf1 < size1 || recs_writen_fromf2 < size2) {

            /* Both arrays still have records */
            if (recs_writen_fromf1 < size1 && recs_writen_fromf2 < size2 && end_of_blocks_f1 != 1 && end_of_blocks_f2 != 1) {
                if (compare_records(array_f1[recs_writen_fromf1], array_f2[recs_writen_fromf2], fieldNo) <= 0) {

                    /* Writhe record to new file */
                    Sorted_InsertEntry(file_desc_new, array_f1[recs_writen_fromf1]);

                    recs_writen_fromf1++;
                } else {/* ( record 1 > record 2 )*/

                    /* Writhe record to new file */
                    Sorted_InsertEntry(file_desc_new, array_f2[recs_writen_fromf2]);

                    recs_writen_fromf2++;
                }

                /* Update the indices */
                recs_in_merged++;
            }
                /* Array 1 has more records than array 2 or file 2 is done  */
            else if (recs_writen_fromf1 < size1 && (recs_writen_fromf2 == size2 || end_of_blocks_f2 == 1)){

                /* If file 2 has no more blocks*/
                if (end_of_blocks_f2) {

                    /* Writhe remaining records to new file from f1 */
                    Sorted_InsertEntry(file_desc_new, array_f1[recs_writen_fromf1]);

                    recs_writen_fromf1++;
                    recs_in_merged++;

                    /* check if we finished this block an we need to go to the next block for file 1*/
                    if (recs_writen_fromf1 == size1){

                        /* Increase the index - state that f1 blocks is done */
                        block_index_f1++;
                        block_f1_done = 1;
                    }
                } else {
                    /* Else start again and take the next block from file f2 */

                    /* Increase the index and state that f2 blocks is done */
                    block_index_f2++;
                    block_f2_done = 1;

                    break;
                }
            }
                /* Array 2 has more records than array 1 or file 1 is done  */
            else if (recs_writen_fromf2 < size2 && (recs_writen_fromf1 == size1 || end_of_blocks_f1 == 1)) {


                /* If file 2 has no more blocks*/
                if (end_of_blocks_f1) {

                    /* Writhe remaining records to new file from f2 */
                    Sorted_InsertEntry(file_desc_new, array_f2[recs_writen_fromf2]);

                    recs_writen_fromf2++;
                    recs_in_merged++;

                    /* check if we finished this block an we need to go to the next block for file 2*/
                    if (recs_writen_fromf2 == size2){

                        /* Increase the index - state that f1 blocks is done */
                        block_index_f2++;
                        block_f2_done = 1;
                    }

                } else {
                    /* Else start again and take the next block from file f1 */

                    /* Increase index f1*/
                    block_index_f1++;

                    block_f1_done = 1;
                    break;
                }

            }


            //printf("We have size1=%d, size2=%d, recs1=%d, recs2=%d, merged_recs=%d\n", size1, size2,recs_writen_fromf1,recs_writen_fromf2,recs_in_merged);
        }

        /* Write the so far merged array in the new file
        for (int record_num = 0; record_num < recs_writen_fromf1 + recs_writen_fromf2; record_num++ ) {
            Sorted_InsertEntry(file_desc_new, merged_array[record_num]);
        }*/

        /* If block f1 was done or block f2 was done , place their counters to zero */
        if (block_f1_done) {

            /* Initialize the records writen from f1 */
            recs_writen_fromf1 = 0;
        }
        if (block_f2_done) {

            /* Initialize the records writen from f2 */
            recs_writen_fromf2 = 0;
        }

        /* Free */
        free(merged_array);
    }

    //printf("+-+-+-+-+-+-+-+Produced files %s+-+-+-+-+-+-+-+\n", file_name);
    //printDebug(file_desc_new);
    //printf("+-+-+-+-+-+-+-+end+-+-+-+-+-+-+-+\n");

    /* Close all 3 files */
    Sorted_CloseFile(file_desc1);
    Sorted_CloseFile(file_desc2);
    Sorted_CloseFile(file_desc_new);

    /* Remove the 2 files */
    if (remove(file_name1) != 0)
        printf("Error , when removing file %s", file_name1);
    if(remove(file_name2) != 0)
        printf("Error , when removing file %s", file_name1);

    return file_name;

}

