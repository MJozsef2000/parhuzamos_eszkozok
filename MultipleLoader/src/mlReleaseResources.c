#include "ClWrapper.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

void mlReleaseResources(ClWrapper_t cw){
  clReleaseKernel(cw.kernel);
  clReleaseProgram(cw.program);
  clReleaseContext(cw.context);
  clReleaseDevice(cw.pad.device_id);
}