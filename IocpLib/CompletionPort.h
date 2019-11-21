#pragma once

#ifndef __COMPLETIONPORTSERVER__IOCP
#define __COMPLETIONPORTSERVER__IOCP

#include "Thread.h"
#include "Singleton.h"

static int MAX_OF_THREADS = 0; // 알아서 바꿀것

class cCompletionPort: public cSingleton<cCompletionPort>
{
	HANDLE		hIOCP;
	SOCKET		listenSocket;
	cThread**	arrThreads;

	static unsigned int WINAPI IoWorkerThread(LPVOID lpParam);
public:
	cCompletionPort();
	~cCompletionPort();

	bool Initialize();
	void Close();

	bool StartThreads();
	void StartAccept();

};

#endif 