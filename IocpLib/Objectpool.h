#pragma once
#ifndef __OBJECTPOOL_UTIL
#define __OBJECTPOOL_UTIL

#include "Singleton.h"
#include "Logger.h"
#include "FastSpinlock.h"

#define _WINSOCKAPI_
#include <Windows.h>

template <typename Object>
class cObjectPool : 
	public ClassTypeLock<cObjectPool>					// 싱글톤 패턴을 사용하여 오브젝트마다 하나의 오브젝트풀만 만들도록 함
{
	Object**		objects;							// 오브젝트의 포인터가 적재된 배열
	DWORD			capacity;							// 리스트의 캐퍼시티
	DWORD			size;								// 할당 리스트의 크기
	bool			bExpansion;							// 캐퍼시티를 초과할 경우 확장할지를 결정할 플래그

	bool	Expansion() {
		DWORD expansion = capacity*2;					// 2배로 확장된 캐퍼시티
		Object** temp = new Object* [expansion];		// 새로운 오브젝트 배열 생성
		if (temp == NULL) {			
			ERROR_CODE("Failed Create New Objects");
			return false;
		}
		ZeroMemory(temp, sizeof(Object*) * expansion);
		for (int i = 0; i < capacity; i++) {
			temp[i] = objects[i];						// 기존 오브젝트들의 정보를 복사
		}
		for (int i = capacity; i < expansion; i++) {	// 확장된 캐퍼시티만큼 오브젝트를 생성
			temp[i] = new T;
			if (temp[i] == NULL) {
				ERROR_CODE("Failed Create New Object");
				delete[] temp;
				return false;
			}
		}
		delete[] objects;
		objects = temp;
		capacity = expansion;
		return true;
	}
public:
	cObjectPool():Objects(nullptr), capacity(0), size(0), bExpansion(false){
	};
	~cObjectPool() {
		if (capacity == 0)
			return;
		for (int i = 0; i < capacity; i++) {
			delete objects[i];
			objects[i] = NULL;
		}
		delete[] objects;
	};
	static bool		CreatePool(DWORD _capacity = 100, bool expansion = false) {
		LockGuard spinlock;

		if (capacity != 0){
			ERROR_CODE("Already Created ObjectPool");
			return nullptr;				// 이미 캐퍼시티가 지정 되어있으면 실패
		}
		capacity = _capacity;
		bExpansion = expansion;
		objects = new Object*[capacity];
		if (objects == NULL) {
			ERROR_CODE("Failed Create New Objects");
			return nullptr;
		}
		ZeroMemory(objects, sizeof(Object*)*capacity);
		for (int i = 0; i < capacity; i++) {
			objects[i] = new T;
			if (objects[i] == NULL) {
				return false;
			}
		}
		LOG("Create ObjectPool objects : %d",capacity);
		return true;
	};
	static Object*	New() {

		LockGuard spinlock;

		if (capacity == size) {			// 오브젝트가 최대치만크 생성된 경우
			if (!bExpansion) {			// 더 이상 오브젝트를 생성할 수 없음
				ERROR_CODE("ObjectPool is fulled");
				return nullptr 
			}
			else if(!Expansion()) {		// 확장에 실패할 경우
				return nullptr		
			}
		}
		auto pReturn = objects[size++];
		ZeroMemory(pReturn,sizeof(Object));
		return pReturn;
	};
	static bool		Delete(Object* object) {

		LockGuard spinlock;

		for (int i = 0; i < size; i++) {
			if (objects[i] = object) {
				auto target = objects[--size];   // 사이즈를 감소시키고 말단의 오브젝트를 임시저장
				objects[size] = objects[i];		// 해제한 오브젝트를 배열의 말단으로 배치
				objects[i] = target;			// 임시저장된 오브젝트를 해제한 오브젝트가 있던 위치로 이동
				return true;
			}
		}
		ERROR_CODE("Invalied Object Pointer");
		return false;							// 할당 리스트에서 해당하는 오브젝트를 찾지 못했을 경우
	};
};
#endif

#define NEW(Object_type) cObjectPool<Object_type>::New();
#define DELETE(Object_type,object)	cObjectPool<Object_type>::Delete(static_cast<Object_type*>(object));