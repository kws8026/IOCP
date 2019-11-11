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
			return nullptr;	// 이미 캐퍼시티가 지정 되어있으면 실패
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
		if (capacity == size) {		// 오브젝트가 최대치만크 생성된 경우
			if (!bExpansion) {		// 더 이상 오브젝트를 생성할 수 없음
				return nullptr 
			}
			else if(!Expansion()) {	// 확장에 실패할 경우
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