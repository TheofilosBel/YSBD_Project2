#ifndef SORT_H
#define SORT_H

typedef struct Record {
    int id;
    char name[15];
    char surname[20];
    char city[25];
} Record;

/*
 * Info struct for every block
 * @bytesInBlock: bytes in the block after the info struct that contain records
 */
typedef struct BlockInfo {
    int bytesInBlock;
} BlockInfo;

/*
 * Info struct for every file
 * @isHeapFile: shows whether a file was created by Sorted_CreateFile or not
 *              in order to print errors
 */
typedef struct FileInfo {
    int isHeapFile;
} FileInfo;

/* Prints all the blocks of a file */
void printDebug(int fileDesc);

/* Create a file and initialise its info */
int Sorted_CreateFile(char *fileName);

/* Open a file and read its info */
int Sorted_OpenFile(char *fileName);

int Sorted_CloseFile(int fileDesc);

int Sorted_InsertEntry(int fileDesc, Record record);

void Sorted_SortFile(char *filename, int fieldNo);

void Sorted_checkSortedFile(char *filename, int fieldNo);

int Sorted_GetAllEntries(int fileDesc, int *fieldNo, void *value);

#endif