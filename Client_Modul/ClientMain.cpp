#include <iostream>
#include "CompletionPort.h"
#include "ServerSession.h"

#pragma comment( lib, "ws2_32.lib")

int main() {
	cServerSession session("192.0.0.1");
	if (IOCP->Initialize() == false) {
		return -1;
	}
	if (IOCP->StartThreads() == false) {
		return -1;
	}
	session.ConnectRequest();
	char flag = 'o';
	while (flag != 'x') {
		flag = getchar();
	}
	IOCP->Close();
	session.Close();

	return 0;
}