#pragma once
#ifndef __CLIENTSESSION_IOCP
#define __CLIENTSESSION_IOCP

#include "Session.h"
#include "Objectpool.h"

class ClientSessionManager;

class cClientSession : public SESSION, public cObjectPool<cClientSession>
{
public:
				cClientSession();
	virtual		~cClientSession();

	void		ResetSession();

	bool		PostAccept();
	bool		AcceptCompletion();
	void		EchoPacket(stPacket* packet);

	virtual void OnReceive();
	virtual void OnSend();
	virtual void OnDisconnect();
	virtual void OnRelease();
	
	friend class ClientSessionManager;
};

#endif 

