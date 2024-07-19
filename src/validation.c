#include "../headers/validation.h"

int isCommand(char *cmd) {
    int i;
    const char *commands[] = {
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
            "stop"
    };

    /* all legal commands */
    if (iscntrl(cmd[strlen(cmd) - 1])) { /* remove unnecessary controls */
        cmd[strlen(cmd) - 1] = '\0';
    }

    /* iterate list of legal commands and look for match */
    for (i = 0; i <= NUM_OF_COMMANDS; i++) {
        if (strcmp(cmd, commands[i]) == 0) {
            return 1;
        }
    }

    /* no match found, print an error and return false */
    printf("Error: invalid command name %s\n", cmd);
    return 0;
}

int validateSingleLabel(char *line) {
    char *labelPtr;

    /* find .extern or .entry part in string and skip it */
    labelPtr = strstr(line, ".extern");
    if (labelPtr != NULL) {
        labelPtr += strlen(".extern");
    } else {
        labelPtr = strstr(line, ".entry");
        labelPtr += strlen(".entry");
    }

    while (isspace(*labelPtr)) { /* skip leading whitespace */
        labelPtr++;
    }

    if (!isalpha(*labelPtr)) { /* first character must be alphabetic character */
        printf("Error: invalid label name!\n");
        return 0;
    }

    while (isalnum(*labelPtr)) { /* skip rest of alpha-numeric characters */
        labelPtr++;
    }

    while (isspace(*labelPtr)) { /* skip trailing whitespace */
        labelPtr++;
    }

    if (*labelPtr == '\0' || *labelPtr == '\n') {
        return 1;
    } else {
        printf("Error: illegal terminator or character at end of label\n");
        return 0;
    }
}

int validateString(char *line) {
    /* find ".string" part in input and skip it */
    char *stringPtr = strstr(line, ".string");
    stringPtr += strlen(".string");

    while (isspace(*stringPtr)) { /* skip whitespaces */
        stringPtr++;
    }

    /* check if first char is double quote */
    if (*stringPtr != '"') {
        printf("Error: no opening double quote found in .string instruction!\n");
        return 0;
    }
    /* skip double quote */
    stringPtr++;

    while (*stringPtr != '\0') { /* iterate untill closing quote found */
        if (*stringPtr == '"') {
            stringPtr++; /* skip closing quote */

            while (isspace(*stringPtr)) { /* skip trailing whitespaces */
                stringPtr++;
            }
            if (*stringPtr == '\0' || *stringPtr == '\n') {
                return 1; /* return true if end of line found */
            } else {
                printf("Error: extraneous characters after closing quote in .string instruction!\n");
            }
        }
        stringPtr++;
    }

    /* no closing double quote found */
    printf("Error: no closing double quote found in .string instruction!\n");
    return 0;
}

int validateData(char *line) {
    char *endPtr;
    long int num;
    /* find ".data" in input line and increment the pointer to skip it */
    char *dataPtr = strstr(line, ".data");
    dataPtr += strlen(".data");

    /* skip whitespaces */
    while (isspace(*dataPtr)) {
        dataPtr++;
    }

    /* iterate untill end of input line */
    while (*dataPtr != '\0') {
        if (*dataPtr == '+' || *dataPtr == '-') { /* skip plus/minus sign */
            dataPtr++;
        }

        num = strtol(dataPtr, &endPtr, BASE_DECIMAL);

        /* check if strtol didn't consume any character or if num is out of range */
        if (dataPtr == endPtr || num < INT_MIN || num > INT_MAX) {
            return 0;
        }
        dataPtr = endPtr;

        /* skip whitespaces */
        while (isspace(*dataPtr)) {
            dataPtr++;
        }
        /* check for comma after number and skip it */
        if (*dataPtr == ',') {
            dataPtr++;
            while (isspace(*dataPtr)) { /* skip whitespace */
                dataPtr++;
            }
            /* check if there is number or sign after comma (no extra comma) */
            if (*dataPtr == '\0' || (!isdigit(*dataPtr) && *dataPtr != '-' && *dataPtr != '+')) {
                return 0;
            }
        } else if (*dataPtr != '\0' && !isspace(*dataPtr)) {
            return 0;
        } else if (isspace(*dataPtr)) {
            while (isspace(*dataPtr)) { /* skip whitespaces */
                dataPtr++;
            }
            if (*dataPtr != '\0' && *dataPtr != ',') {
                return 0;
            }
        }
    }

    return 1;
}

void trimCntrls(char *line) {
    while (iscntrl(line[strlen(line)-1])) { /* trim control characters from end like \r\n */
        line[strlen(line)-1] = '\0';
    }
}

void handleError(int linenum, char *line, int *err) {
    *err = 1;
    trimCntrls(line);
    printf("Error: occurred in line %d: %s\n", linenum, line);
}

void updateDCOffset(Label table[], int labelCount, int IC) {
    int i;

    for (i = 0; i < labelCount; i++) {
        if (table[i].isData) {
            table[i].address += IC;
        }
    }
}

/* remove label from input string */
void removeLabel(char *line) {
    char *colon = strchr(line, ':');
    if (colon) {
        memmove(line, colon + 1, strlen(colon + 1) + 1);
    }
}

/* fetch cmd name */
char *getCmdName(char *line, char *word) {
    while (isspace(*line)) line++;
    while (*line && !isspace(*line) && *line != ',') {
        *word++ = *line++;
    }
    *word = '\0';
    return line;
}

/* find command in the lookup table */
Command *findCommand(char *name, Command *commands) {
    int i;
    for (i = 0; i <= NUM_OF_COMMANDS; i++) {
        if (strcmp(commands[i].name, name) == 0) {
            return &commands[i];
        }
    }
    return NULL;
}

int returnRegisterNumber(char *operand) {
    if (operand[0] == 'r' && isdigit(operand[1]) &&  LOWEST_REG_NUM <= operand[1] && operand[1]  <= MAX_REG_NUM) {
        return atoi(&operand[1]);
    }
    else {
        return atoi(&operand[2]);
    }
}

/* function to check operand types */
int checkOperandTypes(char *operand) {
    if (operand[0] == 'r' && isdigit(operand[1]) &&  LOWEST_REG_NUM <= operand[1] && operand[1]  <= MAX_REG_NUM) {
        return IS_REGISTER; /* Register type */
    }
    if (operand[0] == '*' && operand[1] == 'r' && isdigit(operand[2]) && LOWEST_REG_NUM <= operand[2] && operand[2]  <= MAX_REG_NUM) {
        return IS_P_REGISTER; /* Pointer to register type */
    }
    if (isalpha(operand[0])) {
        return IS_LABEL; /* Label type or other memory address */
    }
    if (operand[0] == '#' && (isdigit(operand[1]) || (operand[1] == '-' && isdigit(operand[2])))) {
        return IS_IMMEDIATE; /* Constant type */
    }
    return 0; /* Unknown type */
}


/* encode operand type into bit fields */
int encodeAddressingMode(int operandType) {
    switch (operandType) {
        case IS_REGISTER: return REGISTER_ADDRESSING; /* Register type */
        case IS_P_REGISTER: return REGSTER_P_ADDRESSING; /* Pointer to register type */
        case IS_LABEL: return LABEL_ADDRESSING; /* label type  */
        case IS_IMMEDIATE: return IMMEDIATE_ADDRESSING; /* immedaite type */
        default: return 0;     /* Unknown type */
    }
}

/* process instruction */
int processInstruction(char *line, int *IC, virtualMem **tableIC, int *sizeIC) {
    int instruction; /* encoded instruction */
    char *operands[MAX_OPERANDS]; /* holder for operands */
    int operandCount = 0;
    Command *cmd;
    char *tmpLine;
    char cmdName[MAX_CMD_NAME];
    char *comma;
    int i;
    int operandTypes[2]; /* holder for operand types */

    Command commands[] = {
            {"mov", 0, 2},
            {"cmp", 1, 2},
            {"add", 2, 2},
            {"sub", 3, 2},
            {"lea", 4, 2},
            {"clr", 5, 1},
            {"not", 6, 1},
            {"inc", 7, 1},
            {"dec", 8, 1},
            {"jmp", 9, 1},
            {"bne", 10, 1},
            {"red", 11, 1},
            {"prn", 12, 1},
            {"jsr", 13, 1},
            {"rts", 14, 0},
            {"stop", 15, 0},
    };


    removeLabel(line);
    tmpLine = getCmdName(line, cmdName);
    cmd = findCommand(cmdName, commands);

    if (!cmd) {
        printf("Error: unknown command: %s\n", cmdName);
        return 0; /* err return */
    }

    while (*tmpLine) {
        while (isspace(*tmpLine)) tmpLine++;
        if (*tmpLine == '\0') break;

        comma = strchr(tmpLine, ',');
        if (comma) {
            *comma = '\0';
        }
        if (operandCount < cmd->num_operands) { /* check if command can support another operand */
            operands[operandCount++] = tmpLine;
        } else {
            printf("Error: invalid number of operands for command: %s\n", cmd->name);
            return 0; /* err return */
        }

        if (comma) {
            tmpLine = comma + 1;
        } else {
            break;
        }
    }

    if (operandCount < cmd->num_operands) {
        printf("Error: insufficient amount of operands, (expected: %d found: %d)\n", cmd->num_operands, operandCount);
        return 0;
    }

    for (i = 0; i < operandCount; i++) { /* assign operand types */
        operandTypes[i] = checkOperandTypes(operands[i]);
        if (operandTypes[i] == 0) {
            printf( "Error: invalid operand type: %s\n", operands[i]);
            return 0; /* err return  */
        }
    }

    /* encode instruction */
    instruction = (cmd->opcode << SHIFT_TO_OPCODE); /* opcode in bits 14-11 */
    if (operandCount  == 2) { /*This case will be for when the first operand is the sopurce and the second is the target*/
        instruction |= (encodeAddressingMode(operandTypes[0]) << SHIFT_TO_SOURCE); /* source operand in bits 7-10 */
        instruction |= (encodeAddressingMode(operandTypes[1]) << SHIFT_TO_TARGET); /* destination operand method in bits 3-6 */
    }
    else if (operandCount  == 1) {
        instruction |= (encodeAddressingMode(operandTypes[0]) << SHIFT_TO_TARGET); /* destination operand method in bits 3-6 */
    }
    instruction |= A_BIT; /* set 'A' bit to on */

    /* save instruction and curr IC here */
    if (resizeMemoryTable(tableIC, ++(*sizeIC))) {
        return 0; /* mem alloc err */
    }
    addEntry(*tableIC, (*sizeIC), *IC, instruction);

    /* Increment instruction counter based on operand types */
    if (operandCount == 2) {
        if ((operandTypes[0] == 1 || operandTypes[0] == 2) && (operandTypes[1] == 1 || operandTypes[1] == 2)) { /* if it's two operands of type register / reg pointer */
            instruction = 0;
            instruction |= returnRegisterNumber(operands[0]) << (SHIFT_TO_SOURCE - 1);
            instruction |= returnRegisterNumber(operands[1]) << SHIFT_TO_TARGET;
            instruction |= A_BIT;
            addNextWord(*tableIC, (*sizeIC) - 1, (*IC)+1, instruction);
            *IC += 2;
        }
        else { /* relevant when there is a constant or label */
            for (i = 0; i < operandCount; i++) {
                switch(operandTypes[i]) {
                    case 1: case 2: /* register or pointer to register */
                        instruction = 0;
                        if (i == 0) {
                            instruction |= returnRegisterNumber(operands[i]) << (SHIFT_TO_SOURCE - 1);
                        } else {
                            instruction |= returnRegisterNumber(operands[i]) << SHIFT_TO_TARGET;
                        }
                        instruction |= A_BIT;
                        addNextWord(*tableIC, (*sizeIC) - 1, (*IC) + i + 1, instruction);
                        break;
                    case 3: /* adresss */
                        addNextWord(*tableIC, (*sizeIC) - 1, (*IC) + i + 1, -1);
                        break;
                    case 4: /* immediate */
                        instruction = ((atoi(operands[i] + 1) << (SHIFT_TO_TARGET)) | A_BIT) & ZERO_UPPER_BITS;
                        addNextWord(*tableIC, (*sizeIC) - 1, (*IC) + i + 1, instruction); /* +1 for the address to skip the "#" */
                        break;
                }
            }
            *IC += 3;
        }
    }
    else if(operandCount == 1) {
        switch(operandTypes[0]) {
            case IS_REGISTER: case IS_P_REGISTER: /* register or pointer to register */
                instruction = 0;
                instruction |= returnRegisterNumber(operands[0]) << SHIFT_TO_TARGET;
                instruction |= A_BIT;
                addNextWord(*tableIC, (*sizeIC) - 1, (*IC) + 1, instruction);
                break;
            case IS_LABEL: /* adresss */
                addNextWord(*tableIC, (*sizeIC) - 1, (*IC) + 1, -1);
                break;
            case IS_IMMEDIATE: /* immediate */
                instruction = ((atoi(operands[0] + 1) << (SHIFT_TO_TARGET)) | A_BIT) & ZERO_UPPER_BITS;
                addNextWord(*tableIC, (*sizeIC) - 1, (*IC) + 1, instruction); /* +1 for the address to skip the "#" */
                break;
        }
        *IC += 2;
    }
    else {
        *IC +=1;
    }

    return 1; /* great success */
}