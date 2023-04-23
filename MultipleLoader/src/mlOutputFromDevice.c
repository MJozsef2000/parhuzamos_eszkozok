#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

void mlOutputFromDevice(cl_command_queue command_queue, cl_mem output_buffers[], int buffer_count, size_t var_size, void * ptrs[])
{
  cl_int error_code;
  for(int i = 0; i < buffer_count; i++){
      error_code = clEnqueueReadBuffer(
        command_queue,
        output_buffers[i],     
        CL_TRUE,
        0,
        var_size,       
        ptrs[i],                  
        0,
        NULL,
        NULL);
        if (error_code != CL_SUCCESS){
          printf("Error while reading from device! Error code: %d\n", error_code);
        }
  }
  return;
}