#include "pch.h"
#include "OverlappedIOContext.h"
#include "Session.h"
cSession::cSession(size_t recv, size_t send) :  
	sock(INVALID_SOCKET), mRecvBuffer(recv), mSendBuffer(send)
{
	memset(&addr, 0, sizeof(SOCKADDR_IN));
}

cSession::~cSession()
{
	closesocket(sock);
}

bool cSession::PostRecv()
{
	if (mRecvBuffer.IsFull()) {
		return false;
	}

	CONTEXT_RECV* recvContext = NEW(CONTEXT_RECV);
	recvContext->SetSession(this);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	char* buffer = mRecvBuffer.Front();
	recvContext->buf.len = MAX_OF_BUFFER;
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

	/// 보낼 데이터가 없는 경우
	if(mSendBuffer.IsEmpty())
	{
		/// 보낼 데이터도 없는 경우
		if (0 == mSendPendingCount) {
			return true;
		}
		return false;
	}

	/// 이전의 send가 완료 안된 경우
	if (mSendPendingCount > 0) {
		return false;
	}


	CONTEXT_SEND* sendContext = NEW(CONTEXT_SEND);
	sendContext->SetSession(this);

	DWORD sendbytes = 0;
	DWORD flags = 0;
	char* buffer = mSendBuffer.pop();
	sendContext->buf.len = strlen(buffer);
	sendContext->buf.buf = buffer;
	LOG("SEND : %s", buffer);

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

void cSession::SendCompletion()
{
	FastSpinlockGuard criticalSection(lock_Session);

	mSendPendingCount--;
}

void cSession::RecvCompletion()
{
	OnReceive();
}

void cSession::DisconnectCompletion()
{
	closesocket(sock);
	OnRelease();
}

void cSession::Close()
{
	closesocket(sock);
}