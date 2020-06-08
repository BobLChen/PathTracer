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

#include "ThreadManager.h"
#include "RunnableThread.h"

void ThreadManager::AddThread(RunnableThread* thread)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	
	auto it = m_Threads.find(thread->GetThreadID());
	if (it == m_Threads.end()) {
		m_Threads.insert(std::make_pair(thread->GetThreadID(), thread));
	}
}

void ThreadManager::RemoveThread(RunnableThread* thread)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	auto it = m_Threads.find(thread->GetThreadID());
	if (it != m_Threads.end()) {
		m_Threads.erase(thread->GetThreadID());
	}
}

const std::string& ThreadManager::GetThreadName(uint64 threadID)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	return m_Threads[threadID]->GetThreadName();
}

ThreadManager& ThreadManager::Get()
{
	static ThreadManager manager;
	return manager;
}