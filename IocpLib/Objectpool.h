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
	public ClassTypeLock<cObjectPool>					// �̱��� ������ ����Ͽ� ������Ʈ���� �ϳ��� ������ƮǮ�� ���鵵�� ��
{
	Object**		objects;							// ������Ʈ�� �����Ͱ� ����� �迭
	DWORD			capacity;							// ����Ʈ�� ĳ�۽�Ƽ
	DWORD			size;								// �Ҵ� ����Ʈ�� ũ��
	bool			bExpansion;							// ĳ�۽�Ƽ�� �ʰ��� ��� Ȯ�������� ������ �÷���

	bool	Expansion() {
		DWORD expansion = capacity*2;					// 2��� Ȯ��� ĳ�۽�Ƽ
		Object** temp = new Object* [expansion];		// ���ο� ������Ʈ �迭 ����
		if (temp == NULL) {			
			ERROR_CODE("Failed Create New Objects");
			return false;
		}
		ZeroMemory(temp, sizeof(Object*) * expansion);
		for (int i = 0; i < capacity; i++) {
			temp[i] = objects[i];						// ���� ������Ʈ���� ������ ����
		}
		for (int i = capacity; i < expansion; i++) {	// Ȯ��� ĳ�۽�Ƽ��ŭ ������Ʈ�� ����
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
			return nullptr;				// �̹� ĳ�۽�Ƽ�� ���� �Ǿ������� ����
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

		if (capacity == size) {			// ������Ʈ�� �ִ�ġ��ũ ������ ���
			if (!bExpansion) {			// �� �̻� ������Ʈ�� ������ �� ����
				ERROR_CODE("ObjectPool is fulled");
				return nullptr 
			}
			else if(!Expansion()) {		// Ȯ�忡 ������ ���
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
				auto target = objects[--size];   // ����� ���ҽ�Ű�� ������ ������Ʈ�� �ӽ�����
				objects[size] = objects[i];		// ������ ������Ʈ�� �迭�� �������� ��ġ
				objects[i] = target;			// �ӽ������ ������Ʈ�� ������ ������Ʈ�� �ִ� ��ġ�� �̵�
				return true;
			}
		}
		ERROR_CODE("Invalied Object Pointer");
		return false;							// �Ҵ� ����Ʈ���� �ش��ϴ� ������Ʈ�� ã�� ������ ���
	};
};
#endif

#define NEW(Object_type) cObjectPool<Object_type>::New();
#define DELETE(Object_type,object)	cObjectPool<Object_type>::Delete(static_cast<Object_type*>(object));