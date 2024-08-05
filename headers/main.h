#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "filehandler.h"
#include "labelhandler.h"

/*
 * preProcess function used to perform the pre-process stage of an assembler by reading the whole input file,
 * registering macros and remove unnecessary blank lines or comments, at the end of the function an .am file is created
 * parameters:
 *  file - name of target file to be processed
 *  destfile - name of destination file
 * returns:
 *  0 for successful operation, -1 if error occurred in process
 */
int preProcess(char *file, char *destfile);

/*
 * firstPass used to perform the first pass of two-pass assembler, reading the lines from .am file one by one
 * validating them, constructing a label table and virtual memory for the program with the encoding of each line
 * parameters:
 *  amfile - string containing name, format and directory of .am file
 *  labelTable - pointer to an array of Label structs
 *  labelCount - pointer to a counter for labels tracking
 *  TableIC - pointer to an array of virtualMem structs to keep encoding of IC commands
 *  sizeIC - size of TableIC
 *  TableDC - pointer to an array of virtualMem structs to keep encoding of DC lines
 *  sizeDC - size of TableDC
 *  IC - pointer to IC address tracker
 *  entflag - pointer to entry labels detection flag
 * returns:
 *  0 for successful operation, -1 if error occurred in process
 */
int firstPass(char *amfile, Label **labelTable, int *labelCount, virtualMem **TableIC,
              int *sizeIC, virtualMem **TableDC, int *sizeDC, int *IC, int *entFlag);

/*
 * secondPass used to perform the second pass of a two-pass assembler, reading the lines from .am files one by one,
 * completing label's address accordingly in the virtual memory table and recording extern labels calls in the code
 * parameters:
 *  amfile - string containing name, format and directory of .am file
 *  labelTable - pointer to an array of Label structs
 *  labelCount - pointer to a counter for labels tracking
 *  TableIC - pointer to an array of virtualMem structs
 *  extApp - linked list of extList structs to keep track of extern calls in code
 * returns:
 *  0 for successful operation, -1 if error occurred in process
 */
int secondPass(char *amfile, Label **labelTable, int *labelCount, virtualMem **TableIC, extList **extApp);

#endif
