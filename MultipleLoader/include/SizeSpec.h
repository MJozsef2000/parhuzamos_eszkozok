#ifndef SIZE_SPEC_H
#define SIZE_SPEC_H
#define CL_TARGET_OPENCL_VERSION 220
#include <CL/cl.h>
typedef struct SizeSpec_s
{
  size_t local_work_size;
  size_t n_work_groups;
  size_t global_work_size;
} SizeSpec_t;
#endif