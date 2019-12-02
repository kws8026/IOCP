#include "pch.h"
#include "ServerSession.h"
#include "OverlappedIOContext.h"

using namespace NETWORK;

#define SIZE_BUFFER_SERVER 4

cServerSession::cServerSession(const char* serverAddr) : 
	serverAddr(serverAddr),SESSION(SIZE_BUFFER_SERVER, SIZE_BUFFER_SERVER)
{
	CreateIOPool(50);
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

cServerSession::~cServerSession()
{
}

bool cServerSession::Connect()
{
	auto ret = inet_pton(AF_INET, serverAddr, (void*)&addr.sin_addr.s_addr);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	if(INVALID_SOCKET == connect(sock, (SOCKADDR*)&addr, sizeof(addr))){
		return false;
	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)sock, IOCP->GetHandle(), (ULONG_PTR)this, 0);
	if (handle != IOCP->GetHandle())
	{
		return false;
	}
	return true;
}

bool cServerSession::ConnectRequest()
{

	auto ret = inet_pton(AF_INET, serverAddr, (void*)&addr.sin_addr.s_addr);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));
	if (SOCKET_ERROR ==bind(sock,(SOCKADDR*) &addr, sizeof(addr)))
	{
		ERROR_CODE(WSAGetLastError(), "Fail to Bind");
		return false;
	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)sock, IOCP->GetHandle(), (ULONG_PTR)this, 0);
	if (handle != IOCP->GetHandle())
	{
		return false;
	}


	LPCONTEXT_CON context = NEW(CONTEXT_CON);
	context->SetSession(this);

	if (FALSE == ConnectEx(sock,addr,(LPWSAOVERLAPPED)context))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(context);
			ERROR_CODE(WSAGetLastError(),"Fail to ConnectEx");
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
