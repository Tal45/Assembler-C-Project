/*
 * Description:
 *      Two-Pass assembler project that can get multiple .as files from user inserted by command line
 *      the program performs pre-process, first pass and second pass accordingly and outputs .ob file
 *      to view the construction of the virutal memory, .am file for the program code after macro extension
 *      .ent file if any entries declared in program and .ext file if any extern addresses used in program
 *
 * Authors:
 *      Tal Levi and Vadim Volkov
 *
 * Version:
 *      05/08/2024
 */
#include "../headers/main.h"

int main (int argc, char *argv[]) {
    char *amfile = NULL, *asfile = NULL, *obfile = NULL, *extfile = NULL, *entfile = NULL;
    int errCode = 0;
    char *filename = NULL;

    int IC = IC_START_ADDRESS;
    int entFlag = 0;

    Label *labelTable = NULL;
    int labelCount = 0;
    virtualMem *TableIC = NULL;
    int sizeIC = 0;
    virtualMem *TableDC = NULL;
    int sizeDC = 0;
    extList *extApperance = NULL;

    if (argc == 1) { /* no arguments */
        printf("Error: no file names passed as arguments!\nResolve by passing arguments in command line (i.e ./assembler filename1 filename2 ...)\n");
    }

    while (--argc > 0) { /* iterate while there are more files to process */

	    if (strrchr(argv[argc], '/') == NULL) { /* find if file is in different dir */
            filename = argv[argc];
        } else {
            filename = strrchr(argv[argc],'/');
            filename++;  /* skip dir to hold only filename */
        }

        printf("\n------------------ Assembling %s file... ------------------\n", filename);

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
            printf("Error: pre-assembly failed for file: %s\n", asfile);
        }
        free(asfile);

        if (!errCode) {
            errCode = firstPass(amfile, &labelTable, &labelCount, &TableIC, &sizeIC, &TableDC, &sizeDC, &IC, &entFlag);
            if (errCode) {
                printf("Error: first pass failed for file: %s\n", amfile);
            }
        }

        if (!errCode) {
            errCode = secondPass(amfile, &labelTable, &labelCount, &TableIC, &extApperance);
            if (errCode) {
                printf("Error: second pass failed for file: %s\n", amfile);
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
                errCode |= dumpEntriesToFile(entfile, &labelTable, labelCount);
                free(entfile); /* de-allocate memory */
            }
        }

        /* if error occured somewhere along the way, remove all files associated if any were created */
        if (errCode) {
            removeFiles(filename);
        }

        /* free up memory and zero flags for next iteration */
        if (sizeIC) { /* free virtual memory tables */
            freeMemoryTable(&TableIC, sizeIC);
        }
        if (sizeDC && !errCode) { /* if first pass failed then tableDC wasn't merged into IC, free separately */
            freeMemoryTable(&TableDC, sizeDC);
        }
        if (labelTable) { /* free allocated memory for labelTable */
            free(labelTable);
        }
        sizeIC = sizeDC = labelCount = entFlag = 0; /* zero out flags and trackers */
        IC = IC_START_ADDRESS;

        freeList(extApperance); /* free single list of extern appearances */

        /* set dynamic structs to NULL */
        TableIC = TableDC = NULL;
        extApperance = NULL;
        labelTable = NULL;
	    if(!errCode) {
            printf("\n- Files assembled successfully!\n");
        } else {
            printf("\n- Error occurred - no files assembled!\n");
        };
        printf("\n----------------- End of assembly (%s.as) -----------------\n", filename);

    } /* end of while loop */

    return SUCCESS; /* main executed successfully */
} /* end of main */
