#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#include "structs.h"
#include "constants.h"
#include "virtualmemory.h"
#include "labelhandler.h"

/*
 * isCommand used to determine if a string given is a legal command
 * parameters:
 *  cmd - string of suspected command name
 * returns:
 *  1 for true if the string is a legal command, 0 for false otherwise
 */
int isCommand(char *cmd);

/*
 * validateSingleLabel used to assure there is only one single label declared after entry/extern instructions
 * and that the format of the label is valid as instructed
 * parameters:
 *  line - pointer to input string
 * returns:
 *  1 for true (label validated), 0 otherwise
 */
int validateSingleLabel(char *line);

/*
 * validateString gets the string part after .string instruction and validates that all characters present are legal
 * and bounded correctly with double-quotes
 * parameters:
 *  line - pointer to input string
 * returns:
 *  1 for true, 0 for false
 */
int validateString(char *line);

/*
 * validateData gets the string part after .data instruction and validates that all characters present are legal
 * comma-seperated and terminated correctly.
 * parameters:
 *  line - pointer to input line
 * returns:
 *  1 for true, 0 for false
 */
int validateData(char *line);

/*
 * handleError used to set error code in err and print a message to user with line and line number
 * parameters:
 *  linenum - number of line where error occurred
 *  line - pointer to string of line where error occurred
 *  err - pointer to error code tracker
 */
void handleError(int linenum, char *line, int *err);

/*
 * processInstruction gets all input lines containing commands, parses and validates them and encodes them into tableIC
 * with the binary encoding accordingly
 * parameters:
 *  line - pointer to input line
 *  IC - pointer to IC address tracker
 *  tableIC - pointer to virtualMem structs array
 *  sizeIC - pointer to counter for the size of tableIC
 * returns:
 *  0 for successful operation, -1 otherwise
 */
int processInstruction(char *line, int *IC, virtualMem **tableIC, int *sizeIC);

/*
 * encodeAddessingMode gets an operand type and returns it's addressing mode
 * parameters:
 *  operandType - the type of the operand
 * returns:
 *  the addressing mode as follows:
 *  - 0001 for immediate
 *  - 0010 for label
 *  - 0100 for pointer to register
 *  - 1000 for register
 */
int encodeAddressingMode(int operandType);

/*
 * checkOperandTypes gets a string with an operand and determines it's type
 * parameters:
 *  operand - pointer to string of operand
 * returns:
 *  returns it's type as integer or 0 if illegal operand
 */
int checkOperandTypes(char *operand);

/*
 * returnRegisterNumber returns the number of the register from the string passed
 * parameters:
 *  opernad - pointer to string of operand
 * returns:
 *  the number of the register passed as operand
 */
int returnRegisterNumber(char *operand);

/*
 * findCommand used to retrieve a Command struct from the commands look up table containing information about the command
 * parameters:
 *  name - pointer to string with name of the command
 *  commands - lookup table of Commad structs
 * returns:
 *  single struct of Command type containing needed command or NULL if no command found
 */
Command *findCommand(char *name, Command *commands);

/*
 * getCmdName fetches the name of the command from an input string
 * parameters:
 *  line - pointer to the line of input
 *  word - pointer to cmdName array, the command name will be stored there
 * returns:
 *  pointer to string of input without command name
 */
char *getCmdName(char *line, char *word);

/*
 * trimCntrls used to trim the control characters at the end of each input line
 * parameters:
 *  line - pointer to string of input
 */
void trimCntrls(char *line);

#endif
