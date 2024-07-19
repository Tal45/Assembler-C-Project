#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <string.h>
#include "virtualmemory.h"
#include "structs.h"


char *createFile(char *name, char *format);
int dumpMemoryToFile(char *filename, virtualMem *array, int sizeIC, int IC, int sizeDC);
int dumpExternsToFile(char *filename, extList *head);
int dumpEntriesToFile(const char *filename, Label labelTable[], int labelCount);
void removeFiles(char *filename);

#endif
