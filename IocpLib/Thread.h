#pragma once

#ifndef __THREAD_IOCP
#define __THREAD_IOCP

#define _WINSOCKAPI_
#include <Windows.h>
#include <thread>

class cThread
{
	HANDLE	hThread;
	HANDLE	hIOCP;
	bool	bFlag;

	void	OnIOCP();
	void	OnSend();
public:
	cThread(HANDLE hThread, HANDLE hCompletionPort);
	~cThread();

	DWORD	Run();
	void	SetEndflag();
	HANDLE GetHandle() { return hThread; }
};

#endif