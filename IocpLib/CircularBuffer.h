#pragma once
#ifndef __CIRCULARBUFFER_UTIL
#define __CIRCULARBUFFER_UTIL

#define MAX_OF_BUFFER 256

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
			strcpy_s(data,MAX_OF_BUFFER,object);
		}
	};
	Node*	buffer;
	size_t	size;
	Node*	front;
	Node*	back;
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
	bool	Commit() {
		if (!IsFull()) {
			size++;
			return true;
		}
		return false;
	}
	char*	Front() {
		ZeroMemory(front->data, sizeof(front->data));
		return front->data;
	}
	Node*	push(const char* object) {
		if (size != 0 && front == back)
			return nullptr;
		back->push(object);
		back = back->next;
		size++;
		return back;
	}
	char*	pop() {
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