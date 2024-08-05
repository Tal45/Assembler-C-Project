#include "../headers/labelhandler.h"

int resizeLabelTable(Label **labelTable, int labelCounter) {
    Label *tmp;

    if (labelCounter == 1) {
        /* allocate memory for one Label struct */
        tmp = (Label *)malloc(sizeof(Label));
        if (tmp == NULL) {
            printf("Malloc: memory allocation failed\n");
            return ERROR;
        }
    } else {
        /* realloc memory to hold labelCounter + 1 Label structs in labelTable */
        tmp = (Label *)realloc(*labelTable, (labelCounter + 1) * sizeof(Label));
        if (tmp == NULL) {
            printf("Realloc: memory allocation failed\n");
            return ERROR;
        }

    }
    *labelTable = tmp;
    memset((*labelTable)[labelCounter-1].name, 0, MAX_LABEL_LENGTH);
    (*labelTable)[labelCounter-1].address = 0;
    (*labelTable)[labelCounter-1].isData = 0;
    (*labelTable)[labelCounter-1].isExtern = 0;
    (*labelTable)[labelCounter-1].isEntry = 0;

    return SUCCESS;
}

void addLabel(Label **labelTable, int labelCount, char *name, int address, int isData, int isExt, int isEnt) {
    strcpy((*labelTable)[labelCount - 1].name, name);
    (*labelTable)[labelCount - 1].address = address;
    (*labelTable)[labelCount - 1].isData = isData;
    (*labelTable)[labelCount - 1].isExtern = isExt;
    (*labelTable)[labelCount - 1].isEntry = isEnt;
}

int isLabel(char *token) {
    return token[strlen(token) - 1] == ':';
}

int isLabelDefined(char *token, Label **labelTable, int labelCount) {
    int i;
    for (i = 0; i < labelCount; i++) {
        if (strcmp((*labelTable)[i].name, token) == 0)
            return ERROR;
    }
    return SUCCESS;
}

/* find the index of a label by name in the label table */
int findLabelIndex(Label **labelTable, int tableSize, char *labelName) {
    int i;

    for (i = 0; i < tableSize; i++) {
        if (strcmp((*labelTable)[i].name, labelName) == 0) {
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

void updateDCOffset(Label **table, int labelCount, int IC) {
    int i;

    for (i = 0; i < labelCount; i++) {
        if ((*table)[i].isData) {
            (*table)[i].address += IC;
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
