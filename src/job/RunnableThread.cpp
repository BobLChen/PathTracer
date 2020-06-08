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

#include "RunnableThread.h"
#include "ThreadManager.h"
#include "Runnable.h"

#include <sstream>

RunnableThread::RunnableThread()
	: m_ThreadName("")
	, m_Runnable(nullptr)
	, m_InitSyncEvent(nullptr)
	, m_ThreadID(0)
	, m_LocalThread(nullptr)
{

}

RunnableThread::~RunnableThread()
{
	ThreadManager::Get().RemoveThread(this);

	delete m_LocalThread;
}

RunnableThread* RunnableThread::Create(Runnable* runnable, const std::string& threadName)
{
	RunnableThread* thread = new RunnableThread();
	thread->CreateInternal(runnable, threadName);
	return thread;
}

bool RunnableThread::CreateInternal(Runnable* runnable, const std::string& threadName)
{
	m_Runnable		= runnable;
	m_ThreadName	= threadName;
	m_InitSyncEvent = new ThreadEvent();

	m_LocalThread   = new std::thread(RunnableThread::ThreadFunction, this);

	m_InitSyncEvent->Wait((uint32)-1);

	delete m_InitSyncEvent;
	m_InitSyncEvent = nullptr;

	return true;
}

void RunnableThread::ThreadFunction(void* pThis)
{
	RunnableThread* thisThread = (RunnableThread*)pThis;

	// ID
	{
		std::stringstream ss;
		ss << std::this_thread::get_id();
		thisThread->m_ThreadID = std::stoull(ss.str());
	}

	ThreadManager::Get().AddThread(thisThread);

	thisThread->PreRun();
	thisThread->Run();
	thisThread->PostRun();
}

void RunnableThread::PreRun()
{
	m_Runnable->runnableThread = this;
}

void RunnableThread::PostRun()
{
	m_Runnable->runnableThread = nullptr;
}

int32 RunnableThread::Run()
{
	int32 exitCode = 0;

	m_InitSyncEvent->Trigger();

	exitCode = m_Runnable->Run();

	m_Runnable->Exit();

	return exitCode;
}