#pragma once

#ifndef __LOGGER_CONSOLE
#define __LOGGER_CONSOLE

#include <Windows.h>
#include <queue>
#include <tchar.h>

#include "FastSpinlock.h"
#include "Singleton.h"
#include "CircularBuffer.h"

typedef class cLogger : public cSingleton<cLogger>{
	SPINLOCK		lock_log;
	HANDLE			hFile;
	char			buffer[MAX_OF_BUFFER];
	int				len;
public:
	cLogger();
	~cLogger();
	bool Init();
	void Log(const char* msg, ...);
	void Error(const char* fileName, const char* funcName, int line, const char* msg, ...);
	void Error(const char* fileName, const char* funcName, int line, int code, const char* msg, ...);
}Debug;

#define LOG(msg, ...) cLogger::Instance()->Log(msg, __VA_ARGS__)
#define LOG_ERROR(msg, ...) cLogger::Instance()->Error(__FILE__, __FUNCTION__, __LINE__, msg, __VA_ARGS__)
#define ERROR_CODE(code, msg, ...) cLogger::Instance()->Error(__FILE__, __FUNCTION__, __LINE__, code, msg, __VA_ARGS__)
#define INIT_LOG() cLogger::Instance()->Init()
#endif