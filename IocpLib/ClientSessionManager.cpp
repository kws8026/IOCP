#include "pch.h"
#include "FastSpinlock.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "CompletionPort.h"
#include "OverlappedIOContext.h"

ClientSessionManager::~ClientSessionManager()
{
	for (auto it : list_freeSession)
	{
		delete it;
	}
}

void ClientSessionManager::PrepareClientSessions()
{
	countMaxSession = 1000;
	cClientSession::CreatePool(countMaxSession);
	cPacketManager::Initialize(countMaxSession*500);
	for (int i = 0; i < countMaxSession; ++i)
	{
		cClientSession* client = NEW(cClientSession);
		client->ResetSession();
		list_freeSession.push_back(client);
	}
	CreateIOPool(countMaxSession*500);

	LOG("MaxSessionCount: %d", countMaxSession);
}

void ClientSessionManager::AcceptCompletion(cClientSession* client)
{
	sessions.push_back(client);
}

void ClientSessionManager::ReturnClientSession(cClientSession* client)
{
	FastSpinlockGuard guard(lock_mngClinet);

	client->ResetSession();

	list_freeSession.push_back(client);

	++mCurrentReturnCount;
}

bool ClientSessionManager::AcceptClientSessions()
{
	FastSpinlockGuard guard(lock_mngClinet);

	while (mCurrentIssueCount - mCurrentReturnCount < countMaxSession)
	{
		cClientSession* newClient = list_freeSession.back();
		list_freeSession.pop_back();

		++mCurrentIssueCount;

		if (false == newClient->PostAccept()) {
			return false;
		}
	}


	return true;
}

cClientSession* ClientSessionManager::GetClientSession(const int index)
{
	if (index < 0 || index >= countMaxSession) {
		return nullptr;
	}

	return sessions[index];
}