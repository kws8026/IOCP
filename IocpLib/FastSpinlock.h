#pragma once
#ifndef __SPINLOCK_UTIL
#define __SPINLOCK_UTIL

typedef class cFastSpinlock
{
public:
	cFastSpinlock();
	~cFastSpinlock();

	void EnterLock();
	void LeaveLock();

private:
	cFastSpinlock(const cFastSpinlock& rhs);
	cFastSpinlock& operator=(const cFastSpinlock& rhs);

	volatile long mLockFlag;
}SPINLOCK;

class FastSpinlockGuard
{
public:
	FastSpinlockGuard(cFastSpinlock& lock) : mLock(lock)
	{
		mLock.EnterLock();
	}

	~FastSpinlockGuard()
	{
		mLock.LeaveLock();
	}

private:
	SPINLOCK& mLock;
};

template <class TargetClass>
class ClassTypeLock
{
public:
	typedef struct LockGuard
	{
		LockGuard()
		{
			TargetClass::mLock.EnterLock();
		}

		~LockGuard()
		{
			TargetClass::mLock.LeaveLock();
		}

	}LOCK;

private:
	static SPINLOCK mLock;

	//friend struct LockGuard;
};

template <class TargetClass>
SPINLOCK ClassTypeLock<TargetClass>::mLock;

#endif // !__SPINLOCK_UTIL