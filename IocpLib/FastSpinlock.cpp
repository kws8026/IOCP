#include "pch.h"
#include "FastSpinlock.h"
#include <timeapi.h>

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

		UINT uTimerRes = 1;
		timeBeginPeriod(uTimerRes);
		Sleep((DWORD)min(10, nloops));
		timeEndPeriod(uTimerRes);
	}

}

void cFastSpinlock::LeaveLock()
{
	InterlockedExchange(&mLockFlag, 0);
}