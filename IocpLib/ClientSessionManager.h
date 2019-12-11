#pragma once
#include <list>
#include <vector>
#include <WinSock2.h>

#include "FastSpinlock.h"
#include "Singleton.h"
#include "Packet.h"

class cClientSession; 

class ClientSessionManager : public cSingleton<ClientSessionManager>
{
	typedef std::list<cClientSession*> ClientList;
	ClientList		list_freeSession;
	std::vector<cClientSession*> sessions;
	cPacketManager	mng_packet;
	SPINLOCK		lock_mngClinet;
	uint64_t		mCurrentIssueCount;
	uint64_t		mCurrentReturnCount;
	int				countMaxSession;

public:
	ClientSessionManager() : mCurrentIssueCount(0), mCurrentReturnCount(0), countMaxSession(0) {};

	~ClientSessionManager();

	void PrepareClientSessions();
	bool AcceptClientSessions();

	void AcceptCompletion(cClientSession* client);
	void ReturnClientSession(cClientSession* client);

	int GetClientsConnectedSize() { return sessions.size(); }
	cClientSession* GetClientSession(const int index);
	cPacketManager* GetPacketManager() { return &mng_packet; }
};

#define CLIENTS ClientSessionManager::Instance()
#define LPMNGPACKET CLIENTS->GetPacketManager()