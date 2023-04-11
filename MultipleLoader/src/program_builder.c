#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220

cl_program program_builder(const char *const path, cl_device_id device_id, cl_context context)
{
  cl_int error_code;
  // Build the program
  const char *kernel_code = load_kernel_source(path, &error_code);
  if (error_code != 0)
  {
    printf("Source code loading error! Source mistake? (%s)\nwith error: %d\n",path,error_code);
    return 0;
  }
  cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
  error_code = clBuildProgram(
      program,
      1,
      &device_id,
      NULL,
      NULL,
      NULL);
  // Error handling in case of faulty program building
  if (error_code != CL_SUCCESS)
  {
    printf("Build error! Code: %d\n", error_code);
    size_t real_size;
    error_code = clGetProgramBuildInfo(
        program,
        device_id,
        CL_PROGRAM_BUILD_LOG,
        0,
        NULL,
        &real_size);
    char *build_log = (char *)malloc(sizeof(char) * (real_size + 1));
    error_code = clGetProgramBuildInfo(
        program,
        device_id,
        CL_PROGRAM_BUILD_LOG,
        real_size + 1,
        build_log,
        &real_size);
    // build_log[real_size] = 0;
    printf("Real size : %d\n", real_size);
    printf("Build log : %s\n", build_log);
    free(build_log);
  }
  return program;
}