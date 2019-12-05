#include "pch.h"
#include "Packet.h"
#include "Logger.h"

size_t cPacketManager::packetSize[256] = {0,};

cPacketManager::cPacketManager()
{
}

cPacketManager::~cPacketManager()
{
}

bool cPacketManager::Initialize(size_t opSize = 20)
{
	memset(packetSize, 0, 256);
	packetSize[ServerLoginCompletion] = sizeof(PACKETS_LOGIN) + 1;
	packetSize[ServerObjects] = sizeof(PACKETS_OBJECT) + 1;
	packetSize[ServerState] = sizeof(PACKETS_STATE) + 1;
	packetSize[ClientLogin] = sizeof(PACKETC_LOGIN) + 1;
	packetSize[ClientObjectsCompletion] = sizeof(PACKETC_OBJECT) + 1;
	packetSize[ClientRequestState] = sizeof(PACKETC_REQSTATE) + 1;
	packetSize[ClientState] = sizeof(PACKETC_STATE) + 1;
	packetSize[ClientChat] = sizeof(PACKET_CHAT) + 1;
	// Packet ObjectPool Create by Size
	if (false == PACKETS_LOGIN::CreatePool(opSize, true)) {
		return false;
	}
	if (false == PACKETS_OBJECT::CreatePool(opSize, true)) {
		return false;
	}
	if (false == PACKETS_STATE::CreatePool(opSize, true)) {
		return false;
	}
	if (false == PACKETC_LOGIN::CreatePool(opSize, true)) {
		return false;
	}
	if (false == PACKETC_OBJECT::CreatePool(opSize, true)) {
		return false;
	}
	if (false == PACKETC_REQSTATE::CreatePool(opSize, true)) {
		return false;
	}
	if (false == PACKETC_STATE::CreatePool(opSize, true)) {
		return false;
	}
	if (false == PACKET_CHAT::CreatePool(opSize, true)) {
		return false;
	}
	return true;
}

void cPacketManager::Serialization(char* des,stPacket* pPacket)
{
	if (des == nullptr || pPacket == nullptr)
		return; 
	memset(des,0,MAX_OF_BUFFER);
	if(pPacket->size == 0)
		pPacket->size = packetSize[pPacket->type];
	WORD sizePacket = pPacket->size -1;
	WORD len = 0;
	switch (pPacket->type)
	{
	case ServerLoginCompletion: {
		PACKETS_LOGIN* pack = static_cast<PACKETS_LOGIN*>(pPacket);
		memcpy(des, pack, sizePacket);
		break;
	}
	case ServerObjects: {
		PACKETS_OBJECT* pack = static_cast<PACKETS_OBJECT*>(pPacket);
		memcpy(des, pack, sizePacket);
		break;
	}
	case ServerState: {
		PACKETS_STATE* pack = static_cast<PACKETS_STATE*>(pPacket);
		memcpy(des, pack, sizePacket);
		break;
	}
	case ClientLogin:{
		PACKETC_LOGIN* pack = static_cast<PACKETC_LOGIN*>(pPacket);
	memcpy(des, pack, sizePacket);
	break; 
	}
	case ClientObjectsCompletion: {
		PACKETC_OBJECT* pack = static_cast<PACKETC_OBJECT*>(pPacket);
		memcpy(des, pack, sizePacket);
		break;
	}
	case ClientRequestState: {
		PACKETC_REQSTATE* pack = static_cast<PACKETC_REQSTATE*>(pPacket);
		memcpy(des, pack, sizePacket);
		break;
	}
	case ClientState:{
		PACKETC_STATE* pack = static_cast<PACKETC_STATE*>(pPacket);
		memcpy(des, pack, sizePacket);
		break;
	}
	case ClientChat: {
		PACKET_CHAT* pack = static_cast<PACKET_CHAT*>(pPacket);
		len = strlen(pack->chat);
		pPacket->size = 4 + len;
		if (sizePacket < pPacket->size)
			sizePacket = pPacket->size;
		memcpy(des, pack, sizePacket);
		break;
	}
	default:
		break;
	}
	des[pPacket->size-1] = END_MARK;
}

stPacket* cPacketManager::Deserialization(const char* buffer)
{
	stPacket* pPacket = nullptr;
	WORD	size;
	memcpy(&size,&buffer[1],2);
	if (buffer[size-1] != END_MARK) {
		DeletePacket(pPacket);
		LOG_ERROR("Invalid Endmark");
		return nullptr;
	}
	switch (buffer[0]){
	case ServerLoginCompletion: {
		pPacket = NEW(PACKETS_LOGIN);
		PACKETS_LOGIN* pack = static_cast<PACKETS_LOGIN*>(pPacket);
		memcpy(pack,buffer, size);
		break;
	}
	case ServerObjects: {
		pPacket = NEW(PACKETS_OBJECT);
		PACKETS_OBJECT* pack = static_cast<PACKETS_OBJECT*>(pPacket);
		memcpy(pack,buffer, size);
		break;
	}
	case ServerState: {
		pPacket = NEW(PACKETS_STATE);
		PACKETS_STATE* pack = static_cast<PACKETS_STATE*>(pPacket);
		memcpy(pack,buffer, size);
		break;
	}
	case ClientLogin: {
		pPacket = NEW(PACKETC_LOGIN);
		PACKETC_LOGIN* pack = static_cast<PACKETC_LOGIN*>(pPacket);
		memcpy(pack,buffer, size);
		break;
	}
	case ClientObjectsCompletion: {
		pPacket = NEW(PACKETC_OBJECT);
		PACKETC_OBJECT* pack = static_cast<PACKETC_OBJECT*>(pPacket);
		memcpy(pack,buffer, size);
		break;
	}
	case ClientRequestState: {
		pPacket = NEW(PACKETC_REQSTATE);
		PACKETC_REQSTATE* pack = static_cast<PACKETC_REQSTATE*>(pPacket);
		memcpy(pack,buffer, size);
		break;
	}
	case ClientState:{
		pPacket = NEW(PACKETC_STATE);
		PACKETC_STATE* pack = static_cast<PACKETC_STATE*>(pPacket);
		memcpy(pack,buffer, size);
		break;
	}
	case ClientChat: {
		pPacket = NEW(PACKET_CHAT);
		PACKET_CHAT* pack = static_cast<PACKET_CHAT*>(pPacket);
		memcpy(pack,buffer, size);
		break;
	}
	default:
		break;
	}
	return pPacket;
}

void cPacketManager::DeletePacket(stPacket* pPacket)
{
	if (nullptr == pPacket) {
		return;
	}
	switch (pPacket->type)
	{
	case ServerLoginCompletion: {
		PACKETS_LOGIN* pack = static_cast<PACKETS_LOGIN*>(pPacket);
		pack->bResult = false;
		RELEASE(PACKETS_LOGIN, pPacket);
		break;
	}
	case ServerObjects: {
		PACKETS_OBJECT* pack = static_cast<PACKETS_OBJECT*>(pPacket);
		RELEASE(PACKETS_OBJECT, pPacket);
		break;
	}
	case ServerState: {
		PACKETS_STATE* pack = static_cast<PACKETS_STATE*>(pPacket);
		RELEASE(PACKETS_STATE, pPacket);
		break;
	}
	case ClientLogin: {
		PACKETC_LOGIN* pack = static_cast<PACKETC_LOGIN*>(pPacket);
		RELEASE(PACKETC_LOGIN, pPacket);
		break;
	}
	case ClientObjectsCompletion:{
		PACKETC_OBJECT* pack = static_cast<PACKETC_OBJECT*>(pPacket);
		RELEASE(PACKETC_OBJECT, pPacket);
		break;
	}
	case ClientRequestState:{
		PACKETC_REQSTATE* pack = static_cast<PACKETC_REQSTATE*>(pPacket);
		RELEASE(PACKETC_REQSTATE, pPacket);
		break;
	}
	case ClientState:{
		PACKETC_STATE* pack = static_cast<PACKETC_STATE*>(pPacket);
		RELEASE(PACKETC_STATE, pPacket); 
		break;
	}
	case ClientChat:{
		PACKET_CHAT* pack = static_cast<PACKET_CHAT*>(pPacket);
		memset(pack->chat,0,256);
		RELEASE(PACKET_CHAT, pPacket);
		break;
	}
	default:
		LOG_ERROR("Invalide Packet");
	}
}