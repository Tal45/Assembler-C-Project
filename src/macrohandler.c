#include "../headers/macrohandler.h"

int resizeMacroTable(Macro **table, unsigned int size) {
    Macro *tmp = NULL;

    if (size == 1) {
        tmp = (Macro *)malloc(size * sizeof(Macro));
        if (tmp == NULL) {
            printf("malloc: Memory allocation failed!\n");
            return ERROR; /* if malloc fails NULL returned to func the called this func to proper handle exit */
        }
        *table = tmp;
    } else {
        tmp = (Macro *)realloc(*table, size * sizeof(Macro));
        if (tmp == NULL) {
            printf("realloc: Memory allocation failed!\n");
            return ERROR; /* if realloc fails the exit handling will be done in the preProcess function */
        }
        *table = tmp;
    }
    (*table)[size-1].lines = NULL;

    return SUCCESS; /* great success */
}

FILE *registerMacro(char *name, FILE *fp, Macro *mp, int *endCode, int *lineCount) {
    char nextLine[MAX_INPUT_LINE];
    char checkEOL[MAX_INPUT_LINE];
    char *tmp = NULL;

    strcpy(mp->name,name);

    while(fgets(nextLine, MAX_INPUT_LINE,fp) != NULL && strstr(nextLine, "endmacr") == NULL) {
        (*lineCount)++;
        if (mp->lines == NULL) {
            mp->lines = (char *)malloc((strlen(nextLine) + 2) * CHAR_SIZE);
            if (mp->lines == NULL) {
                printf("malloc: Memory allocation failed!\n");
                *endCode = ERROR;
                break;
            }
            strcpy(mp->lines,nextLine);

        }
        else {
            tmp = (char *)realloc(mp->lines, (strlen(mp->lines) + strlen(nextLine) + 2) * sizeof(char *));
            if (tmp == NULL) {
                printf("realloc: Memory allocation failed!\n");
                *endCode = ERROR;
                break;
            }
            mp->lines = tmp;
            strcat(mp->lines, nextLine);
        }

    }
    (*lineCount)++;
    if (sscanf(nextLine, "%s %s", checkEOL, checkEOL) == 2) {
        printf("Error: extraneous text after end of macro definition! in line %d: %s\n", *lineCount, nextLine);
        *endCode = ERROR;
    }
    return fp;
}

int findMacroIndex(char *name, Macro *macrotable, unsigned int size) {
    int i;

    for (i = 0; i < size; i++) {
        if (strcmp(macrotable[i].name, name) == 0)
            return i;
    }
    return 0; /* dummy return assuming no issues in macro-stage as mentioned */
}

int isMacroName(char *name, Macro *macrotable, unsigned int size) {
    char tmp[MAX_MACRO_NAME];
    int i;
    sscanf(name, "%s", tmp);

    for (i = 0; i < size; i++) {
        if (strcmp(macrotable[i].name, tmp) == 0) {
            return 1;
        }
    }
    return 0;
}

FILE *extendMacro(FILE *target, Macro *macrotable) {
    fputs(macrotable->lines, target);
    return target;
}

int isSingleWordLine(char *line) {
    char buffer[MAX_INPUT_LINE];
    char *token;
    strncpy(buffer, line, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    token = strtok(buffer, " \t\n");
    if (token == NULL) {
        return 0;
    }

    /* try to get next token */
    token = strtok(NULL, " \t\n");
    if (token == NULL) {
        return 1;
    }

    return 0;
}

int validateMacroName(char *name) {
    int retCode = 0, i;
    const char *reservedNames[RESERVED_NAMES] = {
            /* command names */
            "mov",
            "cmp",
            "add",
            "sub",
            "lea",
            "clr",
            "not",
            "inc",
            "dec",
            "jmp",
            "bne",
            "red",
            "prn",
            "jsr",
            "rts",
            "stop",
            /* instruction names */
            "data",
            "string",
            "entry",
            "extern"
    };

    for (i = 0; i < RESERVED_NAMES; i++) {
        if (strcmp(name, reservedNames[i]) == 0) {
            retCode = ERROR;
            break;
        }
    }

    return retCode;
}

int isEmptyOrCommentLine(char *line) {
    /* check if input line is empty */
    if (line == NULL || *line == '\0') {
        return 1;
    }

    /* skip leading whitespace characters */
    while (*line != '\0' && isspace((unsigned char)*line)) {
        line++;
    }

    /* check if line is empty after whitespaces or if comment line */
    if (*line == '\0' || *line == ';') {
        return 1;
    }

    return 0; /* line is not empty */
}
