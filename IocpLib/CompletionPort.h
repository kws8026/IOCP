#pragma once

#ifndef __COMPLETIONPORTSERVER__IOCP
#define __COMPLETIONPORTSERVER__IOCP

#include <Windows.h>

#include "Thread.h"
#include "Singleton.h"


class cCompletionPort: public cSingleton<cCompletionPort>
{
	HANDLE		hIOCP;
	SOCKET		listenSocket;
	cThread**	arrThreads;
	size_t		MAX_OF_THREADS;
	static unsigned int WINAPI IoWorkerThread(LPVOID lpParam);
public:
	cCompletionPort();
	~cCompletionPort();

	bool Initialize();
	void Close();

	bool StartThreads(size_t threads =2);
	void StartAccept();

	SOCKET GetListenSocket() { return listenSocket; };
	HANDLE GetHandle() { return hIOCP; };
};

#define IOCP cCompletionPort::Instance()

#endif 