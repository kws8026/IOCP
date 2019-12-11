#pragma once
#ifndef __CIRCULARBUFFER_UTIL
#define __CIRCULARBUFFER_UTIL

#define MAX_OF_BUFFER 2000
#include "FastSpinlock.h"
class CircularBuffer
{
	struct Node {
		char* data;
		Node* next;
		Node() :data(nullptr),next(nullptr){
			data = new char[MAX_OF_BUFFER];
		}
		~Node() {
			delete[] data;
		}
		void push(const char* object) { 
			ZeroMemory(data,sizeof(data));
			memcpy(data,object, MAX_OF_BUFFER);
		}
	};
	SPINLOCK	lock;
	Node*		buffer;
	size_t		size;
	Node*		front;
	Node*		back;
public:
	CircularBuffer(size_t capacity = 128): size(0){
		buffer = new Node[capacity];
		for (int i = 1; i < capacity; i++) {
			buffer[i-1].next = &buffer[i];
		}
		buffer[capacity-1].next = &buffer[0];
		front = buffer;
		back = buffer;
	}
	~CircularBuffer() {
		delete[] buffer;
	}
	bool	IsFull() {
		return (size != 0 && front == back) ? true : false;
	}
	bool	IsEmpty() {
		return size == 0;
	}
	char*	Postpush() {
		FastSpinlockGuard lock(lock);
		if (!IsFull()) {
			ZeroMemory(back->data, sizeof(back->data));
			char* buff = back->data;
			back = back->next;
			size++;
			return buff;
		}
		return nullptr;
	}
	char*	Ready() {
		FastSpinlockGuard lock(lock);
		if (!IsFull()) {
			ZeroMemory(back->data, sizeof(back->data));
			char* buff = back->data;
			back = back->next;
			return buff;
		}
		return nullptr;
	}
	void	Commit() {
		FastSpinlockGuard lock(lock);
		size++;
	}
	Node*	push(const char* object) {
		FastSpinlockGuard lock(lock);
		if (size != 0 && front == back)
			return nullptr;
		back->push(object);
		back = back->next;
		size++;
		return back;
	}
	char*	pop() {
		FastSpinlockGuard lock(lock);
		if (size == 0)
			return nullptr;
		else{
			char* data = front->data;
			front = front->next;
			size--;
			return data;
		}
	}
};

#endif // !__POINTERBUFFER_UTIL