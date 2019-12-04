#include "pch.h"
#include "FastSpinlock.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "CompletionPort.h"
#include "OverlappedIOContext.h"

ClientSessionManager::~ClientSessionManager()
{
	for (auto it : mFreeSessionList)
	{
		delete it;
	}
}

void ClientSessionManager::PrepareClientSessions()
{
	m_MaxSessionCount = 1000;
	cClientSession::CreatePool(m_MaxSessionCount);
	cPacketManager::Initialize(m_MaxSessionCount);
	for (int i = 0; i < m_MaxSessionCount; ++i)
	{
		cClientSession* client = NEW(cClientSession);
		client->ResetSession();
		mFreeSessionList.push_back(client);
		m_SessionList.push_back(client);
	}
	CreateIOPool(m_MaxSessionCount);

	LOG("MaxSessionCount: %d", m_MaxSessionCount);
}

void ClientSessionManager::ReturnClientSession(cClientSession* client)
{
	FastSpinlockGuard guard(lock_mngClinet);

	client->ResetSession();

	mFreeSessionList.push_back(client);

	++mCurrentReturnCount;
}

bool ClientSessionManager::AcceptClientSessions()
{
	FastSpinlockGuard guard(lock_mngClinet);

	while (mCurrentIssueCount - mCurrentReturnCount < m_MaxSessionCount)
	{
		cClientSession* newClient = mFreeSessionList.back();
		mFreeSessionList.pop_back();

		++mCurrentIssueCount;

		if (false == newClient->PostAccept()) {
			return false;
		}
	}


	return true;
}

cClientSession* ClientSessionManager::GetClientSession(const int index)
{
	if (index < 0 || index >= m_MaxSessionCount) {
		return nullptr;
	}

	return m_SessionList[index];
}