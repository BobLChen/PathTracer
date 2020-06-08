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

#include "TaskThread.h"
#include "ThreadTask.h"
#include "TaskThreadPool.h"
#include "RunnableThread.h"

TaskThread::TaskThread()
	: m_DoWorkEvent(nullptr)
	, m_TimeToDie(false)
	, m_Task(nullptr)
	, m_OwningThreadPool(nullptr)
	, m_Thread(nullptr)
{
	
}

TaskThread::~TaskThread()
{

}

bool TaskThread::Create(TaskThreadPool* pool)
{
	static int32 TaskThreadIndex = 0;
	char buf[128];
	sprintf(buf, "TaskThread %d", TaskThreadIndex);
	TaskThreadIndex += 1;

	m_OwningThreadPool = pool;
	m_DoWorkEvent = new ThreadEvent();
	m_Thread = RunnableThread::Create(this, std::string(buf));

	return true;
}

bool TaskThread::KillThread()
{
	bool success = true;

	m_TimeToDie = true;
	m_DoWorkEvent->Trigger();
	m_Thread->WaitForCompletion();

	delete m_DoWorkEvent;
	delete m_Thread;

	return success;
}

void TaskThread::DoWork(ThreadTask* task)
{
	m_Task = task;
	m_DoWorkEvent->Trigger();
}

int32 TaskThread::Run()
{
	while (!m_TimeToDie)
	{
		bool continueWaiting = true;
		while (continueWaiting) {				
			continueWaiting = !(m_DoWorkEvent->Wait(5));
		}

		ThreadTask* localTask = m_Task;
		m_Task = nullptr;

		while (localTask != nullptr)
		{
			localTask->DoThreadedWork();
			localTask = m_OwningThreadPool->ReturnToPoolOrGetNextJob(this);
		} 
	}

	return 0;
}