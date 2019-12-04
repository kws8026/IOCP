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
	ClientList		mFreeSessionList;
	std::vector<cClientSession*> m_SessionList;

	cPacketManager	mng_packet;

	cFastSpinlock	lock_mngClinet;

	uint64_t		mCurrentIssueCount;
	uint64_t		mCurrentReturnCount;
	int				m_MaxSessionCount;

public:
	ClientSessionManager() : mCurrentIssueCount(0), mCurrentReturnCount(0), m_MaxSessionCount(0) {};

	~ClientSessionManager();

	void PrepareClientSessions();
	bool AcceptClientSessions();

	void ReturnClientSession(cClientSession* client);

	int MaxClientSessionCount() { return m_MaxSessionCount; }
	int GetClientsConnectedSize() { return m_SessionList.size(); }
	cClientSession* GetClientSession(const int index);
	cPacketManager* GetPacketManager() { return &mng_packet; }
};

#define CLIENTS ClientSessionManager::Instance()
#define LPMNGPACKET CLIENTS->GetPacketManager()