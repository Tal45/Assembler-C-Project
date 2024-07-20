#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "virtualmemory.h"
#include "structs.h"
#include "constants.h"

char *createFile(char *name, char *format);
int dumpMemoryToFile(char *filename, virtualMem *array, int sizeIC, int IC, int sizeDC);
int dumpExternsToFile(char *filename, extList *head);
int dumpEntriesToFile(const char *filename, Label labelTable[], int labelCount);
void removeFiles(char *filename);
int createDirectory(char *path);
char* concatenateDir(char* str1, char* str2);

#endif
