#pragma once
#ifndef __CLIENTSESSION_IOCP
#define __CLIENTSESSION_IOCP

#include "Session.h"
#include "Objectpool.h"

class ClientSessionManager;

class cClientSession : public SESSION, public cObjectPool<cClientSession>
{
	SPINLOCK		lock_client;
public:
				cClientSession();
	virtual		~cClientSession();

	void		ResetSession();

	bool		PostAccept();
	bool		AcceptCompletion();

	virtual void OnReceive();
	virtual void OnSend();
	virtual void OnDisconnect();
	virtual void OnRelease();
	
	friend class ClientSessionManager;
};

#endif 

