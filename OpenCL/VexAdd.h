#pragma once

#ifdef _APPLE_
#include <OpenCL/cl.h>
#else
#include <CL\cl.h>
#endif // _APPLE_

//---------------------------- 基本参数 ------------------------------

const int ARRAY_SIZE = 1000;


//----------------------- OpenCL 程序基本函数 ------------------------

inline char* ReadKernelSourceFile(const char *filename, size_t *length);

/*
1.创建平台
2.创建设备
3.根据设备创建上下文
*/
cl_context CreatContext(cl_device_id *device);												//根据 device 创建 context

cl_command_queue CreatCommandQueue(cl_context context, cl_device_id device);				//在 context 中第一个可用的 device 中创建 command queue

cl_program CreatProgram(cl_context context, cl_device_id device, const char *FileName);		//读取内核源码 kernel 创建 OpenCL 程序

bool CreatMemObject(cl_context context, cl_mem memObjects[3], float *a, float *b);			//在 contex 中创建 memObject

void CleanUp(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem memObject[3]);		//清除 OpenCL 资源