#ifndef MACROHANDLER_H
#define MACROHANDLER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "structs.h"
#include "constants.h"



FILE *registerMacro(char *name, FILE *fp, Macro *mp, int *endCode, int *lineCount);
int findMacroIndex(char *name, Macro *macrotable, unsigned int size);
FILE *extendMacro(FILE *target, Macro *macrotable);
int isSingleWordLine(char *line);
int resizeMacroTable(Macro **table, unsigned int size);
int validateMacroName(char *name);
int isMacroName(char *name, Macro *macrotable, unsigned int size);
int isEmptyOrCommentLine(char *line);


#endif
