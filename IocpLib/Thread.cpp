#include "pch.h"
#include "Thread.h"

cThread::cThread(HANDLE hThread, HANDLE hCompletionPort)
	:hThread(hThread), hIOCP(hCompletionPort), bFlag(true)
{
}

cThread::~cThread()
{
	CloseHandle(hThread);
}

DWORD cThread::Run()
{
	while (bFlag) {
		OnIOCP();
	}
	return 0;
}

void cThread::SetEndflag()
{
	bFlag = false;
}

void cThread::OnIOCP()
{
	DWORD dwTransferred = 0;
	LPOVERLAPPED overlapped = nullptr;

	ULONG_PTR completionKey = 0;

	int ret = GetQueuedCompletionStatus(hIOCP, &dwTransferred, (PULONG_PTR)&completionKey, &overlapped, INFINITE);
	LPIOCONTEXT context = reinterpret_cast<LPIOCONTEXT>(overlapped);

	LPSESSION remote = context ? context->pSession : nullptr;

	if (ret == 0 || dwTransferred == 0)
	{
		if (context == nullptr && GetLastError() == WAIT_TIMEOUT) {
			return;
		}
	}

	if (nullptr != remote) { return; };

	bool bResult = false;

	switch (context->cmd)
	{
	case IO_Connect:
		break;

	case IO_Accept:
		break;

	case IO_Send:
		break;

	case IO_Recv:
		break;

	default:
		break;
	}
}

void cThread::OnSend()
{
}
