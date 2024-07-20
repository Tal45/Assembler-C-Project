
#include "../headers/filehandler.h"


int createDirectory(char *path) {
    char command[256];
    snprintf(command, sizeof(command), "mkdir -p %s", path);
    return system(command);
}

char *createFile(char *name, char *format) {
    char *tmp = NULL;
    char *fullpath = NULL;
    unsigned int name_size = 0;

    name_size += strlen(name) + strlen(format) + strlen(OUTPUT_DIR);

    fullpath = concatenateDir(OUTPUT_DIR,name);
    if (fullpath == NULL) {
        printf("Error: unable to allocate memory for string concatenation %s\n", OUTPUT_DIR);
        return NULL;
    }
    strcat(fullpath,"/"); /*creates the path ' OUTPUT_DIR/filename/ ' */

    tmp = (char *) malloc((name_size + 1) * sizeof(char));
    if (tmp == NULL) {
        printf("malloc: Memory allocation failed!\n");
        printf("Error: unable to create file: %s\n", strcat(name, format));
        free(fullpath);
        return NULL;
    }

    if (createDirectory(fullpath) != 0) {
        printf("Error: unable to create output directory %s\n", OUTPUT_DIR);
        free(tmp);
        free(fullpath);
        return NULL;
    }

    strcpy(tmp, fullpath); /* add output directory */
    strcat(tmp, name); /* add file name */
    strcat(tmp, format); /* add format name */
    free(fullpath);
    /*return full path to file 'OUTPUT_DIR/filename/filename.format' */
    return tmp;
}

int dumpEntriesToFile(const char *filename, Label labelTable[], int labelCount) {
    FILE *file;
    int i;

    file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error: unable to create file: %s\n", filename);
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
        printf("Error: unable to create file: %s\n", filename);
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
        printf("Error: unable to create file: %s\n", filename);
        return 1;
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

void removeFiles(char *filename) {
    char path[MAX_BUFFER];
    int size;
    const char *formats[] = {
            ".am",
            ".ext",
            ".ent",
            ".ob"
    };
    size = sizeof(formats) / sizeof(formats[0]);
    strcpy(path, filename);

    while (--size >= 0) {
        remove(strcat(path, formats[size]));
        strcpy(path, filename);
    }

}

char* concatenateDir(char* str1, char* str2) {
    /* Calculate the total length of the concatenated string */
    size_t length1 = strlen(str1);
    size_t length2 = strlen(str2);
    size_t totalLength = length1 + length2 + 1; /* +1 for the null terminator */

    /* Allocate memory for the concatenated string */
    char* result = (char*)malloc(totalLength * sizeof(char));
    if (result == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    /* Copy the first string into the result */
    strcpy(result, str1);

    /* Concatenate the second string to the result */
    strcat(result, str2);

    return result;
}