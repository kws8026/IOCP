#pragma once

#include <vector>
#include "Packet.h"
#include "Singleton.h"
class cServerSession;

class DummyClients : public cSingleton<DummyClients>
{
	SPINLOCK		lock_dummy;
	cPacketManager	mng_packet;
	const char*		serverAddr;
	std::vector<cServerSession*> vec_ServerSession;
public:
	DummyClients(){};
	~DummyClients();
	bool CreateDummy(size_t num, const char* serveraddr);
	bool Chat(char* buf);
	void Close();
	cPacketManager* GetPacketManager() {return &mng_packet;}
}; 

#define DUMMYS DummyClients::Instance()
#define LPMNGPACKET DUMMYS->GetPacketManager()