#pragma once

#ifndef __LOGGER_CONSOLE
#define __LOGGER_CONSOLE

#include <Windows.h>
#include <queue>
#include <tchar.h>

#include "CriticalSection.h"
#include "Singleton.h"

#define MAX_BUFFER	512

typedef class cLogger : public cSingleton<cLogger>{
	CS cs;
	HANDLE hFile;
	HANDLE hIOCP;
public:
	cLogger();
	~cLogger();

	void Log(const char* msg, ...);
	void Error(const char* fileName, const char* funcName, int line, const char* msg, ...);
	void Error(const char* fileName, const char* funcName, int line, int code, const char* msg, ...);
}Debug;

#define LOG(msg, ...) cLogger::Instance()->Log(msg, __VA_ARGS__);
#define ERROR_MSG(msg, ...) cLogger::Instance()->Error(__FILE__, __FUNCTION__, __LINE__, msg, __VA_ARGS__);
#define ERROR_CODE(code, msg, ...) cLogger::Instance()->Error(__FILE__, __FUNCTION__, __LINE__, code, msg, __VA_ARGS__);

#endif