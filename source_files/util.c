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

void copy_record(Record dest, Record src) {
    /* -----------------
     * Copy src to dest.
     * ----------------- */

    dest.id = src.id;
    strcpy(dest.name, src.name);
    strcpy(dest.surname, src.surname);
    strcpy(dest.city, src.city);
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

Record *merge_arrays(Record *array1, int size1, Record *array2, int size2, int fieldNo) {
    /* -------------------------------
     * Merge the arrays in an new one
     * with size size1+size2.
     * The arrays are sorted!!!
     * -------------------------------*/

    Record *merged_array;
    int recs_writen1 = 0, recs_writen2 = 0, recs_in_merged = 0;

    /* Initialize the 3rd array */
    if ((merged_array = malloc(sizeof(Record)*(size1+size2))) == NULL) {
        perror("*Error* , in allocating mem\n");
    }

    while (recs_writen1 < size1 || recs_writen2 < size2) {

        /* Both arrays still have records */
        if (recs_writen1 < size1 && recs_writen2 < size2) {
            if (compare_records(array1[recs_writen1], array2[recs_writen2], fieldNo) <= 0) {

                /* Writhe record to merge_array */
                copy_record(merged_array[recs_in_merged], array1[recs_writen1]);
                recs_writen1++;
            } else {/* ( record 1 > record 2 )*/

                /* Writhe record to merge_array */
                copy_record(merged_array[recs_in_merged], array2[recs_writen2]);
                recs_writen2++;
            }

            /* Update the indices */
            recs_in_merged++;
        }
        /* Array 1 has more records than array 2 */
        else if (recs_writen1 < size1 ) {

            /* Writhe record to merge_array */
            copy_record(merged_array[recs_in_merged], array1[recs_writen1]);
            recs_writen1++;
            recs_in_merged++;
        }
        /* Array 2 has more records than array 1 */
        else if (recs_writen2 < size2 ) {

            /* Writhe record to merge_array */
            copy_record(merged_array[recs_in_merged], array2[recs_writen1]);
            recs_writen2++;
            recs_in_merged++;
        }

        printf("We have size1=%d, size2=%d, recs1=%d, recs2=%d, merged_recs=%d\n", size1, size2,recs_writen1,recs_writen2,recs_in_merged);
        for (int x = 0 ; x < size1+size2 ; x++) {
            printRecord(&merged_array[x]);
        }
    }

    return merged_array;

}

char *merge_files(char *file_name1, char *file_name2, int fieldNo) {
    /* ------------------------------
     * Merge the 2 files into 1 and
     * return the file_name of the
     * newly created file.
     * -----------------------------*/
    Record *array1, *array2, *new_array;
    BlockInfo *blockInfo;
    int file_desc1, file_desc2, file_desc_new;
    int block_index1, block_index2, new_block_index;
    int block_for_merge1, block_for_merge2;
    int two_blocks_found;
    int old_stage, old_num, size1, size2;
    char *file_name;
    void *block1, *block2, *block_new_file;

    /* Initialize blockInfo*/
    if ((blockInfo = malloc(sizeof(BlockInfo))) == NULL) {
        perror("*Error* , in Allocating mem\n");
    }

    /* Open the 2 files */
    if ((file_desc1 = BF_OpenFile(file_name1)) < 0 || (file_desc2 = BF_OpenFile(file_name2)) < 0) {
        BF_PrintError("Error at merge_files, when opening file: ");
        exit(-1);
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

    if ((file_desc_new = BF_OpenFile(file_name)) < 0) {
        BF_PrintError("Error at merge_files, when opening new file: ");
        exit(-1);
    }


    /* Initialize */
    int blocks_for_new_file = 0;
    int end_of_blocks_f1 = 0, end_of_blocks_f2 = 0;
    block_index1 = 0;
    block_index2 = 0;
    new_block_index = 0;

    /* Read the block 0 from both files */
    if (BF_ReadBlock(file_desc1, block_index1, &block1) < 0 || BF_ReadBlock(file_desc2, block_index2, &block2) < 0) {
        BF_PrintError("Error at merge_files, when getting blocks: ");
        exit(-1);
    }

    /* Initializing - Casting for the arrays */
    array1 = (Record *) block1 + sizeof(BlockInfo);
    array2 = (Record *) block2 + sizeof(BlockInfo);

    /* Merge the files */
    while(block_index1 <= BF_GetBlockCounter(file_desc1) || block_index2 <= BF_GetBlockCounter(file_desc2)){

        /* Find the right 2 blocks to merge */
        two_blocks_found = 0;
        block_for_merge1 = -1;
        block_for_merge2 = -1;
        end_of_blocks_f1 = 0;
        end_of_blocks_f2 = 0;
        while (two_blocks_found <= 2) {

            /* If no more blocks in one file , take from the other file */
            if ( !end_of_blocks_f1 && !end_of_blocks_f2) {

                /* Decide what blocks to take by comparing the first records of each block */
                if (compare_records(array1[0], array2[0], fieldNo) <= 0) {

                    block_for_merge1 = block_index1;
                    block_index1++;
                    two_blocks_found++;
                } else {

                    block_for_merge2 = block_index2;
                    block_index2++;
                    two_blocks_found++;
                }
            } else if (end_of_blocks_f1) {

                block_for_merge2 = block_index2;
                block_index2++;
                two_blocks_found++;

            } else if (end_of_blocks_f2) {

                block_for_merge1 = block_index1;
                block_index1++;
                two_blocks_found++;
            }

            /* Read the next block from the right file */
            if (block_for_merge1 != -1) {

                /* Check if no more blocks */
                if (block_index1 <= BF_GetBlockCounter(file_desc1) - 1) {
                    end_of_blocks_f1 = 1;
                } else {

                    /* If the smaller was from file 1 then read the next from file 1 */
                    if (BF_ReadBlock(file_desc1, block_index1, &block1) < 0) {
                        BF_PrintError("Error at merge_files, when getting blocks: ");
                        exit(-1);
                    }

                    /* Initializing - Casting for the arrays */
                    array1 = (Record *) block1 + sizeof(BlockInfo);
                }
            } else if (block_for_merge2 != -1) {

                /* Check if no more blocks */
                if (block_index2 <= BF_GetBlockCounter(file_desc2) - 1) {
                    end_of_blocks_f2 = 1;
                } else {

                    /* If the smaller was from file 2 then read the next from file 2 */
                    if (BF_ReadBlock(file_desc1, block_index2, &block1) < 0) {
                        BF_PrintError("Error at merge_files, when getting blocks: ");
                        exit(-1);
                    }

                    /* Initializing - Casting for the arrays */
                    array2 = (Record *) block2 + sizeof(BlockInfo);
                }
            }

        }

        /* Find out the size of the arrays */
        memcpy(blockInfo, block1, sizeof(BlockInfo));
        size1 = (blockInfo->bytesInBlock - sizeof(BlockInfo)) / sizeof(Record);

        memcpy(blockInfo, block2, sizeof(BlockInfo));
        size2 = (blockInfo->bytesInBlock - sizeof(BlockInfo)) / sizeof(Record);

        /* Merge the arrays and get the new one */
        new_array = merge_arrays(array1, size1, array2, size2, fieldNo);

        /* Write the array to the new file in 2 new blocks */
        for (int block_num = 1; block_num <= 2; block_num++) {

            /* Allocate the block */
            if(BF_AllocateBlock(file_desc_new) != 0) {
               perror("Error at merge_files , when allocating blocks\n");
            }

            /* Read the block */
            if (BF_ReadBlock(file_desc1, BF_GetBlockCounter(file_desc_new)-1, &block_new_file) < 0) {
                BF_PrintError("Error at merge_files, when getting blocks: ");
                exit(-1);
            }

            /* write part of the array there */
            if (block_num == 1) {

                /* Write the block Info */
                blockInfo->bytesInBlock = size1*sizeof(Record);
                memcpy(block_new_file, blockInfo, sizeof(BlockInfo));

                memcpy(block_new_file + sizeof(BlockInfo), new_array, size1 * sizeof(Record));
            } else {

                /* Write the block Info */
                blockInfo->bytesInBlock = size2*sizeof(Record);
                memcpy(block_new_file, blockInfo, sizeof(BlockInfo));

                memcpy(block_new_file + sizeof(BlockInfo), &new_array[size1], size2 * sizeof(Record));
            }

            /* Write the block */
            if (BF_WriteBlock(file_desc1, BF_GetBlockCounter(file_desc_new)-1) < 0) {
                BF_PrintError("Error at merge_files, when writing blocks: ");
                exit(-1);
            }

        }

        /* Free */
        free(new_array);
        new_block_index++;
    }



    return file_name;
}
