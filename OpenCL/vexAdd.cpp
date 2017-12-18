// OPCL 中函数的具体实现

#include <stdio.h>
#include <stdlib.h>
#include "OPCL.h"

//----------------------- OpenCL 程序基本函数 ------------------------

inline char * ReadKernelSourceFile(const char * filename, size_t * length)

{
	FILE *file = NULL;
	size_t sourceLength;
	char *sourceStrnig;
	int ret;

	fopen_s(&file, filename, "rb");
	if (file == NULL)
	{
		printf("Can't open \n");
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	sourceLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	sourceStrnig = (char*)malloc(sourceLength + 1);
	sourceStrnig[0] = '\0';

	ret = fread(sourceStrnig, sourceLength, 1, file);
	if (ret == 0)
	{
		printf("无法读取源文件.\n");
		return NULL;
	}
	fclose(file);

	if (length != 0)
	{
		*length = sourceLength;
	}
	sourceStrnig[sourceLength] = '\0';

	return sourceStrnig;
}

cl_context CreatContext(cl_device_id * device)
{
	cl_int errNum;
	cl_uint numPlatforms;
	cl_platform_id firstPlatformId;
	cl_context context;

	errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);						//创建平台
	if (errNum != CL_SUCCESS || numPlatforms <= 0)
	{
		printf("找不到平台.\n");
		return NULL;
	}
	

	errNum = clGetDeviceIDs(firstPlatformId, CL_DEVICE_TYPE_GPU, 1, device, NULL);		//创建设备
	if (errNum != CL_SUCCESS)
	{
		printf("没有 GPU ,尝试使用 CPU\n");

		errNum = clGetDeviceIDs(firstPlatformId, CL_DEVICE_TYPE_CPU, 1, device, NULL);
	}
	if (errNum!=CL_SUCCESS)
	{
		printf("没有 GPU 和 CPU .\n");
		return NULL;
	}


	context = clCreateContext(NULL, 1, device, NULL, NULL, &errNum);	// device || *device ?
	if (errNum!=CL_SUCCESS)
	{
		printf("context 创建失败.\n");
		return NULL;
	}

	return context;
}

cl_command_queue CreatCommandQueue(cl_context context, cl_device_id device)
{
	cl_int errNum;		//???
	cl_command_queue commandQueue = NULL;

	commandQueue = clCreateCommandQueue(context, device, 0, NULL);
	if (!commandQueue)
	{
		printf("为设备0创建命令队列失败.\n");
		return NULL;
	}

	return commandQueue;
}

cl_program CreatProgram(cl_context context, cl_device_id device, const char * FileName)
{
	cl_int errNum;
	cl_program program;
	size_t program_length;
	char *const source = ReadKernelSourceFile(FileName, &program_length);

	program = clCreateProgramWithSource(context, 1, (const char**)&source, NULL, NULL);
	if (!program)
	{
		printf("Failed to creat CL program from source.\n");
		return NULL;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum!=CL_SUCCESS)
	{
		char buildLog[16384];

		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
		printf("Error in kernel:%s", buildLog);
		clReleaseProgram(program);

		return NULL;
	}

	return program;
}

bool CreatMemObject(cl_context context, cl_mem memObjects[3], float * a, float * b)
{
	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*ARRAY_SIZE, a, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(float)*ARRAY_SIZE, b, NULL);
	memObjects[2] = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float)*ARRAY_SIZE, NULL, NULL);						//注: 是 WRITE_ONLY ---- 写错

	if (memObjects[0] == NULL || memObjects[1] == NULL || memObjects[2] == NULL)
	{
		printf("Error creating memory objects.\n");

		return false;
	}

	return true;
}

void CleanUp(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem memObject[3])
{
	for (int i = 0; i < 3; i++)
	{
		if (memObject[i] != 0)
		{
			clReleaseMemObject(memObject[i]);
		}
	}

	if (commandQueue != 0)
	{
		clReleaseCommandQueue(commandQueue);
	}
	if (kernel != 0)
	{
		clReleaseKernel(kernel);
	}
	if (program != 0)
	{
		clReleaseProgram(program);
	}
	if (context != 0)
	{
		clReleaseContext(context);
	}

	return;
}