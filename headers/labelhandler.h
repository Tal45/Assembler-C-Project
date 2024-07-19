#ifndef LABELHANDLER_H
#define LABELHANDLER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "structs.h"
#include "constants.h"

void addLabel(Label labelTable[], int *labelCount, char *name, int address, int isData, int isExt, int isEnt);
int isLabel(char *token);
int isLabelDefined(char *token, Label labelTable[], int labelCount);
void resetLabelTable(Label *labelTable, int size);
void removeLabel(char *line);
int findLabelIndex(Label *labelTable, int tableSize, char *labelName);
int enlistNode(extList **extApp, char *label, int addr);
void freeList(extList *head);

#endif
