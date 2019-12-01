#pragma once

#ifndef __OVERLAPPEDIOCONTEXT_IOCP
#define __OVERLAPPEDIOCONTEXT_IOCP

#include "Session.h"
#include "Objectpool.h"

enum IOCommend { 
	IO_Default = 137,
	IO_Send,
	IO_Recv,
	IO_RecvZero,
	IO_Accept,
	IO_Connect,
	IO_Disconnect
};

typedef struct stOverlappedIOContext {
	WSAOVERLAPPED	overlapped;
	WSABUF			buf;
	WORD			cmd;
	LPSESSION 		owner;
	stOverlappedIOContext(WORD cmd):cmd(cmd){
		ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));
		ZeroMemory(&buf, sizeof(WSABUF));
	}
	void SetSession(LPSESSION other) {owner = other;}
}IOCONTEXT,*LPIOCONTEXT;

typedef struct stSendContext	
	: public stOverlappedIOContext,cObjectPool<stSendContext>{
	stSendContext() 
		: stOverlappedIOContext(IO_Send) {
	}
}CONTEXT_SEND, * LPCONTEXT_SEND;

typedef struct stRecvContext	
	: public stOverlappedIOContext, cObjectPool<stRecvContext> {
	stRecvContext() 
		: stOverlappedIOContext(IO_Recv) {
	}
}CONTEXT_RECV, * LPCONTEXT_RECV;

typedef struct stPreRecvContext 
	: public stOverlappedIOContext, cObjectPool<stPreRecvContext> {
	stPreRecvContext() 
		: stOverlappedIOContext(IO_RecvZero) {
	}
}CONTEXT_PRERECV, * LPCONTEXT_PRERECV;

typedef struct stAcceptContext	
	: public stOverlappedIOContext, cObjectPool<stAcceptContext> {
	stAcceptContext()
		: stOverlappedIOContext(IO_Accept) {
	}
}CONTEXT_ACCEPT, * LPCONTEXT_ACCEPT;

typedef struct stConnectContext 
	: public stOverlappedIOContext, cObjectPool<stConnectContext> {
	stConnectContext() 
		: stOverlappedIOContext(IO_Connect) {
	}
}CONTEXT_CON, * LPCONTEXT_CON;

typedef struct stDisconnectContext 
	: public stOverlappedIOContext, cObjectPool<stDisconnectContext> {
	stDisconnectContext() 
		: stOverlappedIOContext(IO_Disconnect) {
	}
}CONTEXT_DISCON, * LPCONTEXT_DISCON;

void DeleteIoContext(LPIOCONTEXT context);
void CreateIOPool(size_t size);

#endif 
