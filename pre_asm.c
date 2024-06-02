#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    char **lines;
    int linecount;
} Macro;
/*
 * Agenda: follow pre-assembly algorithm (page 31) to create am file (marco extended file)
 * from sources file
 */

/*
 * preProcess function explanation here
 */
int preProcess(char *file) {
    int i, j;
    int macroflag = 0, extendmacro = 0;
    char *line;
    char *field, *mcroname;
    FILE *src, *tmp;
    char *tmpfile = "tmp";
    Macro **macrotable;
    int macrocount = 0, mcrindex = -1;

    src = fopen(file, "r");
    if (src == NULL) {
        /* handle file open error here */
    }
    tmp = fopen(tmpfile, "w");
    if (tmp == NULL) {
        /* handle file open error here */
    }

    while ((line = getLine(src)) != NULL) {

        if (sscanf(line,"%s", field) == 0) continue;

        if (macroflag) {
            if (strcmp(field, "endmacr") == 0) {
                macroflag = 0;
                mcrindex = -1;
            }
            else {
                macrotable[macrocount]->lines[++mcrindex] = line;
                macrotable[macrocount]->linecount++;
            }
        }
        else {
            extendmacro = 0;
            for (i = 0; i < macrocount; i++) {
                if ((strcmp(field, macrotable[i]->name)) == 0) {
                    for (j = 0; j < macrotable[i]->linecount; j++) {
                        fputs(macrotable[i]->lines[j], tmp);
                    }
                }
                extendmacro = 1;
                break;
            }
        }
        if (!extendmacro) {
            if (strcmp(field, "macr") == 0) {
                macroflag = 1;
                macrocount++;
                sscanf(line, "%s", mcroname);
                init_macro(mcroname);
            }
        }
        else {
            fputs(line, tmp);
            free(line);
        }
    } /* end of while */

    for (i = 0; i < macrocount; ++i) {
        for (j = 0; j < macrotable[i]->linecount; ++j) {
            free(macrotable[i]->lines[j]);
        }
        free(macrotable[i]->lines);
    }

    free(macrotable);
    fclose(src);
    fclose(tmp);

}