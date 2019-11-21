#include "pch.h"
#include "Session.h"
cSession::cSession() : sock(INVALID_SOCKET),addr()
{
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

cSession::~cSession()
{
}

void cSession::Close()
{
	closesocket(sock);
}