#include <iostream>
#include "CompletionPort.h"
#include "ServerSession.h"

#pragma comment( lib, "ws2_32.lib")

int main() {
	if (IOCP->Initialize() == false) {
		return -1;
	}
	if (IOCP->StartThreads() == false) {
		return -1;
	}
	cServerSession session("127.0.0.1");
	if (session.ConnectRequest() == false) {
		return -1;
	}
	char buf[256];
	while (true) {
		std::cin >> buf;
		session.PostSend(buf);
	}
	IOCP->Close();
	session.Close();

	return 0;
}