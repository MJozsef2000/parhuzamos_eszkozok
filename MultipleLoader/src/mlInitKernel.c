#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

void mlInitKernel(cl_kernel kernel, cl_context context, cl_mem buffers[], int buffer_count)
{
  int i;
  for (i = 0; i<buffer_count; i++){
    clSetKernelArg(kernel, i, sizeof(cl_mem), (void *)&buffers[i]);
  }
  return;
}