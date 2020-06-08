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

#include <vector>
#include <mutex>
#include <condition_variable>

#include "math/Math.h"

class TaskThread;
class ThreadTask;

class TaskThreadPool
{
public:

	TaskThreadPool();

	virtual ~TaskThreadPool();

	virtual bool Create(uint32 numThreads);

	virtual void Destroy();

	virtual void AddTask(ThreadTask* task);

	virtual bool RetractTask(ThreadTask* task);

	virtual ThreadTask* ReturnToPoolOrGetNextJob(TaskThread* thread);

	static TaskThreadPool* Allocate();

	int32 GetNumQueuedJobs() const
	{
		return m_QueuedTask.size();
	}

	int32 GetNumThreads() const
	{
		return m_AllThreads.size();
	}

protected:

	std::vector<ThreadTask*>		m_QueuedTask;
	std::vector<TaskThread*>		m_QueuedThreads;
	std::vector<TaskThread*>		m_AllThreads;

	std::mutex						m_SynchMutex;
	bool							m_TimeToDie = false;

};