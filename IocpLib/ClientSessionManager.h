#pragma once
#include <list>
#include <vector>
#include <WinSock2.h>

#include "FastSpinlock.h"
#include "Singleton.h"

class cClientSession;

class ClientSessionManager : public cSingleton<ClientSessionManager>
{
public:
	ClientSessionManager() : mCurrentIssueCount(0), mCurrentReturnCount(0)
	{}

	~ClientSessionManager();

	void PrepareClientSessions();
	bool AcceptClientSessions();

	void ReturnClientSession(cClientSession* client);


	int MaxClientSessionCount() { return m_MaxSessionCount; }

	cClientSession* GetClientSession(const int index);


private:
	typedef std::list<cClientSession*> ClientList;
	ClientList	mFreeSessionList;

	cFastSpinlock mLock;

	uint64_t mCurrentIssueCount;
	uint64_t mCurrentReturnCount;

	int m_MaxSessionCount = 0;

	std::vector<cClientSession*> m_SessionList;
};

#define CLIENTS ClientSessionManager::Instance()