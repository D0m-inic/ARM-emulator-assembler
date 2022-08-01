#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "fileIO.h"
#include "assemble_constants.h"

fileLines_t *initialisefileLines() {
    fileLines_t *newfileLines = malloc(sizeof(fileLines_t));
    newfileLines->maxLength = MAX_FILE_LINE_LENGTH;
    newfileLines->lines = (char **) malloc(sizeof(*newfileLines->lines) * newfileLines->maxLength);
    newfileLines->currentLine = 0;
    return newfileLines;
}

void convertFile(FILE *inputFile, fileLines_t *fileLines) {
    char line[MAX_LINE_LENGTH];
    memset(line, '\0', sizeof(line));
    while (fgets(line, sizeof(line), inputFile) != NULL && (fileLines->currentLine < fileLines->maxLength)) {
        for (int i = 0; line[i]; i++) {
            if (line[i] == '\n') {
                line[i] = '\0';
            }
        }
        if (strcmp(line, "") != 0) {
            fileLines->lines[fileLines->currentLine] = malloc(sizeof(char) * sizeof(line));
            strcpy(fileLines->lines[fileLines->currentLine], line);
            fileLines->currentLine += 1;
        }
        memset(line, '\0', sizeof(line));
    }
}

// fp will be the second command line argument after using fopen
void fileWriter(FILE *fp, uint32_t *data) {
    fwrite(data, sizeof(uint32_t), 1, fp);
}

void freeFileLines(fileLines_t *fileLines) {
    for (int i = 0; fileLines->lines[i]; i++) {
        free(fileLines->lines[i]);
    }
    free(fileLines->lines);
    free(fileLines);
}
