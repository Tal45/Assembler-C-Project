#include "../headers/filehandler.h"

char *createFile(char *name, char *format) {
    char *tmp;
    unsigned int name_size = strlen(name) + strlen(format);

    tmp = (char *) malloc((name_size + 1) * sizeof(char));
    if (tmp == NULL) {
        printf("malloc: Memory allocation failed!\n");
        printf("Error: unable to create file %s\n", strcat(name, format));
        return NULL;
    }
    strcpy(tmp, name);
    strcat(tmp, format);

    return tmp;
}

int dumpEntriesToFile(const char *filename, Label labelTable[], int labelCount) {
    FILE *file;
    int i;

    file = fopen(filename, "a");
    if (file == NULL) {
        return 1;
    }

    for (i = 0; i < labelCount; i++) {
        if (labelTable[i].isEntry == 1) {
            fprintf(file, "%s %04d\n", labelTable[i].name, labelTable[i].address);
        }
    }

    fclose(file);
    return 0;
}

int dumpExternsToFile(char *filename, extList *head) {
    FILE *file;
    extList *current;

    file = fopen(filename, "a");
    if (file == NULL) {
        return 1;
    }

    current = head;
    while (current != NULL) {
        fprintf(file, "%s %04d\n", current->label, current->add);
        current = current->next;
    }

    fclose(file);
    return 0;
}


int dumpMemoryToFile(char *filename, virtualMem *array, int sizeIC, int IC, int sizeDC) {
    int i;
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        /* file open error */
    }
    fprintf(fp,"  %d %d\n", IC-IC_START_ADDRESS, sizeDC);

    for (i = 0; i < sizeIC; i++) {
        virtualMem *current = &array[i];
        fprintf(fp,"%04d %05o\n", current->address, current->binaryCode);

        /* traverse and print the linked nodes */
        current = current->nextWord;
        while (current != NULL) {
            fprintf(fp,"%04d %05o\n", current->address, current->binaryCode);
            current = current->nextWord;
        }

    }
    fclose(fp);
    return 0; /* success */
}