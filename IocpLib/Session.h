#pragma once

#ifndef __SESSION_IOCP
#define __SESSION_IOCP

#include <Ws2tcpip.h>
#include "FastSpinlock.h"
#include "CircularBuffer.h"

typedef class cSession
{
protected:
	SOCKET		sock;
	SOCKADDR_IN addr;
	SPINLOCK	lock_Session;
	CircularBuffer	mRecvBuffer;
	CircularBuffer	mSendBuffer;
	int				mSendPendingCount;

public:
					cSession(size_t recv, size_t send);
	virtual			~cSession();

	virtual void	OnReceive();
	virtual void	OnDisconnect() {}
	virtual void	OnRelease() {}

	bool			PostRecv();
	bool			PostSend(const char* data);
	bool			FlushSend();
	void			SendCompletion(DWORD transferred);
	void			RecvCompletion(DWORD transferred);
	void			DisconnectCompletion();

	void			EchoBack();

	void			Close();
}SESSION, *LPSESSION;
#endif