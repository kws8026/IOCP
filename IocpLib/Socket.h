#pragma once

#ifndef __SOCKET_IOCP
#define __SOCKET_IOCP

#include <WinSock2.h>
#include <Windows.h>

class cSocket
{
	SOCKET		socket;
	SOCKADDR_IN addr;
};

#endif