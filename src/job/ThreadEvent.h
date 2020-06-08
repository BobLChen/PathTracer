/**********************************************************************
Copyright (c) 2020 BobLChen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************/

#pragma once

#include <mutex>
#include <condition_variable>

#include "math/Math.h"

class ThreadEvent
{
public:
	enum TriggerType
	{
		TRIGGERED_NONE,
		TRIGGERED_ONE,
		TRIGGERED_ALL,
	};

public:

	ThreadEvent(bool isManualReset = false);

	virtual ~ThreadEvent();

	void Trigger();

	void Reset();

	bool Wait(uint32 waitTime = (uint32)-1);

	bool IsInitialized()
	{
		return m_Initialized;
	}

	bool IsManualReset()
	{
		return m_IsManualReset;
	}

	int32 WaitingThreads()
	{
		return m_WaitingThreads;
	}

	bool Wait()
	{
		return Wait((uint32)-1);
	}

private:

	inline void Lock()
	{
		m_Mutex.lock();
	}

	inline void Unlock()
	{
		m_Mutex.unlock();
	}

private:

	bool					m_Initialized;
	bool					m_IsManualReset;

	volatile TriggerType	m_Triggered;
	volatile int32			m_WaitingThreads;

	std::mutex				m_Mutex;
	std::condition_variable	m_Condition;

};