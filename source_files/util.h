#ifndef YSBD_PROJECT2_UTIL_H
#define YSBD_PROJECT2_UTIL_H

#include "sort.h"

typedef enum Boolean {false, true} Boolean;

int isSortedArray(Record *record_array, int records, int fieldNo);
void printRecord(Record* recordptr);
void copy_record(Record *dest, Record *src);
int compare_records(Record record1, Record record2, int fieldNo);

void int_quickSort( Record *record_array, int l, int r);
void string_quickSort( Record* record_array, int l, int r, int fieldNo);

char* merge_files(char *file_name1, char *file_name2, int fieldNo);
char* make_file_name(int stage, int file_num);

int lenfinder(int x);

#endif
