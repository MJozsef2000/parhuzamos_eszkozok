#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <CL/cl.h>

#define CL_TARGET_OPENCL_VERSION 220

void mlInitKernel(cl_kernel kernel, cl_context context, cl_mem buffers[], int nr_of_ops)
{
  // Set kernel arguments
  clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&buffers[0]);
  clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&buffers[1]);
  clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&buffers[2]);
  clSetKernelArg(kernel, 3, sizeof(int), (void *)&nr_of_ops);
  return;
}