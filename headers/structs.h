#ifndef STRUCTS_H
#define STRUCTS_H

#include "constants.h"

typedef struct extList {
    char label[MAX_LABEL_LENGTH];
    int add;
    struct extList *next;
} extList;

/* Define the virtualMem struct */
typedef struct virtualMem {
    int address;
    int binaryCode;
    struct virtualMem *nextWord;
} virtualMem;

/* command lookup table */
typedef struct {
    char *name;
    int opcode;
    int num_operands;
    int legalAddrSrc;
    int legalAddrTarget;
} Command;

/* struct for label table */
typedef struct {
    char name[MAX_LABEL_LENGTH];
    int address;
    int isData; /* 1 for data, 0 for code */
    int isExtern;
    int isEntry;

} Label;

/* struct for macros */
typedef struct {
    char name[MAX_MACRO_NAME];
    char *lines;
} Macro;

#endif
