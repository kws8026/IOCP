#include "pch.h"
#include "Logger.h"
#include <iostream>
using namespace std;

cLogger::cLogger() : hFile(INVALID_HANDLE_VALUE), logBuffer(4096)
{
	tempBuffer = new char[MAX_OF_BUFFER];
}

cLogger::~cLogger()
{
	delete[] tempBuffer;
}

void cLogger::Log(const char* msg, ...)
{
	FastSpinlockGuard lock(lock_log);

	strcpy_s(tempBuffer,MAX_OF_BUFFER, msg);
	logBuffer.push(tempBuffer);
	cout << tempBuffer << endl;
	ZeroMemory(tempBuffer, MAX_OF_BUFFER);
}

void cLogger::Error(const char* fileName, const char* funcName, int line, const char* msg, ...)
{
	FastSpinlockGuard lock(lock_log);

	sprintf_s(tempBuffer, MAX_OF_BUFFER,"%s %s %d : %s", fileName, funcName, line, msg);
	logBuffer.push(tempBuffer);
	cout << tempBuffer << endl;
	ZeroMemory(tempBuffer,MAX_OF_BUFFER);
}

void cLogger::Error(const char* fileName, const char* funcName, int line, int code, const char* msg, ...)
{
	FastSpinlockGuard lock(lock_log);

	sprintf_s(tempBuffer, MAX_OF_BUFFER, "%s %s %d [%d]: %s", fileName, funcName, line, code, msg);
	logBuffer.push(tempBuffer);
	cout << tempBuffer << endl;
	ZeroMemory(tempBuffer, MAX_OF_BUFFER);
}
