#include "pch.h"
#include "Session.h"

cSession::cSession() : sock(INVALID_SOCKET),addr()
{
}

cSession::~cSession()
{
}

bool cSession::Create(WORD port, bool bBind)
{
	return true;
}

void cSession::Close()
{
	closesocket(sock);
}

BOOL cSession::AcceptEX()
{
	return 0;
}

BOOL cSession::ConnectEX()
{
	return 0;
}
