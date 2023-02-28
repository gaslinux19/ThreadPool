#pragma once
#include "TaskBase.h"
#include <queue>
#include <mutex>
#include <condition_variable>

class taskQueue
{
private:
	taskQueue() = default;
	taskQueue(taskQueue& tq) = default;
	taskQueue& operator =(taskQueue& tq) = default;

public:
	static taskQueue* getInstance()
	{
		return m_singerTask;
	}
	TaskBase* getTaskQueue();
	void pushTaskQueue(TaskBase* pTask);

private:
	static taskQueue* m_singerTask;
	std::queue<TaskBase*> m_taskQueue;
	std::mutex m_queueMutex;
	std::condition_variable m_queueConVar;
};