#include "pch.h"
#include "OverlappedIOContext.h"
#include "Session.h"
cSession::cSession(size_t recv, size_t send) :  
	sock(INVALID_SOCKET), mRecvBuffer(recv), mSendBuffer(send)
{
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&addr, 0, sizeof(SOCKADDR_IN));
}

cSession::~cSession()
{
	closesocket(sock);
}

void cSession::OnReceive()
{
	EchoBack();
}

bool cSession::PostRecv()
{
	if (mRecvBuffer.IsFull()) {
		return false;
	}

	CONTEXT_RECV* recvContext = NEW(CONTEXT_RECV);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	char* buffer = mRecvBuffer.pop();
	recvContext->buf.len = strlen(buffer);
	recvContext->buf.buf = buffer;

	/// start real recv
	if (SOCKET_ERROR == WSARecv(sock, &recvContext->buf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)recvContext, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(recvContext);
			printf_s("Session::PostRecv Error : %d\n", GetLastError());
			return false;
		}
	}

	return true;
}

bool cSession::PostSend(const char* data)
{
	FastSpinlockGuard criticalSection(lock_Session);


	/// flush later...
	//LSendRequestSessionList->push_back(this);

	mSendBuffer.push(data);

	return true;
}

bool cSession::FlushSend()
{
	FastSpinlockGuard criticalSection(lock_Session);

	/// ���� �����Ͱ� ���� ���
	if(mSendBuffer.IsEmpty())
	{
		/// ���� �����͵� ���� ���
		if (0 == mSendPendingCount) {
			return true;
		}
		return false;
	}

	/// ������ send�� �Ϸ� �ȵ� ���
	if (mSendPendingCount > 0) {
		return false;
	}


	CONTEXT_SEND* sendContext = NEW(CONTEXT_SEND);

	DWORD sendbytes = 0;
	DWORD flags = 0;
	char* buffer = mSendBuffer.pop();
	sendContext->buf.len = strlen(buffer);
	sendContext->buf.buf = buffer;

	/// start async send
	if (SOCKET_ERROR == WSASend(sock, &sendContext->buf, 1, &sendbytes, flags, (LPWSAOVERLAPPED)sendContext, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(sendContext);
			return true;
		}

	}

	mSendPendingCount++;

	return mSendPendingCount == 1;
}

void cSession::SendCompletion(DWORD transferred)
{
	FastSpinlockGuard criticalSection(lock_Session);

	mSendPendingCount--;
}

void cSession::RecvCompletion(DWORD transferred)
{
	OnReceive();
}

void cSession::DisconnectCompletion()
{
	closesocket(sock);
	OnRelease();
}

void cSession::EchoBack()
{
	if (mRecvBuffer.IsEmpty()) {
		return;
	}

	if (false == PostSend(mRecvBuffer.pop())) {
		return;
	}
}

void cSession::Close()
{
	closesocket(sock);
}