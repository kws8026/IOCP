#include "pch.h"
#include "ServerSession.h"
#include "OverlappedIOContext.h"

using namespace NETWORK;

#define SIZE_BUFFER_SERVER 8

cServerSession::cServerSession(const char* serverAddr) : 
	serverAddr(serverAddr),SESSION(SIZE_BUFFER_SERVER, SIZE_BUFFER_SERVER)
{
}

cServerSession::~cServerSession()
{
}

bool cServerSession::ConnectRequest()
{
	SOCKADDR_IN serverSockAddr;
	ZeroMemory(&serverSockAddr, sizeof(serverSockAddr));
	auto ret = inet_pton(AF_INET, serverAddr, (void*)&serverSockAddr.sin_addr.s_addr);
	serverSockAddr.sin_port = htons(PORT);

	if (SOCKET_ERROR == bind(sock, (SOCKADDR*)&serverSockAddr, sizeof(serverSockAddr)))
	{
		return false;
	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)sock, IOCP->GetHandle(), (ULONG_PTR)this, 0);
	if (handle != IOCP->GetHandle())
	{
		return false;
	}


	LPCONTEXT_CON context = NEW(CONTEXT_CON);

	if (FALSE == ConnectEx(sock,addr,(LPWSAOVERLAPPED)context))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(context);
			LOG_ERROR("Fail to ConnectEx");
		}
	}

	return true;
}

bool cServerSession::ConnectCompletion()
{
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0))
	{
		return false;
	}

	int opt = 1;
	if (SOCKET_ERROR == setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
	{
		return false;
	}

	opt = 0;
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
	{
		return false;
	}

	if (false == PostRecv())
	{
		return false;
	}

	return true;
}
