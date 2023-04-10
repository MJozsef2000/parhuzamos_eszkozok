#ifndef KERNEL_LOADER_H
#define KERNEL_LOADER_H
#include <CL/cl.h>

char* load_kernel_source(const char* const path, cl_int* error_code);
cl_program program_builder(const char *const path, cl_int* error_code, cl_device_id device_id, cl_context context);

#endif
