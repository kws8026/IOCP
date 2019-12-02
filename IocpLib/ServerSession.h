#pragma once
#ifndef __SERVERSESSION_IOCP
#define __SERVERSESSION_IOCP

#include "Session.h"

class cServerSession :
	public SESSION
{
	const char* serverAddr;
public:
	cServerSession(const char* serverAddr);
	virtual ~cServerSession();

	virtual void OnReceive() {};
	virtual void OnSend() {};

	bool Connect();

	bool ConnectRequest();
	bool ConnectCompletion();
};
#endif
