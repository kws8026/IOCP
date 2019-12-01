#pragma once
#ifndef __SINGLETON_TEMPLATE
#define __SINGLETON_TEMPLATE

#include "FastSpinlock.h"

template <typename T>
class cSingleton
{
	static SPINLOCK lock_single;
	static T* pInstance;
public:
	cSingleton() {}
	virtual ~cSingleton() {
		if (pInstance) {
			delete pInstance;
			pInstance = nullptr;
		}
	}
	static T* Instance() {
		FastSpinlockGuard lock(lock_single);
		if (pInstance == nullptr) {
			pInstance = new T;
		}
		return pInstance;
	}
	static void Destroy() {
		if (pInstance)
			delete pInstance;
		pInstance = nullptr;
	}
};

template<typename T> 
T* cSingleton<T>::pInstance = nullptr;

template<typename T>
SPINLOCK cSingleton<T>::lock_single;
#endif 
