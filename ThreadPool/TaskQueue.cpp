#include "TaskQueue.h"
#include <iostream>

taskQueue* taskQueue::m_singerTask = new taskQueue;

TaskBase* taskQueue::getTaskQueue()
{
	TaskBase* ptask = nullptr;

	std::unique_lock<std::mutex> ulock(m_queueMutex);
	m_queueConVar.wait(ulock, [this] {if (m_taskQueue.empty()) return false; else return true; });
	ptask = m_taskQueue.front();
	m_taskQueue.pop();
	std::cout << "getTaskQueue" << std::endl;
	return ptask;

	/*TaskBase* ptask = nullptr;
	restart:
	if (m_taskQueue.empty())
	{
		std::unique_lock<std::mutex> ulock(m_queueMutex);
		m_queueConVar.wait(ulock);
		ptask = m_taskQueue.front();
		m_taskQueue.pop();
	}
	else
	{
		m_queueMutex.lock();
		if (!m_taskQueue.empty())
		{
			ptask = m_taskQueue.front();
			m_taskQueue.pop();
		}
		else
		{
			m_queueMutex.unlock();
			goto restart;
		}
		m_queueMutex.unlock();
	}

	return ptask;*/


}
void taskQueue::pushTaskQueue(TaskBase* pTask)
{
	m_queueMutex.lock();
	m_taskQueue.push(pTask);
	m_queueMutex.unlock();
	m_queueConVar.notify_one();
}