#include "pch.h"
#include "Thread.h"
#include "OverlappedIOContext.h"

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

	LPSESSION remote = context ? context->owner : nullptr;

	if (ret == 0 || dwTransferred == 0)
	{
		if (context == nullptr && GetLastError() == WAIT_TIMEOUT) {
			return;
		}
	}

	bool bResult = false;

	switch (context->cmd)
	{
	case IO_Connect:
		dynamic_cast<cServerSession*>(remote)->ConnectCompletion();
		break;

	case IO_Accept:
		dynamic_cast<cClientSession*>(remote)->AcceptCompletion();
		break;

	case IO_Send:
		remote->SendCompletion(dwTransferred);
		break;

	case IO_Recv:
		remote->RecvCompletion(dwTransferred);
		break;

	default:
		break;
	}
}

void cThread::OnSend()
{
}
