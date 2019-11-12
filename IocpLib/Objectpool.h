#pragma once
#ifndef __OBJECTPOOL_UTIL
#define __OBJECTPOOL_UTIL

#include "Singleton.h"
#include "Logger.h"
#include <Windows.h>

template <typename T>
class cObjectPool : cSingleton<cObjectPool> // 싱글톤 패턴을 사용하여 오브젝트마다 하나의 오브젝트풀만 만들도록 함
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
	Node*			front;			// 리스트의 첫 노드
	Node*			end;			// 리스트의 말단 노드
	Node*			assigned;		// 할당 리스트의 첫 노드
	size_t			capacity;		// 리스트의 캐퍼시티
	size_t			size;			// 할당 리스트의 크기
	bool			bExpansion;		// 캐퍼시티를 초과할 경우 확장할지를 결정할 플래그

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
		if (capacity == size) {		// 오브젝트가 최대치만크 생성된 경우
			if (!bExpansion) {		// 더 이상 오브젝트를 생성할 수 없음
				ERROR_CODE("ObjectPool is fulled");
				return nullptr 
			}
			else if(!Expansion()) {	// 확장에 실패할 경우
				return nullptr		
			}
		}
		size++;
		auto pReturn = front;
		front->pNext = pReturn;		// 사용한 노드를 리스트에서 삭제
		pReturn->pNext = assigned;	// 사용한 노드를 할당 리스트에 등록
		assigned = pReturn;

		return pReturn.pData;
	};
	bool	Delete(T* object) {
		Node* temp;								// 이전 노드를 임시저장함
		auto iter = assigned;					// 할당 리스트에서 탐색함
		for (;iter != nullptr; iter = iter.pNext) {
			if (iter.pData == object) {			// 오브젝트를 가진 노드를 발견함
				ZeroMemory(object, sizeof(T));	// 오브젝트를 초기화 시킴
				if (temp != nullptr) {			// 오브젝트를 가진 노드가 할당 리스트의 첫 노드일 경우
					temp->pNext = iter->pNext;	// 할당 리스트에서 제거함
				}
				end->pNext = iter;				// 리스트의 말단에 다시 등록함
				end = iter;
			}
			temp = iter;
		}
		return false;				// 할당 리스트에서 해당하는 오브젝트를 찾지 못했을 경우
	};
	void	Destroy() {
		auto iter = assigned;					// 할당 리스트에서 탐색함
		for (;iter != nullptr;) {
			auto temp = iter;
			iter = iter->pNext;
			delete(temp->pNext);
			temp->pNext = nullptr;
			delete(temp);
		}
		auto iter = front;					// 리스트에서 탐색함
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