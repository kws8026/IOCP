#pragma once
#ifndef __SINGLETON_TEMPLATE
#define __SINGLETON_TEMPLATE

#include "FastSpinlock.h"

template <typename T>
class cSingleton : public ClassTypeLock<cSingleton>
{
	static T* pInstance;
public:
	cSingleton() {}
	virtual ~cSingleton() {
		if (pInstance) {
			delte pInstance;
			pInstance = NULL;
		}
	}
	static T* Instance() {
		LockGuard interlock;
		if (pInstance == nullptr) {
			pInstance = new T;
		}
		return pInstance;
	}
	static void Destroy() {
		if (pInstance)
			delete pInstance;
	}
};

template<typename T> T* cSingleton<T>::pInstance = nullptr;
#endif 
