#ifndef VIRTUALMEMORY_H
#define VIRTUALMEMORY_H

#include <stdio.h>
#include <stdlib.h>

#include "structs.h"

int addNextWord(virtualMem *head, int index, int address, int binaryCode);
int resizeMemoryTable(virtualMem **table, int newSize);
void addEntry(virtualMem* table, int index, int address, int binaryCode);
int encodeAddr(virtualMem *TableIC, int index, int addr, int isExt);
int mergeMemoryTables(virtualMem **tableIC, virtualMem *tableDC, int *size_tableIC, int size_tableDC, int IC);
void freeMemoryTable(virtualMem **table, int size);

#endif
