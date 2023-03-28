#include "kernel_loader.h"

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>

#include <stdio.h>
#include <stdlib.h>

const int SAMPLE_SIZE = 100000;

int main(void)
{
    cl_int error_code; //For error code handling

    // Get platform
    cl_uint n_platforms;
	cl_platform_id platform_id;
    error_code = clGetPlatformIDs(1, &platform_id, &n_platforms);
	if (error_code != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", error_code);
		return 0;
	}

    // Get device
	cl_device_id device_id;
	cl_uint n_devices;
	error_code = clGetDeviceIDs(
		platform_id,
		CL_DEVICE_TYPE_GPU,
		1,
		&device_id,
		&n_devices
	);
	if (error_code != CL_SUCCESS) {
		printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", error_code);
		return 0;
	}

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, n_devices, &device_id, NULL, NULL, NULL);

    // Build the program
    const char* kernel_code = load_kernel_source("kernels/sample.cl", &error_code);
    if (error_code != 0) {
        printf("Source code loading error!\n");
        return 0;
    }
    cl_program program = clCreateProgramWithSource(context, 1, &kernel_code, NULL, NULL);
    error_code = clBuildProgram(
        program,
        1,
        &device_id,
        NULL,
        NULL,
        NULL
    );
    //Error handling in case of faulty program building
    if (error_code != CL_SUCCESS) {
        printf("Build error! Code: %d\n", error_code);
        size_t real_size;
        error_code = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            0,
            NULL,
            &real_size
        );
        char* build_log = (char*)malloc(sizeof(char) * (real_size + 1));
        error_code = clGetProgramBuildInfo(
            program,
            device_id,
            CL_PROGRAM_BUILD_LOG,
            real_size + 1,
            build_log,
            &real_size
        );
        // build_log[real_size] = 0;
        printf("Real size : %d\n", real_size);
        printf("Build log : %s\n", build_log);
        free(build_log);
        return 0;
    }
    cl_kernel kernel = clCreateKernel(program, "rand_vector", NULL);

    // Create the host buffer and initialize it
    int * out_v = (int*) malloc(SAMPLE_SIZE*sizeof(int));
    // Create the device buffer // Dobozoló // 
    cl_mem out_v_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, SAMPLE_SIZE * sizeof(int), NULL, NULL);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&out_v_buffer);
    clSetKernelArg(kernel, 1, sizeof(int), (void*)&SAMPLE_SIZE);

    // Create the command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, NULL);

    /*// Host buffer -> Device buffer INPUT
    clEnqueueWriteBuffer(
        command_queue,
        a_buffer,
        CL_FALSE,
        0,
        SAMPLE_SIZE * sizeof(int), //changed
        out_v, //changed
        0,
        NULL,
        NULL
    );*/

    // Size specification
    size_t local_work_size = 256; //Local id 0-255 ig
    size_t n_work_groups = (SAMPLE_SIZE + local_work_size + 1) / local_work_size;
    size_t global_work_size = n_work_groups * local_work_size;

    // Apply the kernel on the range
    clEnqueueNDRangeKernel(
        command_queue,
        kernel,
        1,
        NULL,
        &global_work_size,
        &local_work_size,
        0,
        NULL,
        NULL
    );
    clFinish(command_queue);
    // Host buffer <- Device buffer
    //OUTPUT
    clEnqueueReadBuffer(
        command_queue,
        out_v_buffer,
        CL_TRUE,
        0,
        SAMPLE_SIZE * sizeof(int),
        out_v,
        0,
        NULL,
        NULL
    );
    
    for(int i=0; i<SAMPLE_SIZE; i++){
        printf("[%d]: %d\n", i, out_v[i]);
    }
    // Release the resources
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseDevice(device_id);

    free(out_v);
}
