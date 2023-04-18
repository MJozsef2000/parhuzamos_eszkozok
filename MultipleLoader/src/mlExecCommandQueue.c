#include "ClWrapper.h"
#include "SizeSpec.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>


void mlExecComandQueue(cl_command_queue command_queue, ClWrapper_t cw, SizeSpec_t s){
    clEnqueueNDRangeKernel(
        command_queue,
        cw.kernel,
        1,
        NULL,
        &(s.global_work_size),
        &(s.local_work_size),
        0,
        NULL,
        NULL);
clFinish(command_queue);
}