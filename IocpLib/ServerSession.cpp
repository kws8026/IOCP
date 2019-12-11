#include "pch.h"
#include "DummyClients.h"
#include "ServerSession.h"
#include "OverlappedIOContext.h"
#include "NetworkObject.h"
#include "Packet.h"
using namespace NETWORK;

#define SIZE_BUFFER_SERVER 1024

cServerSession::cServerSession(const char* serverAddr) : loginFlag(false),
	serverAddr(serverAddr),SESSION(SIZE_BUFFER_SERVER, SIZE_BUFFER_SERVER)
{
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

cServerSession::~cServerSession()
{
}

void cServerSession::OnReceive()
{
	if (bufRecv.IsEmpty())
		return;
	stPacket* packet = cPacketManager::Deserialization(bufRecv.pop());
	if (packet == nullptr)
		return;
	if (!loginFlag ) {
		if (packet->type == ServerLoginCompletion){
			auto pack = static_cast<PACKETS_LOGIN*>(packet);
			NetworkId = pack->id_Object;
			LOG("your NetworkId : %d", pack->id_Object);
			if (pack->bResult == true)
				loginFlag = true;
		}
	}
	else {
		switch (packet->type) {
		case ServerObjects: {
			break;
		}
		case ServerState: {
			auto pack = static_cast<PACKETS_STATE*>(packet);
			//LOG("RECV : %d Move (%f,%f)", pack->id_Object, pack->x, pack->y);
			auto iter = others.find(pack->id_Object);
			if (iter != others.end()) {
				auto object = iter->second;
				object->SetPos(pack->x, pack->y);
			}
			else {
				auto newObject = NEW(NetworkObject);
				others[pack->id_Object] = newObject;
				LOG("connect new player : %d", pack->id_Object);
			}
			//others.find(pack->id_Object)->second->SetPos(pack->x, pack->y);
			break;
		}
		case ClientChat: {
			LOG("RECV : %s", static_cast<PACKET_CHAT*>(packet)->chat);
			break;
		}
		default:
			break;
		}
	}
	cPacketManager::DeletePacket(packet);
}

bool cServerSession::Connect()
{
	auto ret = inet_pton(AF_INET, serverAddr, (void*)&addr.sin_addr.s_addr);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	if(INVALID_SOCKET == connect(sock, (SOCKADDR*)&addr, sizeof(addr))){
		return false;
	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)sock, IOCP->GetHandle(), (ULONG_PTR)this, 0);
	if (handle != IOCP->GetHandle())
	{
		return false;
	}
	return true;
}

bool cServerSession::ConnectRequest()
{

	auto ret = inet_pton(AF_INET, serverAddr, (void*)&addr.sin_addr.s_addr);
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;
	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(int));
	if (SOCKET_ERROR ==bind(sock,(SOCKADDR*) &addr, sizeof(addr)))
	{
		ERROR_CODE(WSAGetLastError(), "Fail to Bind");
		return false;
	}

	HANDLE handle = CreateIoCompletionPort((HANDLE)sock, IOCP->GetHandle(), (ULONG_PTR)this, 0);
	if (handle != IOCP->GetHandle())
	{
		return false;
	}


	LPCONTEXT_CON context = NEW(CONTEXT_CON);
	context->SetSession(this);

	if (FALSE == ConnectEx(sock,addr,(LPWSAOVERLAPPED)context))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			DeleteIoContext(context);
			ERROR_CODE(WSAGetLastError(),"Fail to ConnectEx");
		}
	}
	return true;
}

bool cServerSession::ConnectCompletion()
{
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0))
	{
		return false;
	}

	int opt = 1;
	if (SOCKET_ERROR == setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
	{
		return false;
	}

	opt = 0;
	if (SOCKET_ERROR == setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
	{
		return false;
	}

	if (false == PostRecv())
	{
		return false;
	}

	return true;
}