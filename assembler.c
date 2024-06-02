#include <stdio.h>
#include <stdlib.h>

#include "assembler.h"

int main (int argc, char *argv[]) {
    char *amfile, *asfile;
    int errCode = -1;

    while (--argc > 0) { /* iterate while there are more files to process */
        asfile = createNewFile(argv[argc], ".as");

        errCode = preProcess(asfile);
        if (errCode < 0) {/*handle error here */}

        amfile = createNewFile(argv[argc], ".am"); /* pre-process was successful, create am file */
        errCode = firstPass(amfile);
        if (errCode < 0) {/*handle error here */}

        errCode = secondPass(amfile);
        if (errCode < 0) {/*handle error here */}

        free(amfile);
        free(asfile);
    }

    return 0; /* main executed successfully */
} /* end of main */