//
// Created by theofilos on 17/1/2017.
//

#ifndef YSBD_PROJECT2_UTIL_H
#define YSBD_PROJECT2_UTIL_H

#include "sort.h"

typedef enum Boolean {false, true} Boolean;

void int_quickSort( Record *record_array, int l, int r);
void string_quickSort( Record* record_array, int l, int r);

char* merge_files(char *file_name1, char *file_name2, int fieldNo);

char* make_file_name(int stage, int file_num);

int lenfinder(int x);

#endif //YSBD_PROJECT2_UTIL_H
