#include "utils.h"
#include "PlatformAndDevices.h"
#include "vectors.h"
#include "SizeSpec.h"
#include "ClWrapper.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>

const cl_int SAMPLE_SIZE = 5;

int main(void)
{
    ClWrapper_t cw = mlInit("kernels/sample.cl","vector_sum");
    // Initialize kernel arguments
    Vector vs[SAMPLE_SIZE];
    for(int i=0; i<5; i++){
        vs[i].a = (cl_int)malloc(sizeof(cl_int));
        vs[i].b = (cl_int)malloc(sizeof(cl_int));
        vs[i].c = (cl_int)malloc(sizeof(cl_int));
        vs[i].a = i;
        vs[i].b = i + 2;
        vs[i].n = SAMPLE_SIZE;
    }

    Vector vs2[SAMPLE_SIZE];
    for(int i=0; i<5; i++){
        vs2[i].a = (cl_int)malloc(sizeof(cl_int));
        vs2[i].b = (cl_int)malloc(sizeof(cl_int));
        vs2[i].c = (cl_int)malloc(sizeof(cl_int));
        vs2[i].a = i + 1;
        vs2[i].b = i + 3;
        vs2[i].n = SAMPLE_SIZE;
    }
    // Create the device buffer for input and output data
    const int buffer_count = 2;
    cl_mem buffers[buffer_count];
    buffers[0] = clCreateBuffer(cw.context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(Vector), NULL, NULL);
    buffers[1] = clCreateBuffer(cw.context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(Vector), NULL, NULL);
    
    mlInitKernel(cw.kernel, cw.context, buffers, buffer_count);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(cw.context, cw.pad.device_id, NULL, NULL);

    // Host buffer -> Device buffer
    void * ptrs[2];
    ptrs[0] = vs;
    ptrs[1] = vs2;

    // INPUT
    mlInputToDevice(command_queue, buffers, buffer_count, SAMPLE_SIZE * sizeof(Vector), ptrs);
    // Size specification (int nr_of_computations)
    SizeSpec_t s = mlSizeSpecification(SAMPLE_SIZE);
    // Apply the kernel on the range
    mlExecComandQueue(command_queue, cw, s);
    // Host buffer <- Device buffer
    // OUTPUT
    mlOutputFromDevice(command_queue, buffers, buffer_count, SAMPLE_SIZE * sizeof(Vector), ptrs);
    
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        printf("[%d]: %d,", i, vs[i].a);
        printf("[%d]: %d,", i, vs[i].b);
        printf("[%d]: %d,\n", i, vs[i].c);
    }
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        printf("[%d]: %d,", i, vs2[i].a);
        printf("[%d]: %d,", i, vs2[i].b);
        printf("[%d]: %d,\n", i, vs2[i].c);
    }
    // Release the resources
    clReleaseKernel(cw.kernel);
    clReleaseProgram(cw.program);
    clReleaseContext(cw.context);
    clReleaseDevice(cw.pad.device_id);
    free(vs);
    free(vs2);
}