#pragma once
#ifndef __SERVERSESSION_IOCP
#define __SERVERSESSION_IOCP

#include "Session.h"
#include <map>

class cServerSession :
	public SESSION
{
	const char*					serverAddr;
	std::map<DWORD,NetworkObject*>	others;
	bool						loginFlag;
	DWORD						NetworkId;
public:
	cServerSession(const char* serverAddr);
	virtual ~cServerSession();

	virtual void	OnSend() {};
	virtual void	OnReceive();

	bool	Connect();

	bool	ConnectRequest();
	bool	ConnectCompletion();
	bool	IsLogin() { return loginFlag; };

	DWORD	GetId() {
		return NetworkId;
	}
	std::map<DWORD, NetworkObject*>& GetOthers() {
		return others;
	}
};
#endif
