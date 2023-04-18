#ifndef UTILS_H
#define UTILS_H
#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include "PlatformAndDevices.h"
#include "SizeSpec.h"
#include "ClWrapper.h"

char* load_kernel_source(const char* const path, cl_int* error_code);
cl_program mlProgramBuilder(const char *const path, cl_device_id device_id, cl_context context);
PlatformAndDevices_t mlGetPlatformAndDevices();
void mlInitKernel(cl_kernel kernel, cl_context context, cl_mem buffers[], int buffer_size);
void mlInputToDevice(cl_command_queue command_queue, cl_mem input_buffers[], int buffer_count, size_t var_size, const void * ptr);
void mlOutputFromDevice(cl_command_queue command_queue, cl_mem output_buffers[], int buffer_count, size_t var_size, const void * ptr);
SizeSpec_t mlSizeSpecification(int nr_of_computations);
ClWrapper_t mlInit(char * path, char * prg_name);
void mlExecComandQueue(cl_command_queue command_queue, ClWrapper_t cw, SizeSpec_t s);
#endif
