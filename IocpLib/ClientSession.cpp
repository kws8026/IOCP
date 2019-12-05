#include "pch.h"
#include "ClientSession.h"
#include "OverlappedIOContext.h"
#include "ClientSessionManager.h"
#include "CompletionPort.h"
#include <string>

using namespace NETWORK;

#define SIZE_BUFFER_CLIENT 4

cClientSession::cClientSession() : SESSION(SIZE_BUFFER_CLIENT, SIZE_BUFFER_CLIENT)
{
}

cClientSession::~cClientSession()
{
}

void cClientSession::ResetSession()
{
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	LINGER lingerOption;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;

	/// no TCP TIME_WAIT
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof(LINGER)))
	{
		ERROR_CODE(GetLastError(),"setsockopt linger option");
	}

	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool cClientSession::PostAccept()
{
	LPCONTEXT_ACCEPT acceptContext = NEW(CONTEXT_ACCEPT);
	acceptContext->SetSession(this);
	acceptContext->buf.len = 0;
	acceptContext->buf.buf = nullptr;

	if (FALSE == NETWORK::AcceptEx(IOCP->GetListenSocket(),sock,(LPOVERLAPPED)acceptContext,acceptContext->buf))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(acceptContext);
			ERROR_CODE(WSAGetLastError(),"Fail to AcceptEx Error");
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
		SOCKET listen = IOCP->GetListenSocket();
		if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&listen, sizeof(SOCKET)))
		{
			ERROR_CODE(GetLastError(),"SO_UPDATE_ACCEPT_CONTEXT error");
			bResult = false;
			break;
		}

		int opt = 1;
		if (SOCKET_ERROR == setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
		{
			ERROR_CODE(GetLastError(), "TCP_NODELAY error");
			bResult = false;
			break;
		}

		opt = 0;
		if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
		{
			ERROR_CODE(GetLastError(), "SO_RCVBUF change error");
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
			ERROR_CODE(GetLastError(), "CreateIoCompletionPort error");
			bResult = false;
			break;
		}

	} while (false);


	if (!bResult)
	{
		ERROR_CODE(GetLastError(),"CreateIoCompletionPort error");
		return bResult;
	}

	char clientIP[32] = { 0, };
	inet_ntop(AF_INET, &(addr.sin_addr), clientIP, 32 - 1);
	std::string str;

	LOG("Client Connected: IP=%s, PORT=%d", clientIP, ntohs(addr.sin_port));

	if (false == PostRecv())
	{
		LOG_ERROR("Fail to PostRecv");
		return false;
	}

	return true;
}

void cClientSession::EchoPacket(stPacket* packet)
{
	auto buffer = bufSend.Front();
	cPacketManager::Serialization(buffer, packet);
	if (nullptr == buffer) {
		cPacketManager::DeletePacket(packet);
		return;
	}
	bufSend.Commit();
}

void cClientSession::OnReceive()
{
	stPacket* packet = cPacketManager::Deserialization(bufRecv.pop());
	if (packet == nullptr)
		return;
	switch (packet->type) {
	case ServerLoginCompletion: {
		break;
	}
	case ServerObjects: {
		break;
	}
	case ServerState: {
		EchoPacket(packet);
		break;
	}
	case ClientChat: {
		EchoPacket(packet);
		break;
	}
	default:
		break;
	}
	cPacketManager::DeletePacket(packet);
}


void cClientSession::OnSend()
{
}

void cClientSession::OnDisconnect()
{
	char clientIP[32] = { 0, };
	inet_ntop(AF_INET, &(addr.sin_addr), clientIP, 32 - 1);
	LOG("Client Disconnected: IP=%s, PORT=%d", clientIP, ntohs(addr.sin_port));
	CLIENTS->ReturnClientSession(this);
}

void cClientSession::OnRelease()
{

}