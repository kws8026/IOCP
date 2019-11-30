#include <iostream>
#include "CompletionPort.h"
#pragma comment( lib, "ws2_32.lib")

int main() {
	if (IOCP->Initialize() == false) {
		return -1;
	}
	if (IOCP->StartThreads() == false) {
		return -1;
	}
	IOCP->StartAccept();
	IOCP->Close();

	return 0;
}