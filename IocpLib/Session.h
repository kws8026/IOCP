#pragma once

#ifndef __SESSION_IOCP
#define __SESSION_IOCP

#include <Ws2tcpip.h>
#include "FastSpinlock.h"
#include "CircularBuffer.h"

#define MAX_OF_BUFFER 1024

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
					cSession();
	virtual			~cSession();

	virtual void	OnReceive(size_t len);
	virtual void	OnDisconnect() {}
	virtual void	OnRelease() {}

	bool			PostRecv();
	bool			PostSend(const char* data, size_t len);
	bool			FlushSend();
	void			SendCompletion(DWORD transferred);
	void			RecvCompletion(DWORD transferred);
	void			DisconnectCompletion();

	void			EchoBack();

	void			Close();
}SESSION, *LPSESSION;
#endif