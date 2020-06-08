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

#include "ThreadEvent.h"

ThreadEvent::ThreadEvent(bool isManualReset)
	: m_Initialized(true)
	, m_IsManualReset(isManualReset)
	, m_Triggered(TRIGGERED_NONE)
	, m_WaitingThreads(0)
{

}

ThreadEvent::~ThreadEvent()
{
	if (m_Initialized)
	{
		Lock();
		m_IsManualReset = true;
		Unlock();

		Trigger();

		Lock();
		m_Initialized = false;
		while (m_WaitingThreads > 0)
		{
			Unlock();
			Lock();
		}
		Unlock();
	}
}

void ThreadEvent::Trigger()
{
	Lock();

	if (m_IsManualReset)
	{
		m_Triggered = TRIGGERED_ALL;
		m_Condition.notify_all();
	}
	else
	{
		m_Triggered = TRIGGERED_ONE;
		m_Condition.notify_one();
	}

	Unlock();
}

void ThreadEvent::Reset()
{
	Lock();
	m_Triggered = TRIGGERED_NONE;
	Unlock();
}

bool ThreadEvent::Wait(uint32 waitTime)
{
	std::unique_lock<std::mutex> uniqueLock(m_Mutex);

	bool needWaiting = true;

	do
	{
		if (m_Triggered == TRIGGERED_ONE)
		{
			m_Triggered = TRIGGERED_NONE;
			needWaiting = false;
		}
		else if (m_Triggered == TRIGGERED_ALL)
		{
			needWaiting = false;
		}
		else if (waitTime == (uint32)-1)
		{
			m_WaitingThreads += 1;
			m_Condition.wait(uniqueLock);
			m_WaitingThreads -= 1;
		}
		else
		{
			m_WaitingThreads += 1;
			m_Condition.wait_for(uniqueLock, std::chrono::microseconds(waitTime));
			m_WaitingThreads -= 1;
		}
	} 
	while (needWaiting);

	return !needWaiting;
}