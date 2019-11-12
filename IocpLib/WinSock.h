#pragma once
#ifndef __WINSOCK_UTIL
#define __WINSOCK_UTIL

#include <WinSock2.h>
#include <mswsock.h>
#include <Windows.h>

#include "Logger.h"

namespace NETWORK{
	static LPFN_ACCEPTEX pAcceptEx = NULL;
	static LPFN_CONNECTEX pConnectEx = NULL;

	bool SetWsa() {
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			ERROR_MSG("WSAStartup failed.");
			return false;	
		}
		return true;
	};
	bool CleanWsa() {
		WSACleanup();
	};

	bool LoadAcceptEx(SOCKET listen, SOCKET other, LPOVERLAPPED overlapped, WSABUF buf) {
		if (pAcceptEx == NULL) {
			GUID guidAcceptEx = WSAID_ACCEPTEX;
			if (SOCKET_ERROR == WSAIoctl(listen, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guidAcceptEx, sizeof(GUID), &pAcceptEx, sizeof(LPFN_ACCEPTEX), NULL, NULL, NULL)) {
				return false;
			}
		}
		return pAcceptEx(listen, other, &(buf.buf), buf.len, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, NULL, overlapped);
	};
	bool LoadConnectEx(SOCKET connect, SOCKADDR_IN addr, LPOVERLAPPED overlapped) {
		if (pConnectEx == NULL) {
			GUID guidConnectEx = WSAID_CONNECTEX;
			if (SOCKET_ERROR == WSAIoctl(connect, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guidConnectEx, sizeof(GUID), &pConnectEx, sizeof(LPFN_CONNECTEX), NULL, NULL, NULL)) {
				return false;
			}
		}
		return  pConnectEx(connect, (sockaddr*)&(addr), sizeof(addr), NULL, 0, NULL, overlapped);
	};
}

#endif