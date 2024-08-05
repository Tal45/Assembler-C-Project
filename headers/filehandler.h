#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <string.h>
#include "virtualmemory.h"
#include "structs.h"

/*
 * createFile function gets the name of the file, format and a flag indicating the directory
 * and returns a concated string accordingly
 * paramters:
 *  name - file's name
 *  format - file's format
 *  isOutput - bool flag to indicate if the file is an output file
 * returns:
 *  a new string containing file's name, format and directory if necessary
 */
char *createFile(char *name, char *format, int isOutput);

/*
 * dumpMemoryToFile function gets an array of virtualMem stacks and writes the virtualMemory into the output file
 * parameters:
 *  filename - name of the output file
 *  array - array consisting of virutalMem structs
 *  sizeIC - size of virutalMem array
 *  IC - current IC address in program
 *  sizeDC - size of data in virutalMemory
 * returns:
 *  0 to indicate success, 1 to indicate error
 */
int dumpMemoryToFile(char *filename, virtualMem *array, int sizeIC, int IC, int sizeDC);

/*
 * dumpExternsToFile gets a linked list of all extern usage appearances in the program
 * and writes them into .ext output file
 * parameters:
 *  filename - name of the output file
 *  head - head node of the linked list
 * returns:
 *  0 to indicate success, 1 to indicate error
 */
int dumpExternsToFile(char *filename, extList *head);

/*
 * dumpEntriesToFile gets the labelTable and dumps into .ent file all entry labels declared in program
 * parameters:
 *  filename - name of output file
 *  labelTable - labelTable array consisting of Label structs
 *  labelCount - number of labels in labelTable
 * returns:
 *  0 to indicate success, 1 to indicate error
 */
int dumpEntriesToFile(const char *filename, Label **labelTable, int labelCount);

/*
 * removeFiles gets the main file's name and removes all associated output files
 * parameters:
 *  filename - name of main file without formatting or directory
 */
void removeFiles(char *filename);

#endif
