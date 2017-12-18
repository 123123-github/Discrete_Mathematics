// OpenCL 实现矢量相加
// 运行环境 vs2017

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include "OPCL.h"

int main()
{
	cl_device_id device = 0;
	cl_context context = 0;
	cl_command_queue commandQueue = 0;
	cl_mem memObject[3] = { 0,0,0 };
	cl_program program = 0;
	cl_kernel kernel = 0;
	cl_int errNum;									//返回值
	bool status;

	//创建上下文
	context = CreatContext(&device);
	if (!context)
	{
		printf(" context 创建失败.\n");
		return 1;
	}

	//创建命令队列
	commandQueue = CreatCommandQueue(context, device);
	if (!commandQueue)
	{
		CleanUp(context, commandQueue, program, kernel, memObject);
		return 1;
	}

	//创建 OpenCL 程序
	program = CreatProgram(context, device, "vecAdd.cl");
	if (!program)
	{
		CleanUp(context, commandQueue, program, kernel, memObject);
		return 1;
	}

	//创建 OpenCl 内核
	kernel = clCreateKernel(program, "vector_add", NULL);
	if (!kernel)
	{
		printf("创建 kernel 失败.\n");
		CleanUp(context, commandQueue, program, kernel, memObject);
		return 1;
	}

	//创建 OpenCl 内存对象
	float a[ARRAY_SIZE];
	float b[ARRAY_SIZE];
	float result[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		a[i] = float(i);
		b[i] = float(i * 2);
	}
	status = CreatMemObject(context, memObject, a, b);
	if (status == false)
	{
		CleanUp(context, commandQueue, program, kernel, memObject);
		return 1;
	}

	//设置内核参数
	errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObject[0]);
	errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObject[1]);
	errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObject[2]);
	if (errNum!=CL_SUCCESS)
	{
		printf("设置内核参数失败.\n");
		CleanUp(context, commandQueue, program, kernel, memObject);
		return 1;
	}

	//设置 kernel 函数的执行尺寸
	size_t globalWorkSize[1] = { ARRAY_SIZE };
	size_t localWorkSize[1] = { 1 };

	//提交至命令队列,执行内核
	errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		printf("Error queuing kernel for executtion.\n");
		CleanUp(context, commandQueue, program, kernel, memObject);
		return 1;
	}

	//计算结果拷贝回主机
	errNum = clEnqueueReadBuffer(commandQueue, memObject[2], CL_TRUE, 0, ARRAY_SIZE * sizeof(float), result, 0, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		printf("Error reading result buffer.\n");
		CleanUp(context, commandQueue, program, kernel, memObject);
		return 1;
	}

	//输出结果
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf("i = %d :%f\n", i, result[i]);
	}

	printf("程序执行成功.\n");
	CleanUp(context, commandQueue, program, kernel, memObject);

	system("pause");

    return 0;
}
