all:
	gcc -Iinclude/ -c src/mlSizeSpecification.c -o mlSizeSpecification.o
	gcc -Iinclude/ -c src/mlLoadKernelFromSource.c -o mlLoadKernelFromSource.o
	gcc -Iinclude/ -c src/mlReleaseResources.c -o mlReleaseResources.o
	gcc -Iinclude/ -c src/mlOutputFromDevice.c -o mlOutputFromDevice.o
	gcc -Iinclude/ -c src/mlExecCommandQueue.c -o mlExecCommandQueue.o
	gcc -Iinclude/ -c src/mlInit.c -o mlInit.o
	gcc -Iinclude/ -c src/mlInputToDevice.c -o mlInputToDevice.o
	gcc -Iinclude/ -c src/mlProgramBuilder.c -o mlProgramBuilder.o
	gcc -Iinclude/ -c src/mlInitKernel.c -o mlInitKernel.o
	gcc -Iinclude/ -c src/mlGetDeviceAndPlatform.c -o mlGetDeviceAndPlatform.o
	ar rcs ml.a mlSizeSpecification.o mlLoadKernelFromSource.o mlReleaseResources.o mlOutputFromDevice.o mlExecCommandQueue.o mlInit.o mlInputToDevice.o mlProgramBuilder.o mlInitKernel.o mlGetDeviceAndPlatform.o