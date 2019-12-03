#include "pch.h"
#include "Packet.h"

cPacketManager::cPacketManager()
{
	memset(packetSize,0,256);
	packetSize[ServerLoginCompletion] = sizeof(PACKETS_LOGIN);
	packetSize[ServerObjects] = sizeof(PACKETS_OBJECT);
	packetSize[ServerState] = sizeof(PACKETS_STATE);
	packetSize[ClientLogin] = sizeof(PACKETC_LOGIN);
	packetSize[ClientObjectsCompletion] = sizeof(PACKETC_OBJECT);
	packetSize[ClientRequestState] = sizeof(PACKETC_REQSTATE);
	packetSize[ClientState] = sizeof(PACKETC_STATE);
}

cPacketManager::~cPacketManager()
{
}

bool cPacketManager::Initialize(size_t opSize = 20)
{
	// Packet ObjectPool Create by Size
	if (false == stServerLogin::CreatePool(opSize, true)) {
		return false;
	}
	if (false == stServerObjects::CreatePool(opSize, true)) {
		return false;
	}
	if (false == stServerState::CreatePool(opSize, true)) {
		return false;
	}
	if (false == stClientLogin::CreatePool(opSize, true)) {
		return false;
	}
	if (false == stClientObjects::CreatePool(opSize, true)) {
		return false;
	}
	if (false == stClientRequestState::CreatePool(opSize, true)) {
		return false;
	}
	if (false == stClientState::CreatePool(opSize, true)) {
		return false;
	}
	return true;
}

void cPacketManager::Serialization(char* des,stPacket* pPacket)
{
	if (des == nullptr || pPacket == nullptr)
		return;
	des[0] = pPacket->type;
	des[1] = packetSize[pPacket->type];
	switch (des[0])
	{
	case ServerLoginCompletion:
		PACKETS_LOGIN* pack = static_cast<PACKETS_LOGIN*>(pPacket);
		memcpy(&des[2], pack, des[1]);
		break;
	case ServerObjects:
		PACKETS_OBJECT* pack = static_cast<PACKETS_OBJECT*>(pPacket);
		memcpy(&des[2], pack, des[1]);
		break;
	case ServerState:
		PACKETS_STATE* pack = static_cast<PACKETS_STATE*>(pPacket);
		memcpy(&des[2], pack, des[1]);
		break;
	case ClientLogin:
		PACKETC_LOGIN* pack = static_cast<PACKETC_LOGIN*>(pPacket);
		memcpy(&des[2], pack, des[1]);
		break;
	case ClientObjectsCompletion:
		PACKETC_OBJECT* pack = static_cast<PACKETC_OBJECT*>(pPacket);
		memcpy(&des[2], pack, des[1]);
		break;
	case ClientRequestState:
		PACKETC_REQSTATE* pack = static_cast<PACKETC_REQSTATE*>(pPacket);
		memcpy(&des[2], pack, des[1]);
		break;
	case ClientState:
		PACKETC_STATE* pack = static_cast<PACKETC_STATE*>(pPacket);
		memcpy(&des[2], pack, des[1]);
		break;
	default:
		break;
	}
	des[des[1] + 2] = END_MARK;
	des[des[1] + 3] = '\0';
}

stPacket* cPacketManager::Deserialization(const char* buffer)
{
	stPacket* pPacket = nullptr;
	switch (buffer[0])
	{
	case ServerLoginCompletion:
		pPacket = NEW(PACKETS_LOGIN);
		PACKETS_LOGIN* pack = static_cast<PACKETS_LOGIN*>(pPacket);
		memcpy(pack, &buffer[2], buffer[1]);
		break;
	case ServerObjects:
		pPacket = NEW(PACKETS_OBJECT);
		PACKETS_OBJECT* pack = static_cast<PACKETS_OBJECT*>(pPacket);
		memcpy(pack, &buffer[2], buffer[1]);
		break;
	case ServerState:
		pPacket = NEW(PACKETS_STATE);
		PACKETS_STATE* pack = static_cast<PACKETS_STATE*>(pPacket);
		memcpy(pack, &buffer[2], buffer[1]);
		break;
	case ClientLogin:
		pPacket = NEW(PACKETC_LOGIN);
		PACKETC_LOGIN* pack = static_cast<PACKETC_LOGIN*>(pPacket);
		memcpy(pack, &buffer[2], buffer[1]);
		break;
	case ClientObjectsCompletion:
		pPacket = NEW(PACKETC_OBJECT);
		PACKETC_OBJECT* pack = static_cast<PACKETC_OBJECT*>(pPacket);
		memcpy(pack, &buffer[2], buffer[1]);
		break;
	case ClientRequestState:
		pPacket = NEW(PACKETC_REQSTATE);
		PACKETC_REQSTATE* pack = static_cast<PACKETC_REQSTATE*>(pPacket);
		memcpy(pack, &buffer[2], buffer[1]);
		break;
	case ClientState:
		pPacket = NEW(PACKETC_STATE);
		PACKETC_STATE* pack = static_cast<PACKETC_STATE*>(pPacket);
		memcpy(pack, &buffer[2], buffer[1]);
		break;
	default:
		break;
	}
	return pPacket;
}

void DeletePacket(stPacket* pPacket)
{
	if (nullptr == pPacket) {
		return;
	}
	/// ObjectPool's operate delete dispatch
	switch (pPacket->type)
	{
	case ServerLoginCompletion:
		RELEASE(PACKETS_LOGIN, pPacket);
		break;
	case ServerObjects:
		RELEASE(PACKETS_OBJECT, pPacket);
		break;
	case ServerState:
		RELEASE(PACKETS_STATE, pPacket);
		break;
	case ClientLogin:
		RELEASE(PACKETC_LOGIN, pPacket);
		break;
	case ClientObjectsCompletion:
		RELEASE(PACKETC_OBJECT, pPacket);
		break;
	case ClientRequestState:
		RELEASE(PACKETC_REQSTATE, pPacket);
		break;
	case ClientState:
		RELEASE(PACKETC_STATE, pPacket);
		break;
	default:
		LOG_ERROR("Invalide Packet");
	}
}