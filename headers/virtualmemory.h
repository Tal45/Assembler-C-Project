#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

/*
 * addNextWord allocates, enlists and links a struct of virtualMem to the linked list, used to point to next word associated to
 * first word of command
 * parameters:
 *  head - head node of struct virtualMem, the first word of a command
 *  index - index of struct required
 *  address - machine code address
 *  binaryCode - binary encoding to store
 * returns:
 *  0 for successful operation, -1 otherwise
 */
int addNextWord(virtualMem *head, int index, int address, int binaryCode);

/*
 * resizeMemoryTable increases the size of a memory table by one struct
 * parameters:
 *  table - pointer to the virtualMem structs array
 *  newSize - size of the table
 * returns:
 *  0 for successful operation, -1 otherwise
 */
int resizeMemoryTable(virtualMem **table, int newSize);

/*
 * addEntry registers the binary code and address in a virutalMem struct
 * parameters:
 *  table - pointer to virtualMem structs array
 *  index - required index
 *  address - address to be stored
 *  binaryCode - encoding to store
 */
void addEntry(virtualMem* table, int index, int address, int binaryCode);

/*
 * encodeAddr used to complete a label's address in second pass, returns the word's address
 * parameters:
 *  TableIC - pointer to virtualMem structs array
 *  index - index required in array
 *  addr - address to be added
 *  isExt - extern flag indicator
 * returns:
 *  the address of the line
 */
int encodeAddr(virtualMem *TableIC, int index, int addr, int isExt);

/*
 * mergeMemoryTables used at the end of the first pass to merge both IC and DC tables into IC table, placing DC
 * memory at the bottom of the table
 * parameters:
 *  tableIC - pointer to virtualMem structs array
 *  tableDC - pointer to virtualMem structs array
 *  size_tableIC - pointer to store new size of tableIC
 *  size_tableDC - size of tableDC
 *  IC - current address at end of first pass
 * returns:
 *  0 if successful operation, -1 if error occurred
 */
int mergeMemoryTables(virtualMem **tableIC, virtualMem *tableDC, int *size_tableIC, int size_tableDC, int IC);

/*
 * freeMemoryTable used to free an array of structs and the linked nodes for each struct
 * parameters:
 *  table - pointer to virtualMem structs array
 *  size - size of array
 */
void freeMemoryTable(virtualMem **table, int size);

#endif
