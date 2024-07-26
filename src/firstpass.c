#include "../headers/validation.h"

int firstPass(char *amfile, Label **labelTable, int *labelCount, virtualMem **TableIC, int *sizeIC, virtualMem **TableDC, int *sizeDC, int *IC, int *entFlag) {

    int onLabel = 0; /* flag for label def */
    char label[MAX_LABEL_LENGTH]; /* label name holder */

    int lineNum = 0;
    char line[MAX_INPUT_LINE];
    char copyLine[MAX_INPUT_LINE];
    char *token;
    int tmpNum;
    int endCode = 0;
    int err = 0;

    FILE *source = fopen(amfile, "r");
    if (!source) {
        printf("Error: unable to open file %s\n", amfile);
        return ERROR;
    }

    while (fgets(line, MAX_INPUT_LINE, source) != NULL) {
        trimCntrls(line);
        lineNum++;
        strcpy(copyLine, line);
        token = strtok(line, " \t\r\n");

        if ((onLabel = isLabel(token))) {
            token[strlen(token) - 1] = '\0'; /* removes : after label */

            strcpy(label, token);
            if (isLabelDefined(label, labelTable, *labelCount)) {
                endCode = ERROR;
            }
            token = strtok(NULL, " \t\r\n");
        }

        if (strcmp(token, ".data") == 0 || strcmp(token, ".string") == 0) {
            if (onLabel) {
                if (resizeLabelTable(labelTable, ++(*labelCount))) {
                    endCode = ERROR; /* memory failure */
                    break;
                }
                addLabel(labelTable, *labelCount, label, *sizeDC, 1, 0, 0);
            }
            if (strcmp(token, ".data") == 0) {
                if (validateData(copyLine)) { /* validates data after ins, returns true if valid */
                    while ((token = strtok(NULL, " ,"))) {
                        err = resizeMemoryTable(TableDC, ++(*sizeDC));
                        if (err) { /* memory allocation error */
                            break;
                        }
                        tmpNum = atoi(token);
                        addEntry(*TableDC, (*sizeDC), (*sizeDC) - 1, tmpNum & ZERO_UPPER_BITS);
                    }
                } else {
                    handleError(lineNum, copyLine, &endCode);
                }
            } else { /* data type is string */
                if (validateString(copyLine)) { /* returns 1 if valid, 0 if invalid */
                    token = strtok(NULL, " \""); /* "bacd" \0 */
                    while (*token) { /* store each character in word in mem */
                        err = resizeMemoryTable(TableDC, ++(*sizeDC));
                        if (err) { /* memory allocation error */
                            break;
                        }
                        addEntry(*TableDC, (*sizeDC), (*sizeDC) - 1, (*token) & ZERO_UPPER_BITS);
                        token++;
                    }
                    /* store \0 after end of string */
                    err = resizeMemoryTable(TableDC, ++(*sizeDC));
                    if (err) { /* memory allocation error */
                        break;
                    }
                    addEntry(*TableDC, (*sizeDC), (*sizeDC) - 1, 0);

                } else {
                    handleError(lineNum, copyLine, &endCode);
                }
            }
        } else if (strcmp(token, ".extern") == 0 || strcmp(token, ".entry") == 0) {
            if (onLabel) {
                printf("Warning: label definition before extern/entry declaration is meaningless!\n");
            }

            if (strcmp(token, ".extern") == 0) {
                if (validateSingleLabel(copyLine)) { /* check if only one label in declaration */
                    token = strtok(NULL, " \t\r\n");
                    if (resizeLabelTable(labelTable, ++(*labelCount))) {
                        endCode = ERROR; /* memory failure */
                        break;
                    }
                    addLabel(labelTable, *labelCount, token, 1, 0, 1, 0);
                } else {
                    handleError(lineNum, copyLine, &endCode);
                }
            } else { /* entry type label */
                *entFlag = 1;
                if (!validateSingleLabel(copyLine)) { /* check if only one label in declaration */
                    handleError(lineNum, copyLine, &endCode);
                }
            }
        } else if (isCommand(token)) {
            if (onLabel) {
                if (resizeLabelTable(labelTable, ++(*labelCount))) {
                    endCode = ERROR; /* memory failure */
                    break;
                }
                addLabel(labelTable, *labelCount, label, *IC, 0, 0 ,0);
            }
            if (processInstruction(copyLine, IC, TableIC, sizeIC)) { /* process instruction to allocate memory */
                handleError(lineNum, copyLine, &endCode);
            }

        } else { /* unknown syntax or undefined command / instruction */
            printf("Error: unknown syntax (line %d): %s\n", lineNum, copyLine);
            endCode = ERROR;
        }
    }

    fclose(source);
    if (endCode) { /* if error stop here */
        return ERROR;
    }

    /* update offset for data counter*/
    updateDCOffset(labelTable, *labelCount, *IC);
    if (*sizeDC) { /* if any data presents, merge it into one virtual memory table */
        mergeMemoryTables(TableIC, *TableDC, sizeIC, *sizeDC, *IC);
    }

    return SUCCESS;
}
