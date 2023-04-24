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
of the OpenCL