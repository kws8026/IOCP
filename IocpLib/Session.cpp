#include "pch.h"
#include "OverlappedIOContext.h"
#include "Session.h"
cSession::cSession() :  sock(INVALID_SOCKET)
{
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&addr, 0, sizeof(SOCKADDR_IN));
}

cSession::~cSession()
{
	closesocket(sock);
}

bool cSession::SetBuffer(size_t sendBufSize, size_t recvBufSize)
{
	mSendBuffer
	mRecvBuffer
	return false;
}

void cSession::OnReceive(size_t len)
{
	EchoBack();
}

bool cSession::PostRecv()
{
	if (0 == mRecvBuffer.GetFreeSpaceSize()) {
		return false;
	}

	CONTEXT_RECV* recvContext = NEW(CONTEXT_RECV);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	recvContext->buf.len = (ULONG)mRecvBuffer.GetFreeSpaceSize();
	recvContext->buf.buf = mRecvBuffer.GetBuffer();


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

bool cSession::PostSend(const char* data, size_t len)
{
	FastSpinlockGuard criticalSection(lock_Session);

	if (mSendBuffer.GetFreeSpaceSize() < len) {
		return false;
	}

	/// flush later...
	//LSendRequestSessionList->push_back(this);

	char* destData = mSendBuffer.GetBuffer();

	memcpy(destData, data, len);

	mSendBuffer.Commit(len);

	return true;
}

bool cSession::FlushSend()
{
	FastSpinlockGuard criticalSection(lock_Session);

	/// 보낼 데이터가 없는 경우
	if (0 == mSendBuffer.GetContiguiousBytes())
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

	DWORD sendbytes = 0;
	DWORD flags = 0;
	sendContext->buf.len = (ULONG)mSendBuffer.GetContiguiousBytes();
	sendContext->buf.buf = mSendBuffer.GetBufferStart();

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

	mSendBuffer.Remove(transferred);

	mSendPendingCount--;
}

void cSession::RecvCompletion(DWORD transferred)
{
	mRecvBuffer.Commit(transferred);

	OnReceive(transferred);
}

void cSession::DisconnectCompletion()
{
	closesocket(sock);
	OnRelease();
}

void cSession::EchoBack()
{
	size_t len = mRecvBuffer.GetContiguiousBytes();

	if (len == 0) {
		return;
	}

	if (false == PostSend(mRecvBuffer.GetBufferStart(), len)) {
		return;
	}

	mRecvBuffer.Remove(len);
}

void cSession::Close()
{
	closesocket(sock);
}