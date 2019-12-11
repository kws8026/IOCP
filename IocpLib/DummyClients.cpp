#include "pch.h"
#include "DummyClients.h"
#include "ServerSession.h"
#include "CompletionPort.h"
#include "OverlappedIOContext.h"
#include "NetworkObject.h"

DummyClients::~DummyClients()
{
	for (int i = 0; i < vec_ServerSession.size(); i++)
	{
		delete vec_ServerSession[i];
	}
}

bool DummyClients::CreateDummy(size_t num, const char* serveraddr)
{
	serverAddr = serveraddr;
	CreateIOPool(num * 2);
	NetworkObject::CreatePool(num);
	cPacketManager::Initialize(num * 2);
	for (int i = 0; i < num; i++)
	{
		cServerSession* session = new cServerSession(serverAddr);
		if (!session)
			return false;
		vec_ServerSession.push_back(session);
		if (session->Connect() == false) {
			return false;
		}
		if (session->ConnectCompletion() == false) {
			return false;
		}
	}
	return true;
}

bool DummyClients::Chat(char* buf)
{
	FastSpinlockGuard lock(lock_dummy);
	PACKET_CHAT* chat = NEW(PACKET_CHAT);
	strcpy_s(chat->chat,256,buf);
	char send[MAX_OF_BUFFER*2] = {0,};
	cPacketManager::Serialization(send, chat);
	for (int i = 0; i < vec_ServerSession.size(); i++)
	{
		if (vec_ServerSession[i]->PostSend(send) == false)
			return false;
		if (vec_ServerSession[i]->FlushSend() == false)
			return false;
	}
	cPacketManager::DeletePacket(chat);
	return true;
}

void DummyClients::Close()
{
	for (int i = 0; i < vec_ServerSession.size(); i++)
	{
		vec_ServerSession[i]->Close();
	}
}
