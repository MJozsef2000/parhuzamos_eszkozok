#include "utils.h"

#define CL_TARGET_OPENCL_VERSION 220

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("Reverse an array:\n");
    reverse();
    printf("\nSwitch odd and even indexed elements:\n");
    swap();
}