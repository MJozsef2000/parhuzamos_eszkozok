#include "utils.h"

#define CL_TARGET_OPENCL_VERSION 220

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("\nCorrected missing numbers:\n\n");
    replace();
}