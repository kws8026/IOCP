#include "pch.h"
#include "FastSpinlock.h"
cFastSpinlock::cFastSpinlock() : mLockFlag(0)
{
}


cFastSpinlock::~cFastSpinlock()
{
}


void cFastSpinlock::EnterLock()
{
	for (int nloops = 0; ; nloops++)
	{
		if (InterlockedExchange(&mLockFlag, 1) == 0) {
			return;
		}

		Sleep((DWORD)min(10, nloops));
	}

}

void cFastSpinlock::LeaveLock()
{
	InterlockedExchange(&mLockFlag, 0);
}