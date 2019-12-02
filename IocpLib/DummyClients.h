#pragma once

#include <vector>

class cServerSession;

class DummyClients
{
	const char* serverAddr;
	std::vector<cServerSession*> vec_ServerSession;
public:
	DummyClients(const char* addr):serverAddr(addr){};
	~DummyClients();
	bool CreateDummy(size_t num);
	bool Send(char* buf);
	void Close();
};