#ifndef ARM11_23_FILEIO_H
#define ARM11_23_FILEIO_H

#include <stdio.h>

typedef struct {
    char **lines; //array of lines (strings -> char pointers)
    int maxLength, currentLine;
} fileLines_t;

fileLines_t *initialisefileLines();
void convertFile(FILE *inputFile, fileLines_t *fileLines);
void fileWriter(FILE *fp, uint32_t *data);
void freeFileLines(fileLines_t *fileLines);

#endif //ARM11_23_FILEIO_H
