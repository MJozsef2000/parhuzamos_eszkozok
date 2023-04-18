#ifndef VECTORS_H
#define VECTORS_H
#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
typedef struct __attribute__ ((packed)) vector{
    cl_int a;
    cl_int b;
    cl_int c;
    cl_int n;
}Vector;
#endif