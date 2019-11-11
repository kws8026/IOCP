#include "pch.h"
#include "Logger.h"

cLogger::cLogger():hFile(INVALID_HANDLE_VALUE)
{
}

cLogger::~cLogger()
{
}

void cLogger::Log(const char* msg, ...)
{
}

void cLogger::Error(const char* fileName, const char* funcName, int line, const char* msg, ...)
{
}

void cLogger::Error(const char* fileName, const char* funcName, int line, int code, const char* msg, ...)
{
}
