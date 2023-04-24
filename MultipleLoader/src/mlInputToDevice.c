#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <CL/cl_ext.h>


void mlInputToDevice(cl_command_queue command_queue, cl_mem input_buffers[], int buffer_count, size_t var_size, void * ptrs[])
{
  cl_int error_code;
  for(int i = 0; i < buffer_count; i++){
      error_code = clEnqueueWriteBuffer(
        command_queue,
        input_buffers[i],     
        CL_TRUE,
        0,
        var_size,       
        ptrs[i],                  
        0,
        NULL,
        NULL);
        if (error_code != CL_SUCCESS){
          printf("Error while writing to device! Error code: %d\n", error_code);
        }
  }
  return;
}