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

#include "math/Math.h"
#include "ThreadEvent.h"

#include <thread>
#include <string>

class Runnable;
class ThreadManager;

class RunnableThread
{
	friend class ThreadManager;

public:

	RunnableThread();

	virtual ~RunnableThread();

	static RunnableThread* Create(Runnable* runnable, const std::string& threadName);

	virtual void WaitForCompletion()
	{
		if (m_LocalThread->joinable()) {
			m_LocalThread->join();
		}
	}

	const uint64 GetThreadID() const
	{
		return m_ThreadID;
	}

	const std::string& GetThreadName() const
	{
		return m_ThreadName;
	}

protected:

	virtual bool CreateInternal(Runnable* runnable, const std::string& threadName);

	virtual void PreRun();

	virtual int32 Run();
	
	virtual void PostRun();

	static void ThreadFunction(void* pThis);

protected:

	std::string		m_ThreadName;
	Runnable*		m_Runnable;
	ThreadEvent*	m_InitSyncEvent;
	uint64			m_ThreadID;
	std::thread*	m_LocalThread;
};