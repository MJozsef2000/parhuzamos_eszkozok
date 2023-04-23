#ifndef VECTORS_H
#define VECTORS_H
#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
typedef struct __attribute__ ((packed)) order_s{
    int order[100];
    int n;
}order_t;
#endif