#pragma once
#ifndef __PACKET_IOCP
#define __PACKET_IOCP
#include <Windows.h>
#include "Objectpool.h"
#include "FastSpinlock.h"

#define MAX_OF_OBJECTS	256
#define END_MARK		'#'

enum HeaderType {
	ServerDefault = 0,
	ServerLoginCompletion = 37,		// 로그인결과
	ServerObjects,					// 그리드안의 오브젝트 정보
	ServerState,					// 오브젝트의 상태 정보
	ClientLogin = 117,
	ClientObjectsCompletion,
	ClientRequestState,
	ClientState,
	ClientChat
};
#pragma pack(push, 1)
typedef struct stPacket {
	unsigned char	type;
	WORD			size;
	stPacket(char type = 0, WORD size = 0) : type(type), size(size){};
}HEAD, *LPHEAD;

typedef struct stServerLogin : public stPacket, cObjectPool<stServerLogin> {
	bool bResult;
	stServerLogin() :bResult(false),stPacket(ServerLoginCompletion) {};
}PACKETS_LOGIN;
typedef struct stServerObjects : public stPacket, cObjectPool<stServerObjects> {
	WORD Object[MAX_OF_OBJECTS];
	stServerObjects() :stPacket(ServerObjects) {};
}PACKETS_OBJECT;
typedef struct stServerState : public stPacket, cObjectPool<stServerState> {
	WORD state;
	WORD id_Object;
	float	x;
	float	y;
	stServerState() :stPacket(ServerState) {};
}PACKETS_STATE;
typedef struct stClientLogin : public stPacket, cObjectPool<stClientLogin> {
	DWORD id;
	DWORD password;
	stClientLogin() :stPacket(ClientLogin) {};
}PACKETC_LOGIN;
typedef struct stClientObjects : public stPacket, cObjectPool<stClientObjects> {
	stClientObjects() :stPacket(ClientObjectsCompletion) {};
}PACKETC_OBJECT;
typedef struct stClientRequestState : public stPacket, cObjectPool<stClientRequestState> {
	stClientRequestState() :stPacket(ClientRequestState) {};
}PACKETC_REQSTATE;
typedef struct stClientState : public stPacket, cObjectPool<stClientState> {
	WORD	state;
	float	x;
	float	y;
	stClientState() :stPacket(ClientState) {};
}PACKETC_STATE;
typedef  struct stClientChat : public stPacket, cObjectPool<stClientChat> {
	char chat[256] = "";
	stClientChat() :stPacket(ClientChat) {};
}PACKET_CHAT;
#pragma pack(pop)
class cPacketManager
{
	static SPINLOCK	lock_packet;
	static size_t	packetSize[256];
public:
	cPacketManager();
	~cPacketManager();
	static bool			Initialize(size_t opSize);

	static void			Serialization(char* des, stPacket* pPacket);
	static stPacket*	Deserialization(const char* buffer);

	static void			DeletePacket(stPacket* pPacket);
};

#endif // !1