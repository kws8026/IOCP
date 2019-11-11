#pragma once
#ifndef __OBJECTPOOL_UTIL
#define __OBJECTPOOL_UTIL

#include "Singleton.h"

template <typename T>
class cObjectpool : cSingleton<cObjectpool>
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
	Node*			front;
	Node*			end;
	size_t			capacity;
	size_t			size;
	bool			bExpansion;
public:
	cObjectpool():capacity(0), size(0), bExpansion(false){
	};
	~cObjectpool() {
		delete end;
	};
	T* CreatePool(size_t _capacity, bool expansion = false) {
		if (capacity != 0){
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
		}
		end = temp;
		return true;
	};
	T*		New() {
		if (capacity == size) {		// ������Ʈ�� �ִ�ġ��ũ ������ ���
			if (!bExpansion) {		// �� �̻� ������Ʈ�� ������ �� ����
				return nullptr 
			}
			else if(!Expansion()) {	// Ȯ�忡 ������ ���
				return nullptr		
			}
		}
		size++;
	};
	void	Delete(T* object) {
	
	};
	bool	Expansion() {
	
	}
};


#define NEW() cObjectpool::Instance()->New();
#define DELETE(object) cObjectpool::Instance()->Delete(object);

#endif