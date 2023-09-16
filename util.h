#ifndef __UTIL_H__
#define __UTIL_H__

#include<pthread.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdint.h>
#include<string>
#include<vector>

pid_t GetThreadId();//获取线程号

uint32_t GetFiberId();//获取协程号

void Backtrace(std::vector<std::string>& bt,int size,int skip = 1);

std::string BacktraceToString(int size,int skip = 2z,const std::string& prefix = "");

#endif