//
// Created by theofilos on 20/1/2017.
//

/* Initializing - Casting for the arrays */
array1 = (Record*)(block1 + sizeof(BlockInfo));
array2 = (Record*)(block2 + sizeof(BlockInfo));

/* Find the right 2 blocks to merge */
from_f1_counter = 0;
from_f2_counter = 0;
block_for_merge1 = -1;
block_for_merge2 = -1;
file_for_block_1 = 0;
file_for_block_2 = 0;

while ((from_f1_counter + from_f2_counter < 2)
&& (end_of_blocks_f1 != 1 || from_f2_counter <1)
&& (end_of_blocks_f2 != 1 || from_f1_counter <1 )) {

printf("1: end1 %d, end2 %d , index1 %d , index2 %d\n",
end_of_blocks_f1, end_of_blocks_f2, block_index1, block_index2);

/* If no more blocks in one file , take from the other file */
if ( !end_of_blocks_f1 && !end_of_blocks_f2) {

/* Decide what blocks to take by comparing the first records of each block */
printf("Comparing :\n");
printRecord(&array1[0]);
printf("with:\n");
printRecord(&array2[0]);
if (compare_records(array1[0], array2[0], fieldNo) <= 0) {

/* Keep the the blockIndex in one of the 2 variables */
if ( block_for_merge1 != -1 ) {
block_for_merge2 = block_index1;
file_for_block_2 = file_desc1;
} else {
block_for_merge1 = block_index1;
file_for_block_1 = file_desc1;
}

/* Update the flags */
from_f1++;
from_f1_counter++;

/* Update indices */
block_index1++;
} else {

/* Keep the the blockIndex in one of the 2 variables */
if ( block_for_merge2 != -1 ) {
block_for_merge1 = block_index2;
file_for_block_1 = file_desc2;
} else {
block_for_merge2 = block_index2;
file_for_block_2 = file_desc2;
}

/* Update the flags */
from_f2++;
from_f2_counter++;

/* Update indices */
block_index2++;

}
} else if (end_of_blocks_f1) {

block_for_merge2 = block_index2;
file_for_block_2 = file_desc2;
block_index2++;
from_f2_counter++;
from_f2++;

} else if (end_of_blocks_f2) {

printf("here");
block_for_merge1 = block_index1;
file_for_block_1 = file_desc1;
block_index1++;
from_f1_counter++;
from_f1++;
}

printf("From f1 %d, f2 %d\n", from_f1, from_f2);
/* Read the next block from the right file */
if (from_f1) {

/* Check if no more blocks */
if (block_index1 > BF_GetBlockCounter(file_desc1) - 1) {
printf("skata\n");
end_of_blocks_f1 = 1;
} else {

/* If its the second time to take block from file 1 then use array 2 */
if (from_f1_counter == 2) {

/* If the smaller was from file 1 then read the next from file 1 */
if (BF_ReadBlock(file_desc1, block_index1, &block2) < 0) {
BF_PrintError("Error at merge_files, when getting blocks: ");
exit(-1);
}

/* Initializing - Casting for the arrays */
array2 = (Record *)(block2 + sizeof(BlockInfo));
} else {

printf("In from f1 in else \n");

/* If the smaller was from file 1 then read the next from file 1 */
if (BF_ReadBlock(file_desc1, block_index1, &block1) < 0) {
BF_PrintError("Error at merge_files, when getting blocks: ");
exit(-1);
}

/* And write it to the first array */
array1 = (Record *)(block1 + sizeof(BlockInfo));

printRecord(&array1[0]);
}
}
} else if (from_f2) {

/* Check if no more blocks */
if (block_index2 > BF_GetBlockCounter(file_desc2) - 1) {
printf("skata2\n");
end_of_blocks_f2 = 1;
} else {

/* If its the second time to take block from file 2 then use array 1 */
if (from_f2_counter == 2) {

/* If the smaller was from file 2 then read the next from file 2 */
if (BF_ReadBlock(file_desc2, block_index2, &block1) < 0) {
BF_PrintError("Error at merge_files, when getting blocks: ");
exit(-1);
}

/* Initializing - Casting for the arrays */
array1 = (Record *)(block1 + sizeof(BlockInfo));
} else {

printf("In from f2 in else \n");

/* If the smaller was from file 2 then read the next from file 2 */
if (BF_ReadBlock(file_desc2, block_index2, &block2) < 0) {
BF_PrintError("Error at merge_files, when getting blocks: ");
exit(-1);
}

/* Initializing - Casting for the arrays */
array2 = (Record *)(block2 + sizeof(BlockInfo));
}
}
}

/* Update flags */
from_f1 = 0;
from_f2 = 0;
}

printf("Out of loop merge blocks are %d , %d,f %d ,f %d\n", block_for_merge1, block_for_merge2, file_for_block_1, file_for_block_2);
printf("1: end1 %d, end2 %d , index1 %d , index2 %d\n",
end_of_blocks_f1, end_of_blocks_f2, block_index1, block_index2);

/* Get the 2 blocks from the files */
if (BF_ReadBlock(file_for_block_1, block_for_merge1, &block1) < 0) {
BF_PrintError("Error at merge_files, when getting blocks: ");
exit(-1);
}

if (BF_ReadBlock(file_for_block_2, block_for_merge2, &block2) < 0) {
BF_PrintError("Error at merge_files, when getting blocks: ");
exit(-1);
}


/* Find out the size of the arrays */
memcpy(blockInfo, block1, sizeof(BlockInfo));
size1 = (blockInfo->bytesInBlock - sizeof(BlockInfo)) / sizeof(Record);

memcpy(blockInfo, block2, sizeof(BlockInfo));
size2 = (blockInfo->bytesInBlock - sizeof(BlockInfo)) / sizeof(Record);

/* Initialize array 1 & array 2 */
if (( array1 = malloc(sizeof(Record)*size1)) == NULL || (array2 = malloc(sizeof(Record)*size2)) == NULL) {
perror("Error in merge_files, when allocating mem");
}

/* Take info for array 1 and array 2 */
memcpy(array1, block1 + sizeof(BlockInfo), sizeof(Record)*size1);
memcpy(array2, block2 + sizeof(BlockInfo), sizeof(Record)*size2);


/* Merge the arrays and get the new one */
new_array = merge_arrays(array1, size1, array2, size2, fieldNo);

/* Write the array to the new file using InsertEntry */
for (int record_num = 0; record_num < size1 + size2; record_num++ ) {
Sorted_InsertEntry(file_desc_new, new_array[record_num]);
}

printf("XXXXXXXXX-In loop-XXXXXXXX\n");
printDebug(file_desc_new);


/* Free */
free(array1);
free(array2);
free(new_array);
new_block_index++;