#include "../BF/linux/BF.h"
#include "sort.h"
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
    int newBlockNeeded = 0; /* Shows whether a new block needs to be allocated to store the record */
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
        newBlockNeeded = 1;
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
            newBlockNeeded = 1;
        }
    }

    /* Insert the entry */
    if (newBlockNeeded == 1) {
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

void Sorted_SortFile(char *filename, int fieldNo) {
    return;
}

void Sorted_checkSortedFile(char *filename, int fieldNo) {
    return;
}

int Sorted_GetAllEntries(int fileDesc, int *fieldNo, void *value) {
    return 0;
}
