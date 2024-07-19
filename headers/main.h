#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "filehandler.h"
#include "labelhandler.h"

int preProcess(char *file, char *destfile);
int firstPass(char *amfile, Label *labelTable, int *labelCount, virtualMem **TableIC,
              int *sizeIC, virtualMem **TableDC, int *sizeDC, int *IC, int *entFlag);
int secondPass(char *amfile, Label *labelTable, int *labelCount, virtualMem **TableIC, extList **extApp);

#endif