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
	if (session.Connect() == false) {
		return -1;
	}
	session.ConnectCompletion();
	char buf[256];
	while (true) {
		std::cin >> buf;
		session.PostSend(buf);
		session.FlushSend();
	}
	IOCP->Close();
	session.Close();

	return 0;
}