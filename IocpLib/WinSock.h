#pragma once
#ifndef __WINSOCK_IOCP
#define __WINSOCK_IOCP

#include <WinSock2.h>
#include <mswsock.h>
#include <Windows.h>

#include "Logger.h"

namespace NETWORK{
	static LPFN_ACCEPTEX		pAcceptEx	= NULL;
	static LPFN_CONNECTEX		pConnectEx	= NULL;
	static DWORD				ReturnByte	= NULL;
	static const unsigned int	PORT		= 10845;

	bool	SetWsa() {
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		{
			LOG_ERROR("WSAStartup failed.");
			return false;	
		}
		return true;
	};
	bool	CleanWsa() {
		WSACleanup();
	};

	bool	CreateTCPSocket(SOCKET& sock) {
		sock = WSASocket(
			AF_INET,
			SOCK_STREAM,
			IPPROTO_TCP,
			NULL, 0, 
			WSA_FLAG_OVERLAPPED);
		if (sock == INVALID_SOCKET) {
			LOG_ERROR("Fail to Create TCP Socket");
			return false;
		}
		return true;
	}
	bool	Bind(SOCKET& sock) {
		int opt = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));

		SOCKADDR_IN serveraddr;
		ZeroMemory(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = htons(PORT);
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

		if (SOCKET_ERROR == bind(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)))
		{
			LOG_ERROR("Fail to Bind");
			return false;
		}
		return true;
	}
	BOOL	AcceptEx(SOCKET listen, SOCKET other, LPOVERLAPPED overlapped, WSABUF buf) {
		if (pAcceptEx == NULL) {
			GUID guidAcceptEx = WSAID_ACCEPTEX;
			if (SOCKET_ERROR == WSAIoctl(listen, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guidAcceptEx, sizeof(GUID), &pAcceptEx, sizeof(LPFN_ACCEPTEX), &ReturnByte, NULL, NULL)) {
				return false;
			}
		}
		return pAcceptEx(listen, other, &(buf.buf), buf.len, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &ReturnByte, overlapped);
	};
	BOOL	ConnectEx(SOCKET connect, SOCKADDR_IN addr, LPOVERLAPPED overlapped) {
		if (pConnectEx == NULL) {
			GUID guidConnectEx = WSAID_CONNECTEX;
			if (SOCKET_ERROR == WSAIoctl(connect, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guidConnectEx, sizeof(GUID), &pConnectEx, sizeof(LPFN_CONNECTEX), &ReturnByte, NULL, NULL)) {
				return false;
			}
		}
		return  pConnectEx(connect, (sockaddr*)&(addr), sizeof(addr), NULL, 0, &ReturnByte, overlapped);
	};
}

#endif