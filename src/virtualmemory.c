#include "../headers/virtualmemory.h"

/* add a node to the linked list - pass head node as */
int addNextWord(virtualMem *head, int index, int address, int binaryCode) {
    virtualMem* newNode;
    virtualMem *pt = &head[index];

    while (pt->nextWord != NULL) { /* find last node in list */
        pt = pt->nextWord;
    }

    newNode = (virtualMem*) malloc(sizeof(virtualMem));
    if (newNode == NULL) {
        printf("malloc: Memory allocation failed!\n");
        return ERROR;
    }
    newNode->address = address;
    newNode->binaryCode = binaryCode;
    newNode->nextWord = NULL;
    pt->nextWord = newNode;

    return SUCCESS;
}

/* resize memory table to newSize / initate table if newSize == 1 */
int resizeMemoryTable(virtualMem **table, int newSize) {
    virtualMem *newTable;

    if (newSize == 1) {
        newTable = (virtualMem *)malloc(newSize * sizeof(virtualMem));
        if (newTable == NULL) {
            printf("malloc: Memory allocation failed!\n");
            return ERROR;
        }
    } else {
        newTable = (virtualMem*) realloc(*table, newSize * sizeof(virtualMem));
        if (newTable == NULL) {
            printf("realloc: Memory allocation failed!\n");
            return ERROR;
        }
    }
    *table = newTable;
    /* Initialize the newly allocated element */
    (*table)[newSize - 1].binaryCode = 0;
    (*table)[newSize - 1].address = 0;
    (*table)[newSize - 1].nextWord = NULL;

    return SUCCESS; /* great success */
}

/* Function to add entry to memory table */
void addEntry(virtualMem* table, int index, int address, int binaryCode) {
    table[index - 1].address = address;
    table[index - 1].binaryCode = binaryCode;
    table[index - 1].nextWord = NULL;
}

/* function to get two virtual memory (IC and DC) and merges them into the IC memory table, also updates IC table size */
int mergeMemoryTables(virtualMem **tableIC, virtualMem *tableDC, int *size_tableIC, int size_tableDC, int IC) {
    int i, j;

    /* allocate enough memory in IC table */
    if (resizeMemoryTable(tableIC, (*size_tableIC) + size_tableDC)) {
        return ERROR; /* re-alloc failure */
    }
    else { /* memory allocation successful copy tableDC to tableIC */
        for (i = *size_tableIC + 1, j = 0; i <= (*size_tableIC) + size_tableDC; j++, i++) {
            addEntry(*tableIC, i, tableDC[j].address + IC, tableDC[j].binaryCode);
        }
    }
    *size_tableIC += size_tableDC; /* update size of tableIC */

    return SUCCESS;
}

/* free memory table and it's linked nodes if any present */
void freeMemoryTable(virtualMem **table, int size) {
    int i;
    virtualMem* current;
    virtualMem* tmp;

    for (i = 0; i < size; i++) {
        current = (*table)[i].nextWord;
        while (current != NULL) {
            tmp = current;
            current = current->nextWord;
            free(tmp);
        }
    }
    free(*table);
}

/* function to insert in virutal memory the address of the label used, it returns the line's address in virtual memory */
int encodeAddr(virtualMem *TableIC, int index, int addr, int isExt){
    virtualMem *pt = &TableIC[index];
    while (pt->binaryCode != -1) {
        pt = pt->nextWord;
    }
    if (isExt) { /* external label - only E bit is on */
        pt->binaryCode = E_BIT;
    } else { /* internal label */
        pt->binaryCode = (addr << SHIFT_TO_TARGET) | R_BIT;
    }

    return pt->address; /* return virtual addr */
}
