#include "PlatformAndDevices.h"
#include "utils.h"
#include "ClWrapper.h"

#include <stdio.h>
#include <stdlib.h>
#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <CL/cl_ext.h>

ClWrapper_t mlInit(char * path, char * prg_name)
{
  cl_int error_code;
  ClWrapper_t wrapper;
  // Create device and platform
  wrapper.pad = mlGetPlatformAndDevices();
  // Create OpenCL context
  wrapper.context = clCreateContext(NULL, wrapper.pad.n_devices, &(wrapper.pad.device_id), NULL, NULL, &error_code);
  if (error_code != CL_SUCCESS)
    printf("Error while creating context (mlInit : clCreateContext): %s\n",mlErrorHandler(error_code));
  // Build program (string path, cl_device_id device_id, cl_context context)
  wrapper.program = mlProgramBuilder(path, wrapper.pad.device_id, wrapper.context);
  // Create kernel (cl_program program, string functionName, cl_int error_code)
  wrapper.kernel = clCreateKernel(wrapper.program, prg_name, &error_code);
  if (error_code != CL_SUCCESS)
    printf("Error while creating kernel (mlInit : clCreateKernel): %s\n", mlErrorHandler(error_code));
  return wrapper;
}