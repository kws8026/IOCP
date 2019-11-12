#pragma once

#ifndef __OVERLAPPEDIOCONTEXT_IOCP
#define __OVERLAPPEDIOCONTEXT_IOCP

#include "Session.h"

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
	OVERLAPPED	overlapped;
	WSABUF		buf;
	IOCommend	cmd;
	LPSESSION	session;
	stOverlappedIOContext(LPSESSION owner, IOCommend cmd);
}IOCONTEXT,*LPIOCONTEXT;

#endif 
