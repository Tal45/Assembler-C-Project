#ifndef MACROHANDLER_H
#define MACROHANDLER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "structs.h"
#include "constants.h"


/*
 * registerMacro used to get a macro definition from a file by reading all of it's content
 * parameters:
 *  name - name of macro
 *  fp - file pointer to .as file
 *  mp - pointer to Macro struct for storing new macro
 *  endCode - pointer to endCode to indicate error in sub-routine
 *  lineCount - pointer to lineCount in order to keep count of line count in file
 * returns:
 *  pointer to .as file after macro has been fully read
 */
FILE *registerMacro(char *name, FILE *fp, Macro *mp, int *endCode, int *lineCount);

/*
 * findMacroIndex gets the macro table and finds it's index in the table by name
 * parameters:
 *  name - name of the macro to be found
 *  macrotable - pointer to array of Macro structs
 *  size - size of macro table
 * returns:
 *  index of macro definition in macro table
 */
int findMacroIndex(char *name, Macro *macrotable, unsigned int size);

/*
 * extendMacro used to dump all of a macro's lines defined into .am file in it's designated line
 * parameters:
 *  target - file pointer to target (.am) file
 *  macrotable - pointer to single struct of Macro to be extended
 * returns:
 *  pointer to current line in target (.am) file
 */
FILE *extendMacro(FILE *target, Macro *macrotable);

/*
 * isSingleWordLine gets a line of inputs and returns true if the line consists of a single word with no extra characters
 * parameters:
 *  line - pointer to line of input
 * returns:
 *  1 for true, 0 for false
 */
int isSingleWordLine(char *line);

/*
 * resizeMacroTable gets a pointer to array of Macro structs and increases it's size by one Macro struct
 * parameters:
 *  table - pointer to array of Macro structs
 *  size - current size of macro array
 * returns:
 *  0 for success, -1 otherwise
 */
int resizeMacroTable(Macro **table, unsigned int size);

/*
 * validateMacroName gets a name of a macro defined in .as file and assures the name isn't a reserved name
 * parameters:
 *  name - name of macro to be defined
 * returns:
 *  0 for validated macro name, -1 for invalid macro name
 */
int validateMacroName(char *name);

/*
 * isMacroName used to determine whether a string is a macro's name or not by looking it up in macro table
 * parameters:
 *  name - pointer to string consisting name suspected to be macro call
 *  macrotable - pointer to Macro structs array
 *  size - size of Macro structs array
 * returns:
 *  1 for true, 0 for false
 */
int isMacroName(char *name, Macro *macrotable, unsigned int size);

/*
 * isEmptyOrCommentLine gets a line of input and checks if line is whitespaces only or comment line
 * parameters:
 *  line - pointer to line of input
 * returns:
 *  1 for true, 0 for false
 */
int isEmptyOrCommentLine(char *line);


#endif
