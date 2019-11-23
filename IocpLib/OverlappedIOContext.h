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
	IOCommend		cmd;
	LPSESSION		pSession;
	stOverlappedIOContext(LPSESSION owner, IOCommend cmd) {
		ZeroMemory(&overlapped, sizeof(WSAOVERLAPPED));
		ZeroMemory(&buf, sizeof(WSABUF));
	}
}IOCONTEXT,*LPIOCONTEXT;

typedef struct stSendContext	
	: public stOverlappedIOContext,cObjectPool<stSendContext>{
	stSendContext(LPSESSION owner) 
		: stOverlappedIOContext(owner, IO_Send) {
		CreatePool();
	}
}CONTEXT_SEND, * LPCONTEXT_SEND;

typedef struct stRecvContext	
	: public stOverlappedIOContext, cObjectPool<stRecvContext> {
	stRecvContext(LPSESSION owner) 
		: stOverlappedIOContext(owner, IO_Recv) {
		CreatePool();
	}
}CONTEXT_RECV, * LPCONTEXT_RECV;

typedef struct stPreRecvContext 
	: public stOverlappedIOContext, cObjectPool<stPreRecvContext> {
	stPreRecvContext(LPSESSION owner) 
		: stOverlappedIOContext(owner, IO_RecvZero) {
		CreatePool();
	}
}CONTEXT_PRERECV, * LPCONTEXT_PRERECV;

typedef struct stAcceptContext	
	: public stOverlappedIOContext, cObjectPool<stAcceptContext> {
	stAcceptContext(LPSESSION owner) 
		: stOverlappedIOContext(owner, IO_Accept) {
		CreatePool();
	}
}CONTEXT_ACCEPT, * LPCONTEXT_ACCEPT;

typedef struct stConnectContext 
	: public stOverlappedIOContext, cObjectPool<stConnectContext> {
	stConnectContext(LPSESSION owner) 
		: stOverlappedIOContext(owner, IO_Connect) {
		CreatePool();
	}
}CONTEXT_CON, * LPCONTEXT_CON;

typedef struct stDisconnectContext 
	: public stOverlappedIOContext, cObjectPool<stDisconnectContext> {
	stDisconnectContext(LPSESSION owner) 
		: stOverlappedIOContext(owner, IO_Disconnect) {
		CreatePool();
	}
}CONTEXT_DISCON, * LPCONTEXT_DISCON;

void DeleteIoContext(LPIOCONTEXT context) {
	if (nullptr == context) {
		return;
	}
	/// ObjectPool's operate delete dispatch
	switch (context->cmd)
	{
	case IO_Send:
		DELETE(CONTEXT_SEND,context);
		break;

	case IO_RecvZero:
		DELETE(CONTEXT_PRERECV, context);
		break;

	case IO_Recv:
		DELETE(CONTEXT_RECV, context); 
		break;

	case IO_Accept:
		DELETE(CONTEXT_ACCEPT, context); 
		break;

	case IO_Connect:
		DELETE(CONTEXT_CON, context); 
		break;

	case IO_Disconnect:
		DELETE(CONTEXT_DISCON, context);
		break;

	default:
		LOG_ERROR("Invalide Commend");
	}
};

#endif 
