#include "pch.h"
#include "ClientSession.h"
#include "OverlappedIOContext.h"
#include "CompletionPort.h"

using namespace NETWORK;

cClientSession::cClientSession() : SESSION()
{
}

cClientSession::~cClientSession()
{
}

void cClientSession::ResetSession()
{
	FastSpinlockGuard criticalSection(lock);
	memset(&addr, 0, sizeof(SOCKADDR_IN));

	LINGER lingerOption;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;

	/// no TCP TIME_WAIT
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(LINGER)))
	{
		LOG("setsockopt linger option error: %d\n", GetLastError());
	}

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool cClientSession::PostAccept()
{
	LPCONTEXT_ACCEPT acceptContext = NEW(CONTEXT_ACCEPT);
	acceptContext->buf.len = 0;
	acceptContext->buf.buf = nullptr;

	if (FALSE == NETWORK::AcceptEx(IOCP->GetListenSocket(),sock,(LPOVERLAPPED)acceptContext,acceptContext->buf))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(acceptContext);
			LOG_ERROR("Fail to AcceptEx");
			return false;
		}
	}
	return true;
}

bool cClientSession::AcceptCompletion()
{
	bool bResult = true;
	do
	{
		if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)cCompletionPort::Instance()->GetListenSocket(), sizeof(SOCKET)))
		{
			LOG("SO_UPDATE_ACCEPT_CONTEXT error: %d", GetLastError());
			bResult = false;
			break;
		}

		int opt = 1;
		if (SOCKET_ERROR == setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
		{
			LOG("TCP_NODELAY error: %d", GetLastError());
			bResult = false;
			break;
		}

		opt = 0;
		if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
		{
			LOG("SO_RCVBUF change error: %d", GetLastError());
			bResult = false;
			break;
		}

		int addrlen = sizeof(SOCKADDR_IN);
		if (SOCKET_ERROR == getpeername(sock, (SOCKADDR*)&addr, &addrlen))
		{
			LOG("getpeername error: %d", GetLastError());
			bResult = false;
			break;
		}

		HANDLE handle = CreateIoCompletionPort((HANDLE)sock, IOCP->GetHandle(), (ULONG_PTR)this, 0);
		if (handle != cCompletionPort::Instance()->GetHandle())
		{
			LOG("CreateIoCompletionPort error: %d", GetLastError());
			bResult = false;
			break;
		}

	} while (false);


	if (!bResult)
	{
		LOG("[%s] CreateIoCompletionPort error: %d", __FUNCTION__, GetLastError());
		return bResult;
	}

	char clientIP[32] = { 0, };
	inet_ntop(AF_INET, &(addr.sin_addr), clientIP, 32 - 1);
	LOG("Client Connected: IP=%s, PORT=%d", clientIP, ntohs(addr.sin_port));

	if (false == PostRecv())
	{
		LOG_ERROR("Fail to PostRecv");
		return false;
	}

	return true;
}

void cClientSession::OnDisconnect()
{
	char clientIP[32] = { 0, };
	inet_ntop(AF_INET, &(addr.sin_addr), clientIP, 32 - 1);
	LOG("Client Disconnected: IP=%s, PORT=%d", clientIP, ntohs(addr.sin_port));
}

void cClientSession::OnRelease()
{

}