#ifndef ARM11_23_FETCH_H
#define ARM11_23_FETCH_H

#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>

void initialiseArm(ARM *arm, char *fileName);
void fetch(ARM* arm);
long fileSize(const char* filename);
#endif //ARM11_23_FETCH_H
