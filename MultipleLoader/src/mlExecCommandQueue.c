#include "ClWrapper.h"
#include "SizeSpec.h"
#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <CL/cl_ext.h>

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
        printf("\nError while applying Kernel on range (mlExecCommandQueue : clEnqueueNDRangeKernel): %s\n", mlErrorHandler(error));

    error = clFinish(command_queue);
    if (error != CL_SUCCESS)
        printf("\nError while awaiting command queue (mlExecCommandQueue : clFinish): %s\n", mlErrorHandler(error));
}