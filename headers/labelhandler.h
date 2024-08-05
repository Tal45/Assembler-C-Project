#ifndef LABELHANDLER_H
#define LABELHANDLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "constants.h"

/*
 * resizeLabelTable used to increase the labelTable array by one struct, allocating space for another label to be saved
 * parameters:
 *  labelTable - pointer to labelTable struct array
 *  labelCounter - number of labels in array
 * returns:
 *  0 to indicate success, 1 to indicate error
 */
int resizeLabelTable(Label **labelTable, int labelCounter);

/*
 * addLabel used to add a new label to the labelTable
 * parameters:
 *  labelTable - pointer to labelTable struct array
 *  labelCount - number of labels in array
 *  name - label's name
 *  address - label's address
 *  isData - indicates if the label is data type (.string, .data)
 *  isExt - indicates if the label is extern type
 *  isEnt - indicates if the label is entry type
 */
void addLabel(Label **labelTable, int labelCount, char *name, int address, int isData, int isExt, int isEnt);

/*
 * addLabel used to determine whether a string of input contatins a label declaration in it
 * parameters:
 *  token - the line of input
 * returns:
 *  1 to indicate label declaration, 0 otherwise
 */
int isLabel(char *token);

/*
 * isLabelDefined used the determine whether a label's name has already been defined in labelTable
 * parameters:
 *  token - string of input
 *  labelTable - pointer to labelTable struct array
 *  labelCount - count of labels in labelTable
 * returns:
 *  0 to indicate success, 1 to indicate error
 */
int isLabelDefined(char *token, Label **labelTable, int labelCount);

/*
 * updateDCOffset used to update the address of data types at the end of the first pass by adding IC to the address
 * parameters:
 *  table - pointer to labelTable
 *  labelCount - count of labels in labelTable
 *  IC - current address at end of first pass
 */
void updateDCOffset(Label **table, int labelCount, int IC);

/*
 * removeLabel used to skip the label declaration in input string
 * parameters:
 *  line - pointer to line of input string
 */
void removeLabel(char *line);

/*
 * findLabelIndex iterates the labelTable to find the index of a label
 * parameters:
 *  labelTable - pointer to labelTable array
 *  tableSize - size of labelTable
 *  labelName - label to be found
 * returns:
 *  index of label in labelTable if successful, -1 otherwise
 */
int findLabelIndex(Label **labelTable, int tableSize, char *labelName);

/*
 * enlistNode used to enlist a new node in the extern appearances linked list
 * parameters:
 *  extApp - pointer to linked list of extList struct
 *  label - name of label
 *  addr - address of appearance
 * returns:
 *  0 if successfully enlisted, 1 otherwise
 */
int enlistNode(extList **extApp, char *label, int addr);

/*
 * freeList used to free up allocated memory used in linked list of extern appearance
 * parameters:
 *  head - pointer to head of extList struct linked list
 */
void freeList(extList *head);

#endif
