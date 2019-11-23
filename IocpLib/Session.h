#pragma once

#ifndef __SESSION_IOCP
#define __SESSION_IOCP

#include <Ws2tcpip.h>
#include "WinSock.h"
#include "FastSpinlock.h"
#include "OverlappedIOContext.h"

#define MAX_BUFFER 1024

typedef class cSession
{
protected:
	SOCKET		sock;
	SOCKADDR_IN addr;
	SPINLOCK	lock;
	char		buf[MAX_BUFFER];
public:
					cSession();
	virtual			~cSession();

	virtual void	OnReceive(size_t len);
	virtual void	OnDisconnect() {}
	virtual void	OnRelease() {}

	bool			PostRecv();
	bool			PostSend();

	void			SendCompletion(DWORD transferred);
	void			RecvCompletion(DWORD transferred);
	void			DisconnectCompletion();


	void			Close();
}SESSION, *LPSESSION;


#endif