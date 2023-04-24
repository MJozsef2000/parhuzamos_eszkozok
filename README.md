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

## 1. Initialize the kernel for a method
```c
ClWrapper_t cw = mlInit("kernels/your_cl_file.cl","your_method");
```
MultipleLoader introduces a bundle which describes the context the kernel runs on, called ClWrapper. This wrapper contains essential information, like the program object, the context, device information, and so on. The user had to initialize these one by one, but now this is taken care of by mlInit, which requires the location and name of the kernel method, and returns an already assembled ClWrapper instance. Note
that while this is extremely easy, it has it's downsides: Currently ClWrapper is only 
suitable for 1 device, which means several GPU setups will require the original OpenCL implementation.

## 2. Input and output information is a packed structure
For streamlining the passing and retrieving of input and output data, they are all bundled into their respective structures. For example, in the case of vector addition,
we declare the following structure:
```c
typedef struct __attribute__ ((packed)) vectors{
  cl_int a[32]; //Input
  cl_int b[32]; //Input
  cl_int c[32]; //Output
  cl_int n; //Size of array
}Vectors;
```
With the ((packed)) attribute we can ensure the structure arrives in the same memory 
size as the Kernel awaits it. Do note that the structure has to be declared in the kernel code too, like so:
```c
typedef struct __attribute__ ((packed)) vector{
  int a[32];
  int b[32];
  int c[32];
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

After this step, it's recommended to initialize the data on the host side.

## 3. Create buffer(s) for input and output data
The host has to establish a "vessel" to communicate with the device. We call these vessels buffers, which are special cl_mem typed memory fields, initialized with the established context, memory type, and size. Since our way of sending and retrieving data to and from the device is using structs, we just have to give it's size to the buffer initializer. This is done by a standard OpenCL function, because fine tuning is important here.
```c
cl_mem buffers[1];
buffers[0] = clCreateBuffer(cw.context, CL_MEM_READ_WRITE, sizeof(Vectors), NULL, NULL);
```
Note that we store the buffers in an array of cl_mem instances. This is needed for a future step, so it's recommended to follow this practice for even one structure tasks.

## 4. Prepare the kernel for operations with the buffer(s)
Once buffers have been initialized, they are passed onto a MultipleLoader function which prepares the kernel for the incoming data. For this we need to provide the kernel and context from our ClWrapper, the buffers (which we already initialized), and the number of buffers.
```c
mlInitKernel(cw.kernel, cw.context, buffers, 1);
```

## 5. Create command queue
After initializing the kernel, we need to create the command queue which describes a stream of commands we will give to the kernel to execute. This is done by the standard OpenCl clCreateCommandQueueWithProperties function, which takes context and device properties.
```c
cl_command_queue command_queue = clCreateCommandQueueWithProperties(
  cw.context, cw.pad.device_id, NULL, NULL);
```
Additional parameters include an option to provide more command queue parameters, and a field to catch an error with type cl_error. We leave these fields on NULL, but they can be utilized without interference with the ml API.

## 6. Transform the input
The OpenCL way of reading and writing variables (parameters) is by passing them as a 
unified void * type, so we naturally have to cast our structure(s) to this type as well. One way of doing this is to create a pointer of type void * and cast it upon the structure(s) we wish to pass to the device. For instance, we will create an array of void * pointers, and assign them the structures of data we wish to pass onto the device.
```c
void * i_ptrs[1];
i_ptrs[0] = &vs; // vs is a Vectors variable, already filled with input data
```
It is outmost important that if a kernel expects a certain order of parameters, 
they should be in the EXACT order in the input void * pointers array too.

## 7. Sending data from the host (CPU) to device (GPU)
After assembling the void * for data transfer, we can pass it to a MultipleLoader function which will send every input parameter described in the pointers to the device.
We need to provide the previously established command queue, buffers and their length, and also the size of the input(s). In case of different sizes, another mlInputToDevice command can be called, providing the corresponding buffers, size and pointers.
```c
mlInputToDevice(command_queue, buffers, buffer_count, sizeof(Vectors), i_ptrs);
```
While this process needed a command for every variable in the past, it is now easier to pass similar data structures, especially in the case of smaller operations, where one structure can contain both input and output data.


In case of several different structures, it is recommended to separate them to different buffers, so the input process can become easier with large amount of data.

## 8. Provide size specification for the kernel
When doing parallel computations, we have to tell the device(s) how much work needs to be done, so it can optimize it's performance as much as possible. For this reason, we have to tell the kernel how much data needs to be processed, and how can it be divided to groups. For instance, lets say we want to add 32 element vectors together. This task requires 32 threads to maximize performance, which can be bundled in groups of 2, 3, 4, 8, 16 ... 512. This is only a recommendation, as OpenCL 2.0 and above supports odd number of groups as well.
```c
//32 computations divided into 4 groups
SizeSpec_t s = mlSizeSpecification1D(32,4);
```
As the name suggests, this is only for one dimensional problems. For 2 dimensional
problems, we need to use:
```c
//32 computations divided into 4 groups for dimension 1, and 64 computations divided into 8 groups for dimension 2
SizeSpec_t s = mlSizeSpecification2D(32,4,64,8);
```

## 9. Execute (apply) kernel on the range
After specifying the size requirements to our computations, we can launch the operation on the kernel. For this we provide the command queue, the ClWrapper instance we defined, and the size specifications.
```c
mlExecComandQueue(command_queue, cw, s);
```

## 10. Transform the output
Just like previously described in section 6, OpenCL prefers a void * approach to extracting data from the kernel too. For this purpose, we can either use our input pointers made in step 6 - if the provided structure contains data for the output too, for example a c vector for results - or create another batch of output void * pointers. For the sake of this demonstration, we will create an output pointer array too:
```c
void * o_ptrs[output_param_count];
o_ptrs[0] = &vs; //Vectors variable, contains a field for output
```
## 11. Read data from kernel
After the kernel is done with computation, its time to use the output pointers to read 
data from the kernel. The process is very similar to how the input is processed:
```c
mlOutputFromDevice(command_queue, buffers, buffer_count,sizeof(Vector), o_ptrs);
```

## 12. Cleanup
At the end of the program, it is recommended to free up any OpenCL instances created during the process, as well as other memory spaces taken by the user.
```c
mlReleaseResources(cw);
```

# Closing remarks
As stated in the beginning, this work is still very basic and needs a lot of touches to serve a wider range of users, but for demonstration - and learning - purposes, I find this alternative a fitting first level for anyone interested in OpenCL programming.