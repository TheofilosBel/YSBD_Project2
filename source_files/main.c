#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sort.h"
#include "../BF/linux/BF.h"
#define fileName "heapFile"

/*
 * Reads a file from the input stream and writes
 * all of its entries to a file (using Sorted_InsertEntry)
 * organised in blocks creating the initial file to be sorted
 */
void insert_Entries(int fd) {
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    stream = stdin;
    Record record;

    while ((read = getline(&line, &len, stream)) != -1) {
        line[read - 2] = 0;
        char *pch;

        pch = strtok(line, ",");
        record.id = atoi(pch);

        pch = strtok(NULL, ",");
        pch++;
        pch[strlen(pch) - 1] = 0;
        strncpy(record.name, pch, sizeof(record.name));

        pch = strtok(NULL, ",");
        pch++;
        pch[strlen(pch) - 1] = 0;
        strncpy(record.surname, pch, sizeof(record.surname));

        pch = strtok(NULL, ",");
        pch++;
        pch[strlen(pch) - 1] = 0;
        strncpy(record.city, pch, sizeof(record.city));

        assert(!Sorted_InsertEntry(fd, record));
    }

    free(line);
    return;
}

int main(int argc, char **argv) {
    int fd;
    int fieldNo;
   
    BF_Init();


    //create heap file
    if (Sorted_CreateFile(fileName) == -1)
        printf("Error creating file!\n");

    fd = Sorted_OpenFile(fileName);
    if (fd == -1)
        printf("Error opening file!\n");
    insert_Entries(fd);

    //printDebug(fd);

    //sort heap file using 2-way merge-sort
    /*
    if (Sorted_SortFile(fileName,0) == -1  )
        printf("Error sorting file!\n");
    
    if (Sorted_checkSortedFile("heapFile", 0) == -1  )
        printf("Error sorting file!\n");
    */
    //get all entries with value
    //char value[20];
    //strcpy(value, "Keratsini");

    fieldNo = 0;
    int value = 11903588;    
/*
    fd = Sorted_OpenFile("heapFileSorted0");
    if( fd == -1  )
        printf("Error opening file!\n");
*/
    Sorted_GetAllEntries(fd,&fieldNo,&value);

    return EXIT_SUCCESS;
}