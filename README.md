# Repository for "párhuzamos eszközök" course at Miskolci Egyetem + MultipleLoader
This repository is for my school work and school project, which consists of a custom
made API to handle OpenCL tasks easier. The development of this project is 
at amateur levels and should be taken at face value, it is just merely a concept.

The main goal of the project was for me to get closer to the OpenCL standard,
while also making a more user friendly API for my peers to use during this course.

The MultipleLoader folder contains the source code for the API, which basically 
narrows down the overhead that is required from the user to start OpenCL kernels.
The MLRelease folder holds the released version of the API. 

# Usage
Copy the ml.a archive from the released version right next to your makefile, and link it like so:
```makefile
all:
	gcc example.c s -o example.exe -Iinclude ./ml.a -lOpenCL 
```

# Utilities provided by MultipleLoader
A lot of the overhead that came with OpenCL was originating from multiple stages of initialization. While these initialization steps are still necessary, they are 
made as user friendly as possible. Note that few of the changes impact the versatility
of the OpenCL library, so this API may not be suitable in every case, but it offers a 
great advantage to those who just start getting used to it.

## Initialize the kernel for a method
```c
  ClWrapper_t cw = mlInit("kernels/your_cl_file.cl","your_method");
```
MultipleLoader introduces a bundle which describes the context the kernel runs on, called ClWrapper. This wrapper contains essential information, like the program object, the context, device information, and so on. The user had to initialize these one by one, but now this is taken care of by mlInit, which requires the location and name of the kernel method, and returns an already assembled ClWrapper instance. Note
that while this is extremely easy, it has it's downsides: Currently ClWrapper is only 
suitable for 1 device, which means several GPU setups will require the original OpenCL implementation.

## Input and output information is a packed structure
For streamlining the passing and retrieving of input and output data, they are all bundled into their respective structures. For example, in the case of vector addition,
we declare the following structure:
```c
typedef struct __attribute__ ((packed)) vectors{
    cl_int a[5]; //Input
    cl_int b[5]; //Input
    cl_int c[5]; //Output
    cl_int n; //Size of array
}Vectors;
```
With the ((packed)) attribute we can ensure the structure arrives in the same memory 
size as the Kernel awaits it. Do note that the structure has to be declared in the kernel code too, like so:
```c
typedef struct __attribute__ ((packed)) vector{
    int a[5];
    int b[5];
    int c[5];
    int n;
}Vector;

__kernel void vector_sum(__global Vector * vs)
{
    int GID = get_global_id(0);
    if (GID < vs->n){
        vs->c[GID] = vs->a[GID] + vs->b[GID];
        printf("kernel %d: %d + %d = %d\n",GID,vs->a[GID],vs->b[GID],vs->c[GID]);
    }
}
```
It is recommended to use types provided by OpenCL on the host (CPU) side, but it is not required.

## Create buffer(s) for input and output data
The host has to establish a "vessel" to communicate with the device. We call these vessels buffers, which are special cl_mem typed memory fields, initialized with the established context, memory type, and size. Since our way of sending and retrieving data to and from the device is using structs, we just have to give it's size to the buffer initializer. This is done by a standard OpenCL function, because fine tuning is important here.
```c
cl_mem buffers[1];
buffers[0] = clCreateBuffer(cw.context, CL_MEM_READ_WRITE, sizeof(Vectors), NULL, NULL);
```
Note that we store the buffers in an array of cl_mem instances. This is needed for a future step, so it's recommended to follow this practice for even one structure tasks.

