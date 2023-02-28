#include "IdleQueue.h"


idleQueue* idleQueue::m_singeridle = new idleQueue;

ThreadBase* idleQueue::getidleQueue()
{
	ThreadBase* pidle = nullptr;

	std::unique_lock<std::mutex> ulock(m_queueMutex);
	m_queueConVar.wait(ulock, [this] {if (m_idleQueue.empty()) return false; else return true; });
	pidle = m_idleQueue.front();
	m_idleQueue.pop();

	return pidle;

}
void idleQueue::pushidleQueue(ThreadBase* pThread)
{
	m_queueMutex.lock();
	m_idleQueue.push(pThread);
	m_queueMutex.unlock();
	m_queueConVar.notify_one();
}

bool idleQueue::empty()
{
	bool bemp;
	m_queueMutex.lock();
	bemp = m_idleQueue.empty();
	m_queueMutex.unlock();

	return bemp;
}