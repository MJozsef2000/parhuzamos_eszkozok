#include "PlatformAndDevices.h"

#include <stdio.h>
#include <stdlib.h>

#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
#include <CL/cl_ext.h>

PlatformAndDevices_t mlGetPlatformAndDevices()
{
    cl_int error_code; // For error code handling
    PlatformAndDevices_t pad;
    // Get platform
    cl_uint n_platforms;
    cl_platform_id platform_id;
    error_code = clGetPlatformIDs(1, &platform_id, &n_platforms);
    if (error_code != CL_SUCCESS)
    {
        printf("[ERROR] Error calling clGetPlatformIDs. Error code: %d\n", error_code);
    }

    // Get device
    cl_device_id device_id;
    cl_uint n_devices;
    error_code = clGetDeviceIDs(
        platform_id,
        CL_DEVICE_TYPE_GPU,
        1,
        &device_id,
        &n_devices);
    if (error_code != CL_SUCCESS)
    {
        printf("[ERROR] Error calling clGetDeviceIDs. Error code: %d\n", error_code);
    }
    pad.device_id = device_id;
    pad.n_devices = n_devices;
    return pad;
}