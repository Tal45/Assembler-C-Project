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


int isCommand(char *cmd);
int validateSingleLabel(char *line);
int validateString(char *line);
int validateData(char *line);
void handleError(int linenum, char *line, int *err);
int processInstruction(char *line, int *IC, virtualMem **tableIC, int *sizeIC);
int encodeAddressingMode(int operandType);
int checkOperandTypes(char *operand);
int returnRegisterNumber(char *operand);
Command *findCommand(char *name, Command *commands);
char *getCmdName(char *line, char *word);
void trimCntrls(char *line);

#endif
