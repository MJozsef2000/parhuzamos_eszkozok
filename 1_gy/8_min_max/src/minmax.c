#include "utils.h"
#include "PlatformAndDevices.h"
#include "minmaxorder.h"
#include "SizeSpec.h"
#include "ClWrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CL\cl_ext.h>
void minmax(){
    srand(time(NULL));
    ClWrapper_t cw = mlInit("kernels/minmax.cl","minmax");
    order_t o;
    for (int i=0; i<100; i++)
        o.order[i] = rand() % 101;;
    o.n = 100;
    printf("listing order:\n");
    for (int i = 0; i < 100; i++)
    {
        printf("[%d]: %d, ", i, o.order[i]);
    }
    o.minimum = &(o.order[0]);
    o.maximum = &(o.order[99]);
    printf("min: %d, max:%d\n", *(o.minimum), *(o.maximum));
    printf("\n");
    const int buffer_count = 1;
    const int input_param_count = 1;
    const int output_param_count = 1;
    cl_mem buffers[buffer_count];
    buffers[0] = clCreateBuffer(cw.context, CL_MEM_READ_WRITE, sizeof(order_t), NULL, NULL);

    mlInitKernel(cw.kernel, cw.context, buffers, buffer_count);

    cl_command_queue command_queue = clCreateCommandQueueWithProperties(cw.context, cw.pad.device_id, NULL, NULL);

    void * i_ptrs[input_param_count];
    i_ptrs[0] = &o;

    mlInputToDevice(command_queue, buffers, buffer_count, sizeof(order_t), i_ptrs);

    SizeSpec_t s = mlSizeSpecification1D(100,10);

    mlExecComandQueue(command_queue, cw, s);

    void * o_ptrs[output_param_count];
    o_ptrs[0] = &o;

    mlOutputFromDevice(command_queue, buffers, buffer_count,sizeof(order_t), o_ptrs);
    printf("min: %d, max:%d\n", *(o.minimum), *(o.maximum));
    mlReleaseResources(cw);
}