#include "pch.h"
#include "FastSpinlock.h"
cFastSpinlock::cFastSpinlock() : lockflag(0)
{
}


cFastSpinlock::~cFastSpinlock()
{
}


void cFastSpinlock::EnterLock()
{
	for (int nloops = 0; ; nloops++)
	{
		if (InterlockedExchange(&lockflag, 1) == 0) {
			return;
		}

		Sleep((DWORD)min(10, nloops));
	}

}

void cFastSpinlock::LeaveLock()
{
	InterlockedExchange(&lockflag, 0);
}