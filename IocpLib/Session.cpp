#include "pch.h"
#include "OverlappedIOContext.h"
#include "Session.h"
#include "Packet.h"

cSession::cSession(size_t recv, size_t send) :  
	sock(INVALID_SOCKET), bufRecv(recv), bufSend(send)
{
	memset(&addr, 0, sizeof(SOCKADDR_IN));
}

cSession::~cSession()
{
	closesocket(sock);
}

bool cSession::PostRecv()
{
	FastSpinlockGuard criticalSection(lock_recv);

	if (bufRecv.IsFull()) {
		return false;
	}

	CONTEXT_RECV* recvContext = NEW(CONTEXT_RECV);
	recvContext->SetSession(this);

	DWORD recvbytes = 0;
	DWORD flags = 0;
	char* buffer = bufRecv.Front();
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

	bufSend.push(data);

	return true;
}

bool cSession::FlushSend()
{
	FastSpinlockGuard criticalSection(lock_Session);

	/// 보낼 데이터가 없는 경우
	if(bufSend.IsEmpty())
	{
		/// 보낼 데이터도 없는 경우
		if (0 == countSendpadding) {
			return true;
		}
		return false;
	}

	/// 이전의 send가 완료 안된 경우
	if (countSendpadding > 0) {
		return false;
	}


	CONTEXT_SEND* sendContext = NEW(CONTEXT_SEND);
	sendContext->SetSession(this);

	DWORD sendbytes = 0;
	DWORD flags = 0;
	char* buffer = bufSend.pop();
	LPWORD len = (LPWORD)&buffer[1];
	sendContext->buf.len = *len;
	sendContext->buf.buf = buffer;
	LOG("SEND : %s", buffer+3);

	if (SOCKET_ERROR == WSASend(sock, &sendContext->buf, 1, &sendbytes, flags, (LPWSAOVERLAPPED)sendContext, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			ERROR_CODE(WSAGetLastError(),"Fail to Send");
			DeleteIoContext(sendContext);
			return true;
		}
	}

	countSendpadding++;
	sendbytes;
	return countSendpadding == 1;
}

void cSession::SendCompletion()
{
	FastSpinlockGuard criticalSection(lock_Session);

	countSendpadding--;
}

void cSession::RecvCompletion()
{
	FastSpinlockGuard criticalSection(lock_recv);
	bufRecv.Commit();
	OnReceive();
}

void cSession::DisconnectCompletion()
{
	closesocket(sock);
	OnDisconnect();
}

void cSession::Close()
{
	closesocket(sock);
}