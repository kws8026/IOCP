#include "pch.h"
#include "DummyClients.h"
#include "ServerSession.h"
#include "CompletionPort.h"
#include "OverlappedIOContext.h"


DummyClients::~DummyClients()
{
	for (int i = 0; i < vec_ServerSession.size(); i++)
	{
		delete vec_ServerSession[i];
	}
}

bool DummyClients::CreateDummy(size_t num)
{
	CreateIOPool(num * 10);
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

bool DummyClients::Send(char* buf)
{
	for (int i = 0; i < vec_ServerSession.size(); i++)
	{
		if (vec_ServerSession[i]->PostSend(buf) == false)
			return false;
		if (vec_ServerSession[i]->FlushSend() == false)
			return false;
	}
	return true;
}

void DummyClients::Close()
{
	for (int i = 0; i < vec_ServerSession.size(); i++)
	{
		vec_ServerSession[i]->Close();
	}
}
