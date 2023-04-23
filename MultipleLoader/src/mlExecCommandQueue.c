#include "ClWrapper.h"
#include "SizeSpec.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>


void mlExecComandQueue(cl_command_queue command_queue, ClWrapper_t cw, SizeSpec_t s){
    cl_int error = clEnqueueNDRangeKernel(
        command_queue,
        cw.kernel,
        s.dim,
        NULL,
        s.global_size,
        s.local_size,
        0,
        NULL,
        NULL);
    if (error != CL_SUCCESS)
        printf("Error while applying Kernel on range: %d\n", error);

clFinish(command_queue);
}