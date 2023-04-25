#include "utils.h"
#include "PlatformAndDevices.h"
#include "vectors.h"
#include "SizeSpec.h"
#include "ClWrapper.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <CL/cl_ext.h>

#include <stdio.h>
#include <stdlib.h>

const cl_int SAMPLE_SIZE = 5;

int main(void)
{
    ClWrapper_t cw = mlInit("kernels/sample.cl","vector_sum");
    // Initialize kernel arguments
    Vector vs;
    vs.n = SAMPLE_SIZE;
    for(int i=0; i<SAMPLE_SIZE; i++){
        vs.a[i] = i;
        vs.b[i] = i + 2;
        vs.c[i] = 0;
    }
    // Create the device buffer for input and output data
    const int buffer_count = 1;
    const int input_param_count = 1;
    const int output_param_count = 1;
    cl_mem buffers[buffer_count];
    buffers[0] = clCreateBuffer(cw.context, CL_MEM_READ_WRITE, sizeof(Vector), NULL, NULL);

    mlInitKernel(cw.kernel, cw.context, buffers, buffer_count);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(cw.context, cw.pad.device_id, NULL, NULL);

    // Host buffer -> Device buffer
    void * i_ptrs[input_param_count];
    i_ptrs[0] = &vs;

    // INPUT
    mlInputToDevice(command_queue, buffers, buffer_count, sizeof(Vector), i_ptrs);
    // Size specification (Number of threads, number of subtrheads)
    SizeSpec_t s = mlSizeSpecification1D(5,1);
    // Apply the kernel on the range
    mlExecComandQueue(command_queue, cw, s);
    // Host buffer <- Device buffer
    void * o_ptrs[output_param_count];
    o_ptrs[0] = &vs;
    // OUTPUT
    mlOutputFromDevice(command_queue, buffers, buffer_count,sizeof(Vector), o_ptrs);
    printf("For %d number of work:\n",vs.n);
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        printf("[%d]: %d,", i, vs.a[i]);
        printf("[%d]: %d,", i, vs.b[i]);
        printf("[%d]: %d,\n", i, vs.c[i]);
    }
    // Release the resources
    mlReleaseResources(cw);
}