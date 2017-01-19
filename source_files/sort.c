#include "../BF/linux/BF.h"
#include "sort.h"
#include "util.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Prints a single record */
void printRecord(Record* recordptr){
    printf("id: %d\n", recordptr->id);
    printf("Name: %s\n", recordptr->name);
    printf("Surname: %s\n", recordptr->surname);
    printf("City: %s\n\n", recordptr->city);
}

void printDebug(int fileDesc) {
    BlockInfo tempInfo;
    void *block;
    int offset;
    int blockIndex = 1;
    Record recordTemp;

    while (blockIndex < BF_GetBlockCounter(fileDesc)) {
        offset = 0;
        /* Read block with num blockIndex */
        if (BF_ReadBlock(fileDesc, blockIndex, &block) < 0) {
            BF_PrintError("Error at printDebug, when getting block: ");
            return;
        }

        printf("----------------------------------------\n");
        memcpy(&tempInfo, block, sizeof(BlockInfo));
        printf("Block %d - bytesInBlock = %d\n\n", blockIndex, tempInfo.bytesInBlock);
        offset += sizeof(BlockInfo);

        while (offset < tempInfo.bytesInBlock) {
            memcpy(&recordTemp, block + offset, sizeof(Record));
            offset += sizeof(Record);
            printRecord(&recordTemp);
        }

        blockIndex++;
    }
}





int Sorted_CreateFile(char *fileName) {
    int fileDesc;
    void *block;
    FileInfo *fileInfo;

    /* Allocate memory and initialise the fileInfo struct */
    if ((fileInfo = malloc(sizeof(FileInfo))) == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return -1;
    }
    fileInfo->isHeapFile = 1;

    /* Create a new BF file and write the info in the first block (block0) */
    if (BF_CreateFile(fileName) < 0) {
        BF_PrintError("Error at Sorted_CreateFile, when creating file: ");
        return -1;
    }

    /* Open the new file */
    if ((fileDesc = BF_OpenFile(fileName)) < 0) {
        BF_PrintError("Error at Sorted_CreateFile, when opening file: ");
        return -1;
    }

    /* Allocate a block for the info */
    if (BF_AllocateBlock(fileDesc) < 0) {
        BF_PrintError("Error at Sorted_CreateFile, when allocating block: ");
        return -1;
    }

    /* Read block with num 0 */
    if (BF_ReadBlock(fileDesc, 0, &block) < 0) {
        BF_PrintError("Error at Sorted_CreateFile, when getting block: ");
        return -1;
    }

    /* Write the info in the first block */
    memcpy(block, fileInfo, sizeof(FileInfo));

    /* Write the block back */
    if (BF_WriteBlock(fileDesc, 0) < 0){
        BF_PrintError("Error at Sorted_CreateFile, when writing block back");
        return -1;
    }

    free(fileInfo);
    return 0;
}

int Sorted_OpenFile(char *fileName) {
    int fileDesc;
    void *block;
    FileInfo *fileInfo;

    /* Allocate memory and initialise the fileInfo struct */
    if ((fileInfo = malloc(sizeof(FileInfo))) == NULL) {
        fprintf(stderr, "Error allocating memory.\n");
        return -1;
    }

    /* Open the BF file */
    if ((fileDesc = BF_OpenFile(fileName)) < 0) {
        BF_PrintError("Error at Sorted_OpenFile, when opening file: ");
        return -1;
    }

    /* Read the info block (block0) */
    if (BF_ReadBlock(fileDesc, 0, &block) < 0) {
        BF_PrintError("Error at Sorted_OpenFile, when getting block: ");
        return -1;
    }

    /* Read the info */
    memcpy(fileInfo, block, sizeof(FileInfo));

    /* Not a heap file */
    if (fileInfo->isHeapFile != 1) {
        fprintf(stderr, "File is not a heap file.\n");
        return -1;
    }

    free(fileInfo);
    return fileDesc;
}

int Sorted_CloseFile(int fileDesc) {
    if (BF_CloseFile(fileDesc) < 0) {
        BF_PrintError("Error at Sorted_CloseFile, when closing file: \n");
        return -1;
    }

    return 0;
}

int Sorted_InsertEntry(int fileDesc, Record record) {
    Boolean newBlockNeeded = false; /* Shows whether a new block needs to be allocated to store the record */
    BlockInfo *blockInfo;
    void *block;

    /* Allocate memory for the blockInfo struct */
    blockInfo = malloc(sizeof(BlockInfo));
    if (blockInfo == NULL) {
        printf("Error allocating memory.\n");
        return -1;
    }

    /* Check if a new block is needed */
    if (BF_GetBlockCounter(fileDesc) == 1) {
        /* The file only contains the Info Block so a new block is needed */
        newBlockNeeded = true;
    }
    else {
        /* Read the last block of the file */
        if (BF_ReadBlock(fileDesc, BF_GetBlockCounter(fileDesc)-1, &block) < 0) {
            BF_PrintError("Error at Sorted_InsertEntry, when getting block: ");
            return -1;
        }

        /* Get its info and check its capacity */
        memcpy(blockInfo, block, sizeof(BlockInfo));
        if (blockInfo->bytesInBlock + sizeof(Record) > BLOCK_SIZE) {
            newBlockNeeded = true;
        }
    }

    /* Insert the entry */
    if (newBlockNeeded == true) {
        /* Allocate a new block */
        if (BF_AllocateBlock(fileDesc) < 0) {
            BF_PrintError("Error at Sorted_InsertEntry, when allocating block: ");
            return -1;
        }

        /* Read the new block */
        if (BF_ReadBlock(fileDesc, BF_GetBlockCounter(fileDesc)-1, &block) < 0) {
            BF_PrintError("Error at Sorted_InsertEntry, when getting block: ");
            return -1;
        }

        /* Initialise the blockInfo and write it to the block */
        blockInfo->bytesInBlock = sizeof(BlockInfo) + sizeof(Record);
        memcpy(block, blockInfo, sizeof(BlockInfo));

        /* Write the record after the info */
        memcpy(block + sizeof(BlockInfo), &record, sizeof(Record));

        /* Write the block back */
        if (BF_WriteBlock(fileDesc, BF_GetBlockCounter(fileDesc)-1) < 0){
            BF_PrintError("Error at Sorted_InsertEntry, when writing block back");
            return -1;
        }
    }
    else {
        /* Read the last block */
        if (BF_ReadBlock(fileDesc, BF_GetBlockCounter(fileDesc)-1, &block) < 0) {
            BF_PrintError("Error at Sorted_InsertEntry, when getting block: ");
            return -1;
        }

        /* Get its info */
        memcpy(blockInfo, block, sizeof(BlockInfo));

        /* Write the record at the end of the block */
        memcpy(block + blockInfo->bytesInBlock, &record, sizeof(Record));

        /* Update its info */
        blockInfo->bytesInBlock += sizeof(Record);
        memcpy(block, blockInfo, sizeof(BlockInfo));

        /* Write the block back */
        if (BF_WriteBlock(fileDesc, BF_GetBlockCounter(fileDesc)-1) < 0){
            BF_PrintError("Error at Sorted_InsertEntry, when writing block back");
            return -1;
        }
    }

    free(blockInfo);
    return 0;
}

int Sorted_SortFile(char *filename, int fieldNo) {
    /*
     * First sort internally
     * -----------------------*/
    BlockInfo block_info;
    Record *record_array;
    int block_num = 0, file_desc_father, records, offset;
    void *block;

    /* Open file */
    if ((file_desc_father = BF_OpenFile(filename)) < 0) {
        BF_PrintError("Error at Sorted_ShortFile, when opening file: ");
        exit(-1);
    }

    /* Internal shorting of all blocks */
    for (block_num = 1; block_num < BF_GetBlockCounter(file_desc_father); block_num++) {

        /* Read each block */
        if (BF_ReadBlock(file_desc_father, block_num, &block) < 0) {
            BF_PrintError("Error at Sorted_ShortEntry, when getting block: ");
            exit(-1);
        }

        /* Take blocks info */
        memcpy(&block_info, block, sizeof(BlockInfo));

        /* Count the records in the block */
        records = (block_info.bytesInBlock - sizeof(BlockInfo)) / sizeof(Record);

        /*Create a table of records to pass to the quick sort func */
        if ((record_array = malloc(sizeof(Record)*records)) == NULL){
            printf("Error allocating mem\n");
            exit(-1);
        }

        offset = sizeof(BlockInfo);
        for (int x = 0 ; x < records ; x++) {
            memcpy(&record_array[x],block + offset, sizeof(Record));
            offset += sizeof(Record);
        }

        /* Call quick sort depending on filedNo*/
        if ( fieldNo == 0 ) {
            int_quickSort(record_array, 0, records - 1);
        } else {
            string_quickSort(record_array, 0, records - 1);
        }

        /*
        for (int x = 0 ; x < records ; x++) {
            printRecord(&record_array[x]);
        }
        */

        /* Write back the records */
        memcpy(block, record_array, sizeof(record_array));

        /* Write the block back */
        if (BF_WriteBlock(file_desc_father, block_num) < 0){
            BF_PrintError("Error at Sorted_InsertEntry, when writing block back");
            return -1;
        }

        /* Free the array */
        free(record_array);
    }

    /* External 2 way shorting */
    int num_of_files = 0;
    int stage = 1;
    int curr_file_1, curr_file_2;
    int *file_desc_array;
    char *file_name, *file_name1, *file_name2;

    /* Initialize vars */
    num_of_files = BF_GetBlockCounter(file_desc_father) - 1;
    printf("Num of files %d\n", num_of_files);

    /* Create the files needed for stage 2 */
    for (int file = 0; file < num_of_files ; file++) {

        /* Create the file name. */
        file_name = make_file_name(stage, file);
        printf("File name is %s\n", file_name);

        /* Create a new BF file */
        if (BF_CreateFile(file_name) < 0) {
            BF_PrintError("Error at Sorted_CreateFile, when creating file: ");
            return -1;
        }

        /* Free */
        free(file_name);
    }

    while (num_of_files != 0) {

        /* Initialize the array of the fd's */
        if ((file_desc_array =malloc(sizeof(int)*num_of_files)) == NULL) {
            printf("Error , in allocating mem\n");
            exit(-1);
        }

        /* Pick the 2 files for merging */
        curr_file_1 = 0;
        curr_file_2 = 1;

        /* Merge the two files */
        for (int i = 0 ; i < num_of_files/2 ; i++) {

            /* Make the merging files , file names */
            file_name1 = make_file_name(stage, curr_file_1);
            file_name2 = make_file_name(stage, curr_file_2);

            file_name = merge_files(file_name1, file_name2, fieldNo);
            printf("%s + %s -> %s\n", file_name1, file_name2, file_name);

            /* update the curr files indices */
            curr_file_1 += 2;
            curr_file_2 = curr_file_1 + 1;

            /* Free */
            free(file_name1);
            free(file_name2);
        }

        /* Update the number of files for the next stage and the stage */
        //num_of_files = num_of_files / 2 + num_of_files % 2;
        num_of_files = 0;
        stage++;

        free(file_desc_array);
    }

    return 0;
}

void Sorted_checkSortedFile(char *filename, int fieldNo) {
    return;
}

int Sorted_GetAllEntries(int fileDesc, int *fieldNo, void *value) {
    return 0;
}
