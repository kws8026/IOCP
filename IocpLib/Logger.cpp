#include "pch.h"
#include "Logger.h"
#include <iostream>
using namespace std;

cLogger::cLogger() : hFile(INVALID_HANDLE_VALUE)
{
	ZeroMemory(buffer,sizeof(buffer));
}

cLogger::~cLogger()
{
}

bool cLogger::Init()
{
	hFile = CreateFile(L"log.txt",GENERIC_ALL,0,NULL,CREATE_ALWAYS,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		ERROR_CODE(WSAGetLastError(),"Faid to CreatFile");
		return false;
	}
	LOG("Logger Ready");
	return true;
}

void cLogger::Log(const char* msg, ...)
{
	FastSpinlockGuard lock(lock_log);

	ZeroMemory(buffer, sizeof(buffer));
	va_list args;
	va_start(args, msg);
	len = vsnprintf_s(buffer, MAX_OF_BUFFER, MAX_OF_BUFFER - 1, msg, args);
	va_end(args);
	if (len < 0)
		return;
	cout << buffer << endl;
	WriteFile(hFile, buffer, len+2, NULL, NULL);
}

void cLogger::Error(const char* fileName, const char* funcName, int line, const char* msg, ...)
{
	FastSpinlockGuard lock(lock_log);

	ZeroMemory(buffer, sizeof(buffer));
	va_list args;
	va_start(args, msg);
	len = vsnprintf_s(buffer, MAX_OF_BUFFER, MAX_OF_BUFFER - 1, msg, args);
	va_end(args);
	if (len < 0)
		return;
	cout << "File: " << fileName << "\nFunction: " << funcName << "\nLine: " << line \
		<< "\nError: " << buffer << endl;
	buffer[len] = 0x0A0D;
	WriteFile(hFile, buffer, len + 2, NULL, NULL);
}

void cLogger::Error(const char* fileName, const char* funcName, int line, int code, const char* msg, ...)
{
	FastSpinlockGuard lock(lock_log);

	ZeroMemory(buffer, sizeof(buffer));
	va_list args;
	va_start(args, msg);
	len = vsnprintf_s(buffer, MAX_OF_BUFFER, MAX_OF_BUFFER - 1, msg, args);
	va_end(args);
	if (len < 0)
		return;
	cout << "File: " << fileName << "\nFunction: " << funcName << "\nLine: " << line \
		<< "\nError: " << buffer <<  "EroorCode: " << code << endl;
	buffer[len] = 0x0A0D;
	WriteFile(hFile, buffer, len + 2, NULL, NULL);
}
