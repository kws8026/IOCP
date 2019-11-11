#pragma once
#ifndef __CRITICALSECTION_UTIL
#define __CRITICALSECTION_UTIL

#include <Windows.h>

typedef class cCriticalSection
{
	CRITICAL_SECTION cs;
public:
	cCriticalSection();
	~cCriticalSection();
	
	void Enter();
	void Leave();
}CS;

#endif 