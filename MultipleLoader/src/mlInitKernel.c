#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <CL/cl.h>

#define CL_TARGET_OPENCL_VERSION 220

void mlInitKernel(cl_kernel kernel, cl_context context, cl_mem buffers[], int buffer_count)
{
  int i;
  for (i = 0; i<buffer_count; i++){
    clSetKernelArg(kernel, i, sizeof(cl_mem), (void *)&buffers[i]);
  }
  return;
}