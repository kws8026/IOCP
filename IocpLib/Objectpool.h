#pragma once
#ifndef __OBJECTPOOL_UTIL
#define __OBJECTPOOL_UTIL

#include "Singleton.h"
#include "Logger.h"
#include "FastSpinlock.h"

#define _WINSOCKAPI_
#include <Windows.h>

template <typename Object>
class cObjectPool 
{
	static SPINLOCK		lock_op;
	static Object**		objects;							// ������Ʈ�� �����Ͱ� ����� �迭
	static WORD			capacity;							// ����Ʈ�� ĳ�۽�Ƽ
	static WORD			size;								// �Ҵ� ����Ʈ�� ũ��
	static bool			bExpansion;							// ĳ�۽�Ƽ�� �ʰ��� ��� Ȯ�������� ������ �÷���

	static bool	Expansion() {
		WORD expansion = capacity*2;					// 2��� Ȯ��� ĳ�۽�Ƽ
		Object** temp = new Object* [expansion];		// ���ο� ������Ʈ �迭 ����
		if (temp == NULL) {			
			LOG_ERROR("Failed Create New Objects");
			return false;
		}
		ZeroMemory(temp, sizeof(Object*) * expansion);
		for (int i = 0; i < capacity; i++) {
			temp[i] = objects[i];						// ���� ������Ʈ���� ������ ����
		}
		for (int i = capacity; i < expansion; i++) {	// Ȯ��� ĳ�۽�Ƽ��ŭ ������Ʈ�� ����
			temp[i] = new Object;
			if (temp[i] == NULL) {
				LOG_ERROR("Failed Create New Object");
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
	cObjectPool(){};
	~cObjectPool() {
		if (capacity == 0)
			return;
		for (int i = 0; i < capacity; i++) {
			delete objects[i];
			objects[i] = NULL;
		}
		delete[] objects;
	};
	static bool		CreatePool(WORD _capacity = 100, bool expansion = false) {
		FastSpinlockGuard enter(lock_op);
		if (capacity != 0){
			LOG_ERROR("Already Created ObjectPool");
			return nullptr;				// �̹� ĳ�۽�Ƽ�� ���� �Ǿ������� ����
		}
		capacity = _capacity;
		bExpansion = expansion;
		objects = new Object*[capacity];
		if (objects == NULL) {
			LOG_ERROR("Failed Create New Objects");
			return nullptr;
		}
		for (int i = 0; i < capacity; i++) {
			objects[i] = new Object;
			if (objects[i] == NULL) {
				return false;
			}
		}
		LOG("Create ObjectPool");
		return true;
	};
	static Object*	New() {
		FastSpinlockGuard enter(lock_op);
		if (capacity == size) {			// ������Ʈ�� �ִ�ġ��ũ ������ ���
			if (!bExpansion) {			// �� �̻� ������Ʈ�� ������ �� ����
				LOG_ERROR("ObjectPool is fulled");
				return nullptr;
			}
			else if(!Expansion()) {		// Ȯ�忡 ������ ���
				return nullptr;
			}
		}
		auto pReturn = objects[size++];
		return pReturn;
	};
	static bool		Release(Object* object) {
		FastSpinlockGuard enter(lock_op);
		for (int i = 0; i < size; i++) {
			if (objects[i] == object) {
				auto target = objects[--size];   // ����� ���ҽ�Ű�� ������ ������Ʈ�� �ӽ�����
				objects[size] = objects[i];		// ������ ������Ʈ�� �迭�� �������� ��ġ
				objects[i] = target;			// �ӽ������ ������Ʈ�� ������ ������Ʈ�� �ִ� ��ġ�� �̵�
				return true;
			}
		}
		LOG_ERROR("Invalied Object Pointer");
		return false;							// �Ҵ� ����Ʈ���� �ش��ϴ� ������Ʈ�� ã�� ������ ���
	};
};

// �Ű����� �ʱ�ȭ
template <typename Object>
Object** cObjectPool<Object>::objects = nullptr;							
template <typename Object>
WORD	 cObjectPool<Object>::capacity = 0;				
template <typename Object>
WORD	 cObjectPool<Object>::size = 0;		
template <typename Object>
bool	 cObjectPool<Object>::bExpansion = false;
template<typename Object>
SPINLOCK cObjectPool<Object>::lock_op;

// ���� ��� ���̸� ���� �Լ� ������
#define NEW(Object_type) cObjectPool<Object_type>::New();
#define RELEASE(Object_type,object)	cObjectPool<Object_type>::Release(static_cast<Object_type*>(object));

#endif