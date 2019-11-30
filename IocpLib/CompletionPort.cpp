#include "pch.h"
#include "ClientSessionManager.h"

cCompletionPort::cCompletionPort() : hIOCP(NULL), listenSocket(NULL)
{
	arrThreads = new cThread*[MAX_OF_THREADS];
	ZeroMemory(
		arrThreads,
		sizeof(cThread*) * MAX_OF_THREADS);
}


cCompletionPort::~cCompletionPort()
{
}

bool cCompletionPort::Initialize()
{
	// ���� ����
	if (!NETWORK::SetWsa()) {
		return false;
	}

	// IOCP �ڵ� ����
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hIOCP == INVALID_HANDLE_VALUE)
	{	
		LOG_ERROR("Fail Create IOCP");
		return false;
	}

	// Listen ��Ĺ ����
	if (!NETWORK::CreateTCPSocket(listenSocket)) {
		return false;
	}

	// Listen ���� IOCP ���
	HANDLE handle = CreateIoCompletionPort((HANDLE)listenSocket, hIOCP, 0, 0);
	if (handle != hIOCP)
	{
		LOG_ERROR("Fail To Registe Listen socket to IOCP");
		return false;
	}

	// Listen ��Ĺ ���ε�
	if (!NETWORK::Bind(listenSocket)) {
		return false;
	}

	LOG("Server Initialize Successfully");
	return true;
}

void cCompletionPort::Close()
{
}

bool cCompletionPort::StartThreads()
{
	for (int i = 0; i < MAX_OF_THREADS; ++i)
	{
		unsigned __int64 threadIndex = i;
		DWORD dwThreadId = 0;

		/// ������ID�� DB ������ ���Ŀ� IO �������..
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, IoWorkerThread, (PVOID)threadIndex, CREATE_SUSPENDED, (unsigned int*)&dwThreadId);
		if (hThread == NULL) {
			return false;
		}

		arrThreads[i] = new cThread(hThread, hIOCP);
	}

	/// start!
	for (int i = 0; i < MAX_OF_THREADS; ++i)
	{
		ResumeThread(arrThreads[i]->GetHandle());
	}
	return true;
}

void cCompletionPort::StartAccept()
{
	/// listen
	if (SOCKET_ERROR == listen(listenSocket, SOMAXCONN))
	{
		LOG_ERROR("Fail to Listen");
		return;
	}

	LOG("Start Accept");

	while (CLIENTS->AcceptClientSessions()) // ���Ʈ �Լ�
	{
		Sleep(32);
	}

	LOG("End Accept");
}

unsigned int WINAPI cCompletionPort::IoWorkerThread(LPVOID lpParam)
{
	auto LWorkerThreadId = reinterpret_cast<unsigned __int64>(lpParam);

	LOG("Start WorkerThreadId: %I64u",LWorkerThreadId);

	return Instance()->arrThreads[LWorkerThreadId]->Run();
}