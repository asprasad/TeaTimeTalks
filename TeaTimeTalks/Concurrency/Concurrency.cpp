// Concurrency.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <cassert>

class ProtectedCounter
{
	int m_val;
	CRITICAL_SECTION m_lock;
	HANDLE m_event;
public:
	ProtectedCounter(int initialVal)
	{
		m_val = initialVal;
		InitializeCriticalSection(&m_lock);
		m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	void WaitForEvent()
	{
		WaitForSingleObject(m_event, INFINITE);
	}

	void SignalEvent()
	{
		SetEvent(m_event);
	}

	~ProtectedCounter()
	{
		DeleteCriticalSection(&m_lock);
		CloseHandle(m_event);
	}

	void Lock()
	{
		EnterCriticalSection(&m_lock);
	}

	void Unlock()
	{
		LeaveCriticalSection(&m_lock);
	}

	void Increment()
	{
		// EnterCriticalSection(&m_lock);
		++m_val;
		// LeaveCriticalSection(&m_lock);
	}

	void Decrement()
	{
		// EnterCriticalSection(&m_lock);
		--m_val;
		// LeaveCriticalSection(&m_lock);
	}

	int GetCounterValue()
	{
		return m_val;
	}
};

struct ThreadArgument
{
	ProtectedCounter *firstCounter;
	ProtectedCounter *secondCounter;
	int initialTotal;
};

DWORD WINAPI PlayerThreadFunction_FirstPlayer(LPVOID arg)
{
	ThreadArgument *pairOfCounters = (ThreadArgument*)arg;
	while (true)
	{
		pairOfCounters->firstCounter->WaitForEvent();

		std::cout << "Player 1 playing ...\n";

		pairOfCounters->firstCounter->Lock();
		pairOfCounters->secondCounter->Lock();
			
		pairOfCounters->firstCounter->Increment();
		pairOfCounters->secondCounter->Decrement();

		pairOfCounters->secondCounter->Unlock();
		pairOfCounters->firstCounter->Unlock();

		pairOfCounters->secondCounter->SignalEvent();
	}
	return 0;
}

DWORD WINAPI PlayerThreadFunction_SecondPlayer(LPVOID arg)
{
	ThreadArgument *pairOfCounters = (ThreadArgument*)arg;
	while (true)
	{
		pairOfCounters->firstCounter->WaitForEvent();

		std::cout << "Player 2 playing ...\n";

		pairOfCounters->secondCounter->Lock();
		pairOfCounters->firstCounter->Lock();

		pairOfCounters->firstCounter->Increment();
		pairOfCounters->secondCounter->Decrement();

		pairOfCounters->firstCounter->Unlock();
		pairOfCounters->secondCounter->Unlock();

		pairOfCounters->secondCounter->SignalEvent();
	}
	return 0;
}

DWORD WINAPI ObserverThreadFunction(LPVOID arg)
{
	ThreadArgument *pairOfCounters = (ThreadArgument*)arg;
	while (true)
	{
		pairOfCounters->firstCounter->Lock();
		pairOfCounters->secondCounter->Lock();

		int val1 = pairOfCounters->firstCounter->GetCounterValue();
		int val2 = pairOfCounters->secondCounter->GetCounterValue();
		assert ( val1 + val2 == pairOfCounters->initialTotal);

		pairOfCounters->secondCounter->Unlock();
		pairOfCounters->firstCounter->Unlock();
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	ProtectedCounter player1(50000), player2(50000);
	ThreadArgument arg1 = {&player1, &player2, 100000};
	ThreadArgument arg2 = {&player2, &player1, 100000};

	player1.SignalEvent();

	HANDLE player1Thread = CreateThread(NULL, 0, PlayerThreadFunction_FirstPlayer, (void*)&arg1, 0, NULL);
	HANDLE player2Thread = CreateThread(NULL, 0, PlayerThreadFunction_SecondPlayer, (void*)&arg2, 0, NULL);
	HANDLE observerThread = CreateThread(NULL, 0, ObserverThreadFunction, (void*)&arg1, 0, NULL);

	WaitForSingleObject(player1Thread, INFINITE);
	WaitForSingleObject(player2Thread, INFINITE);
	WaitForSingleObject(observerThread, INFINITE);
	
	return 0;
}

