#pragma once

#ifndef __SESSION_IOCP
#define __SESSION_IOCP

#include "WinSock.h"

typedef class cSession
{
	SOCKET		sock;
	SOCKADDR_IN addr;
public:
				cSession();
	virtual		~cSession();

	virtual void OnReceive(size_t len);
	virtual void OnDisconnect() {}
	virtual void OnRelease() {}

	void		Close();



}SESSION, *LPSESSION;

#endif