#include "utils.h"
#include "PlatformAndDevices.h"
#include "vectors.h"

#define CL_TARGET_OPENCL_VERSION 220
#define SIZEOF sizeof(int)
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>

const int SAMPLE_SIZE = 5;

int main(void)
{
    cl_int error_code;
    // Create device and platform
    PlatformAndDevices_t pad;
    mlGetPlatformAndDevices(&pad);
    // Create OpenCL context
    cl_context context = clCreateContext(NULL, pad.n_devices, &(pad.device_id), NULL, NULL, NULL);
    // Build program
    cl_program program = program_builder("kernels/sample.cl", pad.device_id, context);
    // Pass program and the function's name
    cl_kernel kernel = clCreateKernel(program, "vector_sum", &error_code);
    if (error_code != CL_SUCCESS)
        printf("Kernel building failed: %d\n", error_code);
    // Initialize kernel arguments
    Vectors v;
    v.a = (int *)malloc(SAMPLE_SIZE * sizeof(int));
    v.b = (int *)malloc(SAMPLE_SIZE * sizeof(int));
    v.c = (int *)malloc(SAMPLE_SIZE * sizeof(int));
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        v.a[i] = i;
        v.b[i] = i + 2;
    }
    // Create the device buffer
    cl_mem buffers[4];
    buffers[0] = clCreateBuffer(context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(int), NULL, NULL);
    buffers[1] = clCreateBuffer(context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(int), NULL, NULL);
    buffers[2] = clCreateBuffer(context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(int), NULL, NULL);
    
    mlInitKernel(
        kernel,
        context,
        buffers,
        SAMPLE_SIZE);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, pad.device_id, NULL, NULL);

    // Host buffer -> Device buffer
    clEnqueueWriteBuffer(
        command_queue,
        buffers[0],                  // changed
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(int), // changed
        v.a,                       // changed
        0,
        NULL,
        NULL);
    clEnqueueWriteBuffer(
        command_queue,
        buffers[1],                  // changed
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(int), // changed
        v.b,                       // changed
        0,
        NULL,
        NULL);

    // Size specification
    size_t local_work_size = 256; // Local id 0-255 ig
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    // Apply the kernel on the range
    clEnqueueNDRangeKernel(
        command_queue,
        kernel,
        1,
        NULL,
        &global_work_size,
        &local_work_size,
        0,
        NULL,
        NULL);
    clFinish(command_queue);
    // Host buffer <- Device buffer

    // OUTPUT
    clEnqueueReadBuffer(
        command_queue,
        buffers[2],                   // changed
        CL_TRUE,
        0,
        SAMPLE_SIZE * sizeof(int),  // changed
        v.c,                        // changed
        0,
        NULL,
        NULL);
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        printf("[%d]: %d,", i, v.a[i]);
        printf("[%d]: %d,", i, v.b[i]);
        printf("[%d]: %d,\n", i, v.c[i]);
    }
    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(pad.device_id);

    free(v.a);
    free(v.b);
    free(v.c);
}
