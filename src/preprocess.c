#include "../headers/constants.h"
#include "../headers/structs.h"
#include "../headers/macrohandler.h"

int preProcess(char *file, char *destfile) {
    int i;
    char line[MAX_INPUT_LINE];
    int lineCount = 0;
    char mcroname[MAX_MACRO_NAME];
    char checkEndOfLine[EOL_BUFFER];

    FILE *src, *tmp;
    Macro *macrotable = NULL;

    unsigned int macrotable_size = 0;
    int endCode = 0;

    int mcrindex;

    src = fopen(file, "r");
    if (src == NULL) {
        printf("Error: unable to open file! file name: %s\n", file);
        return -1;
    }
    tmp = fopen(destfile, "w");
    if (tmp == NULL) {
        printf("Error: unable to open file! file name: %s\n", destfile);
        fclose(src);
        return -1;
    }

    while ((fgets(line, MAX_INPUT_LINE, src)) != NULL && !endCode) {
        lineCount++;
        if (isEmptyOrCommentLine(line)) {
            /* if there is a blank line or comment, continue to next line in file without copying it */
            continue;
        }
        else {
            if (strstr(line, "macr") != NULL && !isSingleWordLine(line)) { /* check if macr substring is present in current line, if so start register new macro */
                if (sscanf(line, "%s %s %s", mcroname, mcroname, checkEndOfLine) == 3) {
                    printf("Error: extraneous text after macro definition! in line %d: %s\n", lineCount, line);
                    endCode = -1;
                    continue;
                }
                if ((endCode = validateMacroName(mcroname))) {
                    /* invalid macro name */
                    printf("Error: macro name can not be command/instruction name! in line %d: %s\n", lineCount, line);
                    continue;
                }
                endCode = resizeMacroTable(&macrotable, ++macrotable_size);
                if (endCode < 0) {
                    /* if re-alloc failed macrotable still holds the original pointer so it can be freed */
                    continue;
                }
                /* function gets macro name, src file and macrotable and initializes the macro actions into struct */
                src = registerMacro(mcroname, src, &macrotable[macrotable_size - 1], &endCode, &lineCount);
                if (endCode < 0) { /* error occured in mem allocation or endmacr line */
                    continue;
                }
            }
            else if (isSingleWordLine(line) && isMacroName(line, macrotable, macrotable_size)) {
                sscanf(line, "%s", line);
                mcrindex = findMacroIndex(line, macrotable, macrotable_size);
                tmp = extendMacro(tmp, &macrotable[mcrindex]);
            }
            else { /* copy line to .am file as it is */
                fputs(line, tmp);
            }
        }
    } /* end of while */

    /* freeing up memory used by macrotable and closing files */
    if (macrotable != NULL) {
        for (i = 0; i < macrotable_size; i++) {
            if (macrotable[i].lines != NULL) {
                free(macrotable[i].lines);
            }
        }
        free(macrotable);
    }
    fclose(src);
    fclose(tmp);

    return endCode;
}



