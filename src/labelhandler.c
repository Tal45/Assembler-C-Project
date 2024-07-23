#include "../headers/labelhandler.h"

void addLabel(Label labelTable[], int *labelCount, char *name, int address, int isData, int isExt, int isEnt) {
    strcpy(labelTable[*labelCount].name, name);
    labelTable[*labelCount].address = address;
    labelTable[*labelCount].isData = isData;
    labelTable[*labelCount].isExtern = isExt;
    labelTable[*labelCount].isEntry = isEnt;
    (*labelCount)++;
}

int isLabel(char *token) {
    return token[strlen(token) - 1] == ':';
}

int isLabelDefined(char *token, Label *labelTable, int labelCount) {
    int i;
    for (i = 0; i < labelCount; i++) {
        if (strcmp(labelTable[i].name, token) == 0)
            return 1;
    }
    return 0;
}

/* reset all labels in label table */
void resetLabelTable(Label *labelTable, int size) {
    int i;

    for (i = 0; i < size; i++) {
        memset(labelTable[i].name, 0, MAX_LABEL_LENGTH);
        labelTable[i].address = 0;
        labelTable[i].isData = 0;
        labelTable[i].isExtern = 0;
        labelTable[i].isEntry = 0;
    }
}

/* find the index of a label by name in the label table */
int findLabelIndex(Label *labelTable, int tableSize, char *labelName) {
    int i;

    for (i = 0; i < tableSize; i++) {
        if (strcmp(labelTable[i].name, labelName) == 0) {
            return i;
        }
    }
    printf("Error: label %s not found in the label table!\n", labelName);
    return ERROR;
}

/* extern calls linked list */
int enlistNode(extList **extApp, char *label, int addr) {
    extList *newNode;
    extList *current;

    newNode = (extList *)malloc(sizeof(extList));
    if (newNode == NULL) {
        printf("malloc: Memory allocation failed!\n");
        return ERROR;
    }

    strcpy(newNode->label, label);
    newNode->add = addr;
    newNode->next = NULL;

    if (*extApp == NULL) {
        *extApp = newNode;
    } else {
        current = *extApp;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    return SUCCESS;
}

/* free the linked list of extern calls */
void freeList(extList *head) {
    extList *current;
    extList *nextNode;

    current = head;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
}
