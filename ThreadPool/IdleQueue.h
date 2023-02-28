#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "ThreadBase.h"

//�����̶߳��У��ö�����ʵ��

class idleQueue
{
private:
	idleQueue() = default;
	idleQueue(idleQueue& tq) = default;
	idleQueue& operator =(idleQueue& tq) = default;

public:
	static idleQueue* getInstance()
	{
		return m_singeridle;
	}
	bool empty();
	ThreadBase* getidleQueue();
	void pushidleQueue(ThreadBase* pThread);

private:
	static idleQueue* m_singeridle;
	std::queue<ThreadBase*> m_idleQueue;
	std::mutex m_queueMutex;
	std::condition_variable m_queueConVar;
}; 
