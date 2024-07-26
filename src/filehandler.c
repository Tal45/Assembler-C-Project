#include "../headers/filehandler.h"

char *createFile(char *name, char *format, int isOutput) {
    char *tmp;
    char *filepath = NULL;
    unsigned int name_size = 0;

    filepath = strrchr(name, '/'); /* find if directory included in filename */
    if (filepath && isOutput) { /* if output directory, cut the input directory */
        filepath++;
    } else {
        filepath = name; /* otherwise take the filename and directory */
    }
    name_size += strlen(filepath) + strlen(format);
    if (isOutput) {
        name_size += strlen(OUTPUT_DIR);
    }

    tmp = (char *) malloc((name_size + 1) * CHAR_SIZE);
    if (tmp == NULL) {
        printf("malloc: Memory allocation failed!\n");
        printf("Error: unable to create file: %s\n", strcat(filepath, format));
        return NULL;
    }
    *tmp = '\0';
    if (isOutput) {
        strcpy(tmp, OUTPUT_DIR); /* add output directory */
    }
    strcat(tmp, filepath);
    strcat(tmp, format);

    return tmp;
}

int dumpEntriesToFile(const char *filename, Label **labelTable, int labelCount) {
    FILE *file;
    int i;

    file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: unable to create file: %s\n", filename);
        return ERROR;
    }

    for (i = 0; i < labelCount; i++) {
        if ((*labelTable)[i].isEntry == 1) {
            fprintf(file, "%s %04d\n", (*labelTable)[i].name, (*labelTable)[i].address);
        }
    }

    fclose(file);
    return SUCCESS;
}

int dumpExternsToFile(char *filename, extList *head) {
    FILE *file;
    extList *current;

    file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: unable to create file: %s\n", filename);
        return ERROR;
    }

    current = head;
    while (current != NULL) {
        fprintf(file, "%s %04d\n", current->label, current->add);
        current = current->next;
    }

    fclose(file);
    return SUCCESS;
}


int dumpMemoryToFile(char *filename, virtualMem *array, int sizeIC, int IC, int sizeDC) {
    int i;
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error: unable to create file: %s\n", filename);
        return ERROR;
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
    return SUCCESS; /* success */
}

void removeFiles(char *filename) {
    int size;
    char *pt;
    char path[MAX_BUFFER] = OUTPUT_DIR;
    const char *formats[] = {
            ".am",
            ".ext",
            ".ent",
            ".ob"
    };
    pt = strchr(path, '/'); /* pointer to end of directory name */
    pt++;
    size = sizeof(formats) / sizeof(formats[0]);
    strcat(path, filename); /* copy file name */

    while (--size >= 0) {
        remove(strcat(path, formats[size])); /* concat format and remove file */
        strcpy(pt, filename); /* reset file name to remove formatting */
    }

}
