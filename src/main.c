#include "../headers/main.h"

int main (int argc, char *argv[]) {
    char *amfile = NULL, *asfile = NULL, *obfile = NULL, *extfile = NULL, *entfile = NULL;
    int errCode = 0;

    Label labelTable[MAX_LABELS];
    int labelCount = 0;

    int IC = IC_START_ADDRESS;
    int entFlag = 0;

    virtualMem *TableIC = NULL;
    int sizeIC = 0;
    virtualMem *TableDC = NULL;
    int sizeDC = 0;
    extList *extApperance = NULL;

    while (--argc > 0) { /* iterate while there are more files to process */
        asfile = createFile(argv[argc], ".as", 0);
        if (asfile == NULL) {
            break;
        }
        amfile = createFile(argv[argc], ".am",1);
        if (amfile == NULL) {
            break;
        }

        errCode = preProcess(asfile, amfile);
        if (errCode) {
            remove(amfile);
            printf("Error: pre-assembly failed for file: %s\n", asfile);
        }
        free(asfile);

        if (!errCode) {
            errCode = firstPass(amfile, labelTable, &labelCount, &TableIC, &sizeIC, &TableDC, &sizeDC, &IC, &entFlag);
            if (errCode) {
                printf("Error: first pass failed for file: %s\n", amfile);
                remove(amfile);
            }
        }


        if (!errCode) {
            errCode = secondPass(amfile, labelTable, &labelCount, &TableIC, &extApperance);
            if (errCode) {
                printf("Error: second pass failed for file: %s\n", amfile);
                remove(amfile);
            }
        }
        free(amfile); /* am file no longer needed, deallocate memory */

        if (!errCode) { /* dump data into files */
            obfile = createFile(argv[argc], ".ob", 1);
            if (obfile == NULL) {
                break;
            }
            remove(obfile); /* if previous file exists remove it before creating new */
            errCode |= dumpMemoryToFile(obfile, TableIC, sizeIC, IC, sizeDC);
            free(obfile); /* de-allocate memory */

            if (extApperance) {
                extfile = createFile(argv[argc], ".ext", 1);
                if (extfile == NULL) {
                    errCode = 1;
                    break;
                }
                remove(extfile);
                errCode |= dumpExternsToFile(extfile, extApperance);
                free(extfile); /* de-allocate memory */
            }

            if (entFlag) {
                entfile = createFile(argv[argc], ".ent", 1);
                if (entfile == NULL) {
                    errCode = 1;
                    break;
                }
                remove(entfile);
                errCode |= dumpEntriesToFile(entfile, labelTable, labelCount);
                free(entfile); /* de-allocate memory */
            }
        }

        /* if error occured somewhere along the way, remove all files associated if any were created */
        if (errCode) {
            removeFiles(argv[argc]);
        }


        /* free up memory and zero flags for next iteration */

        if (sizeIC) { /* free virtual memory tables */
            freeMemoryTable(&TableIC, sizeIC);
        }
        if (sizeDC && !errCode) { /* if first pass failed then tableDC wasn't merged into IC, free separately */
            freeMemoryTable(&TableDC, sizeDC);
        }
        resetLabelTable(labelTable, labelCount); /* zero out values of label table */
        sizeIC = sizeDC = labelCount = entFlag = 0; /* zero out flags and trackers */
        IC = IC_START_ADDRESS;

        freeList(extApperance); /* free single list of extern appearances */

        /* set dynamic structs to NULL */
        TableIC = TableDC = NULL;
        extApperance = NULL;

    } /* end of while loop */

    return 0; /* main executed successfully */
} /* end of main */