#include "../headers/constants.h"
#include "../headers/structs.h"
#include "../headers/macrohandler.h"

int preProcess(char *file, char *destfile) {
    int i;
    char line[MAX_INPUT_LINE];
    unsigned int lineLength;
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
        return ERROR;
    }
    tmp = fopen(destfile, "w");
    if (tmp == NULL) {
        printf("Error: unable to open file! file name: %s\n", destfile);
        fclose(src);
        return ERROR;
    }

    while ((fgets(line, MAX_INPUT_LINE, src)) != NULL) {
        lineCount++;
        lineLength = strlen(line);
        if (isEmptyOrCommentLine(line)) {
            /* if there is a blank line or comment, continue to next line in file without copying it */
            continue;
        } else if (lineLength == MAX_INPUT_LINE -1 && line[lineLength-1] != '\n') {
          /* if input line exceeds buffer */
          printf("Error: input line exceeds maximum line length of 80, line number: %d\n", lineCount);
          endCode = ERROR;
          continue;
        } else {
            if (strstr(line, "macr") != NULL && !isSingleWordLine(line)) { /* check if macr substring is present in current line, if so start register new macro */
                if (sscanf(line, "%s %s %s", mcroname, mcroname, checkEndOfLine) == THREE_WORDS) {
                    printf("Error: extraneous text after macro definition! in line %d: %s\n", lineCount, line);
                    endCode = ERROR;
                    continue;
                }
                if ((endCode = validateMacroName(mcroname))) {
                    /* invalid macro name */
                    printf("Error: macro name can not be command/instruction name! in line %d: %s\n", lineCount, line);
                    continue;
                }
                endCode = resizeMacroTable(&macrotable, ++macrotable_size);
                if (endCode < 0) {
                    /* if re-alloc failed macrotable still holds the original pointer, so it can be freed */
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



