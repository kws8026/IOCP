#include <iostream>
#include "CompletionPort.h"
#include "ClientSessionManager.h"
#pragma comment( lib, "ws2_32.lib")

int main() {
	if (IOCP->Initialize() == false) {
		return -1;
	}
	if (IOCP->StartThreads() == false) {
		return -1;
	}
	CLIENTS->PrepareClientSessions();

	IOCP->StartAccept();
	IOCP->Close();

	return 0;
}