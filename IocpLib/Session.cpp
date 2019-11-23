#include "pch.h"
#include "Session.h"
cSession::cSession() : sock(INVALID_SOCKET)
{
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&addr, 0, sizeof(SOCKADDR_IN));
}

cSession::~cSession()
{
	closesocket(sock);
}

void cSession::OnReceive(size_t len)
{
}

bool cSession::PostRecv()
{
	return false;
}

bool cSession::PostSend()
{
	return false;
}

void cSession::SendCompletion(DWORD transferred)
{
}

void cSession::RecvCompletion(DWORD transferred)
{
}

void cSession::DisconnectCompletion()
{
	OnRelease();
}

void cSession::Close()
{
	closesocket(sock);
}