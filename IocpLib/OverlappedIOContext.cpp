#include "pch.h"
#include "OverlappedIOContext.h"

void DeleteIoContext(LPIOCONTEXT context)
{
	if (nullptr == context) {
		return;
	}
	/// ObjectPool's operate delete dispatch
	switch (context->cmd)
	{
	case IO_Send:
		RELEASE(CONTEXT_SEND, context);
		break;

	case IO_RecvZero:
		RELEASE(CONTEXT_PRERECV, context);
		break;

	case IO_Recv:
		RELEASE(CONTEXT_RECV, context);
		break;

	case IO_Accept:
		RELEASE(CONTEXT_ACCEPT, context);
		break;

	case IO_Connect:
		RELEASE(CONTEXT_CON, context);
		break;

	case IO_Disconnect:
		RELEASE(CONTEXT_DISCON, context);
		break;

	default:
		LOG_ERROR("Invalide Commend");
	}
}

void CreateIOPool(size_t size)
{
	CONTEXT_ACCEPT::CreatePool(size,true);
	CONTEXT_RECV::CreatePool(size, true);
	CONTEXT_PRERECV::CreatePool(size, true);
	CONTEXT_SEND::CreatePool(size, true);
	CONTEXT_CON::CreatePool(size, true);
	CONTEXT_DISCON::CreatePool(size, true);
}
