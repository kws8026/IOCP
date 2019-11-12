#pragma once
#ifndef __SINGLETON_TEMPLATE
#define __SINGLETON_TEMPLATE

template <typename T>
class cSingleton
{
	static T* pInstance;
public:
	cSingleton() {}
	virtual ~cSingleton() {
		if (pInstance) {
			pInstance = NULL;
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
	}
};

template<typename T> T* cSingleton<T>::pInstance = nullptr;
#endif 
