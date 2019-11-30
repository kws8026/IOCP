#pragma once
#ifndef __WINSOCK_IOCP
#define __WINSOCK_IOCP
#define PORT 10845

#include <Windows.h>
#include <WinSock2.h>
#include <mswsock.h>

#include "Logger.h"

namespace NETWORK{
	bool	SetWsa();
	void	CleanWsa();

	bool	CreateTCPSocket(SOCKET& sock);
	bool	Bind(SOCKET& sock);
	BOOL	AcceptEx(SOCKET listen, SOCKET other, LPOVERLAPPED overlapped, WSABUF buf);
	BOOL	ConnectEx(SOCKET connect, SOCKADDR_IN addr, LPOVERLAPPED overlapped);
}

#endif