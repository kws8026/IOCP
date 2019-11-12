#pragma once
#ifndef __OBJECTPOOL_UTIL
#define __OBJECTPOOL_UTIL

#include "Singleton.h"
#include "Logger.h"
#include <Windows.h>

template <typename T>
class cObjectPool : cSingleton<cObjectPool> // �̱��� ������ ����Ͽ� ������Ʈ���� �ϳ��� ������ƮǮ�� ���鵵�� ��
{
	struct  Node{
		T* pData;
		Node* pNext;
		Node():pData(nullptr),pNext(nullptr)
		{
				pData = new T
		};
		~Node() { 
			if (pData != nullptr) {
			delete pData }
		};
	};
	Node*			front;			// ����Ʈ�� ù ���
	Node*			end;			// ����Ʈ�� ���� ���
	Node*			assigned;		// �Ҵ� ����Ʈ�� ù ���
	size_t			capacity;		// ����Ʈ�� ĳ�۽�Ƽ
	size_t			size;			// �Ҵ� ����Ʈ�� ũ��
	bool			bExpansion;		// ĳ�۽�Ƽ�� �ʰ��� ��� Ȯ�������� ������ �÷���

	bool	Expansion() {
		Node* temp = new Node;
		end->pNext = temp;
		for (int i = 1; i < capacity; i++) {
			Node* other = new Node;
			temp->pNext = other;
			temp = other;
			if (other == NULL) {
				ERROR_CODE("Create Node Failed");
				return false;
			}
		}
		end = temp;
		capacity *= 2;
		return true;
	}
public:
	cObjectPool():front(nullptr), end(nullptr), assigned(nullptr), capacity(0), size(0), bExpansion(false){
	};
	~cObjectPool() {
	};
	T*		CreatePool(size_t _capacity, bool expansion = false) {
		if (capacity != 0){
			ERROR_CODE("Already Created ObjectPool");
			return nullptr;	// �̹� ĳ�۽�Ƽ�� ���� �Ǿ������� ����
		}
		capacity = _capacity;
		bExpansion = expansion;
		front = new Node;
		Node* temp = front;
		for (int i = 1; i < capacity; i++) {
			Node* other = new Node;
			temp->pNext = other;
			temp = other;
			if (other == NULL) {
				ERROR_CODE("Create Node Failed");
				return false;
			}
		}
		end = temp;
		LOG("Create ObjectPool objects : %d",capacity);
		return true;
	};
	T*		New() {
		if (capacity == size) {		// ������Ʈ�� �ִ�ġ��ũ ������ ���
			if (!bExpansion) {		// �� �̻� ������Ʈ�� ������ �� ����
				ERROR_CODE("ObjectPool is fulled");
				return nullptr 
			}
			else if(!Expansion()) {	// Ȯ�忡 ������ ���
				return nullptr		
			}
		}
		size++;
		auto pReturn = front;
		front->pNext = pReturn;		// ����� ��带 ����Ʈ���� ����
		pReturn->pNext = assigned;	// ����� ��带 �Ҵ� ����Ʈ�� ���
		assigned = pReturn;

		return pReturn.pData;
	};
	bool	Delete(T* object) {
		Node* temp;								// ���� ��带 �ӽ�������
		auto iter = assigned;					// �Ҵ� ����Ʈ���� Ž����
		for (;iter != nullptr; iter = iter.pNext) {
			if (iter.pData == object) {			// ������Ʈ�� ���� ��带 �߰���
				ZeroMemory(object, sizeof(T));	// ������Ʈ�� �ʱ�ȭ ��Ŵ
				if (temp != nullptr) {			// ������Ʈ�� ���� ��尡 �Ҵ� ����Ʈ�� ù ����� ���
					temp->pNext = iter->pNext;	// �Ҵ� ����Ʈ���� ������
				}
				end->pNext = iter;				// ����Ʈ�� ���ܿ� �ٽ� �����
				end = iter;
			}
			temp = iter;
		}
		return false;				// �Ҵ� ����Ʈ���� �ش��ϴ� ������Ʈ�� ã�� ������ ���
	};
	void	Destroy() {
		auto iter = assigned;					// �Ҵ� ����Ʈ���� Ž����
		for (;iter != nullptr;) {
			auto temp = iter;
			iter = iter->pNext;
			delete(temp->pNext);
			temp->pNext = nullptr;
			delete(temp);
		}
		auto iter = front;					// ����Ʈ���� Ž����
		for (;iter != nullptr;) {
			auto temp = iter;
			iter = iter->pNext;
			delete(temp->pNext);
			temp->pNext = nullptr;
			delete(temp);
		}
	}
};


#define NEW() cObjectPool::Instance()->New();
#define DELETE(object) cObjectPool::Instance()->Delete(object);

#endif