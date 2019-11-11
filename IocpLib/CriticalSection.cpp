#include "pch.h"
#include "CriticalSection.h"

cCriticalSection::cCriticalSection()
{
	InitializeCriticalSection(&cs);
}

cCriticalSection::~cCriticalSection()
{
	DeleteCriticalSection(&cs);
}

void cCriticalSection::Enter()
{
	EnterCriticalSection(&cs);
}

void cCriticalSection::Leave()
{
	LeaveCriticalSection(&cs);
}