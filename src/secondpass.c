#include "../headers/validation.h"


int secondPass(char *amfile, Label **labelTable, int *labelCount, virtualMem **TableIC, extList **extApp) {

    char nextLine[MAX_INPUT_LINE];
    char copyLine[MAX_INPUT_LINE];
    char *token;
    int labelIndex;
    int addr;
    int extUseFlag = 0;
    int tmpIC;
    int err = 0, endCode = 0;
    int lineNum = 0;
    int IC = 0;

    FILE *source = fopen(amfile, "r");
    if (!source) {
        printf("Error: unable to open file %s\n", amfile);
        return ERROR;
    }

    while (fgets(nextLine, MAX_INPUT_LINE, source) != NULL) {
        trimCntrls(nextLine);
        lineNum++;
        strcpy(copyLine, nextLine);
        token = strtok(copyLine, " \t\r\n");
        if (isLabel(token)) { /* if label definition - skip it */
            token = strtok(NULL, " \t\r\n");
        }
        if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0 || strcmp(token, ".extern") == 0) {
            /* skip to next line in file */
            continue;
        }
        else if ((strcmp(token, ".entry") == 0)){
            token = strtok(NULL, " \t\r\n");
            if ((labelIndex = findLabelIndex(labelTable, *labelCount, token)) < 0) { /* label not defined in first pass */
                handleError(lineNum, nextLine, &endCode);
            } else {
                (*labelTable)[labelIndex].isEntry = 1;
                continue;
            }
        } else { /* handle command & operands */
            while ( (token = strtok(NULL, " ,\t\r\n")) != NULL) {
                if (checkOperandTypes(token) == IS_LABEL) { /* function to check if the current operand is a label use */
                    if ((labelIndex = findLabelIndex(labelTable, *labelCount,token)) < 0) {
                        handleError(lineNum, nextLine, &endCode);
                    } else { /* complete the address of the label in the virutal memory */
                        addr = (*labelTable)[labelIndex].address;
                        extUseFlag = (*labelTable)[labelIndex].isExtern;
                        tmpIC = encodeAddr(*TableIC, IC, addr, extUseFlag);
                        if (extUseFlag) { /* label is extern, enlist it in extern linked list */
                            err = enlistNode(extApp, token,tmpIC);
                            if (err) { /* malloc issue - break */
                                break;
                            }
                            extUseFlag = 0;
                        }
                    }
                }
            }
            IC++;
        }
    }
    fclose(source);

    return err || endCode; /* end */
}
