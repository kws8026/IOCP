#pragma once
#ifndef __SINGLETON_TEMPLATE
#define __SINGLETON_TEMPLATE

#include "FastSpinlock.h"

template <typename T>
class cSingleton
{
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
#endif 
