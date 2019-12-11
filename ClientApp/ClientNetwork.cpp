#include "ClientNetwork.h"
#include "CompletionPort.h"
#include "OverlappedIOContext.h"
#include "NetworkObject.h"
#include "Packet.h"
#include "ServerSession.h"
#include "Object.h"

bool ClientNetwork::Intialize(const char* serveraddr)
{
	serverAddr = serveraddr;
	if (IOCP->Initialize() == false) {
		return false;
	}
	if (IOCP->StartThreads(3) == false) {
		return false;
	}
	Sleep(50);
	CreateIOPool(100);
	if (NetworkObject::CreatePool(500) == false) {
		return false;
	}
	if (cPacketManager::Initialize(100) == false) {
		return false;
	}
	pSession = new cServerSession(serverAddr);
	if (!pSession)
		return false;
	return true;
}

bool ClientNetwork::Connect()
{
	if (pSession->Connect() == false) {
		return false;
	}
	if (pSession->ConnectCompletion() == false) {
		return false;
	}
	LOG("Connect Successful");
	return true;
}

void ClientNetwork::Login(int id)
{
	PACKETC_LOGIN* login = NEW(PACKETC_LOGIN);
	login->id = id;
	LOG("Send : LOGIN %d",id);
	cPacketManager::Serialization(buf, login);
	pSession->PostSend(buf);
	pSession->FlushSend();
	cPacketManager::DeletePacket(login);
}

void ClientNetwork::RefPlayer(Object* player)
{
	//auto object = static_cast<NetworkObject*>(player);
	//pSession->SetPlayer(object);
}

void ClientNetwork::Move(float x, float y)
{
	PACKETC_STATE* state = NEW(PACKETC_STATE);
	state->x = x;
	state->y = y;
	state->state = 'm';
	//LOG("Send : Move (%f,%f)",x,y);
	cPacketManager::Serialization(buf, state);
	pSession->PostSend(buf);
	pSession->FlushSend();
	cPacketManager::DeletePacket(state);
}

void ClientNetwork::Move(Vector2D pos)
{
	PACKETC_STATE* state = NEW(PACKETC_STATE);
	state->x = pos.x;
	state->y = pos.y;
	state->state = 'm';
	//LOG("Send : Move (%f,%f)",x,y);
	cPacketManager::Serialization(buf, state);
	pSession->PostSend(buf);
	pSession->FlushSend();
	cPacketManager::DeletePacket(state);
}

void ClientNetwork::CheckLogin(bool& flag)
{
	flag = pSession->IsLogin();
}
