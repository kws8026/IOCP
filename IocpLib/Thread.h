#pragma once

#ifndef __THREAD_IOCP
#define __THREAD_IOCP
class cThread
{
	HANDLE	hThread;
	HANDLE	hIOCP;
	bool	bFlag;
public:
	cThread(HANDLE hThread, HANDLE hCompletionPort);
	~cThread();

	DWORD	Run();
	void	SetEndflag();

	void	OnIOCP();
	void	OnSend();

	HANDLE GetHandle() { return hThread; }
};

#endif