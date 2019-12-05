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
	Sleep(50);
	char buf[256];
	int  size;
	std::cout << "input num of dummy : ";
	std::cin  >> size;
	if (DUMMYS->CreateDummy(size, "127.0.0.1") == false) {
		return -1;
	}

	while (true) {
		std::cin >> buf;
		DUMMYS->Chat(buf);
	}

	IOCP->Close();
	DUMMYS->Close();

	return 0;
}