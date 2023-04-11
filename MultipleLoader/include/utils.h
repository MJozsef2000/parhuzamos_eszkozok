#ifndef UTILS_H
#define UTILS_H
#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include "PlatformAndDevices.h"

char* load_kernel_source(const char* const path, cl_int* error_code);
cl_program program_builder(const char *const path, cl_device_id device_id, cl_context context);
cl_int mlGetPlatformAndDevices(PlatformAndDevices_t* pad);
void mlInitKernel(cl_kernel kernel, cl_context context, cl_mem buffers[], int nr_of_ops);
#endif
