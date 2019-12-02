#include "DummyClients.h"
#include "CompletionPort.h"
#include <iostream>
#pragma comment( lib, "ws2_32.lib")

int main() {
	if (IOCP->Initialize() == false) {
		return -1;
	}
	if (IOCP->StartThreads(6) == false) {
		return -1;
	}
	DummyClients clients("127.0.0.1");

	char buf[256];
	int  size;
	std::cout << "input num of dummy : ";
	std::cin  >> size;
	if (clients.CreateDummy(size) == false) {
		return -1;
	}

	while (true) {
		std::cin >> buf;
		clients.Send(buf);
	}

	IOCP->Close();
	clients.Close();

	return 0;
}