#pragma once

#ifndef __SESSION_IOCP
#define __SESSION_IOCP

#include "WinSock.h"
#include "Logger.h"

typedef class cSession
{
	SOCKET		sock;
	SOCKADDR_IN addr;
public:
	cSession();
	~cSession();
	bool		Create(WORD port, bool bBind = false);
	void		Close();
	BOOL		AcceptEX();
	BOOL		ConnectEX();
}SESSION, *LPSESSION;

#endif