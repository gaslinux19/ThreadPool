#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "TaskBase.h"

class ThreadBase
{
public:
	ThreadBase();
	
	void runTask();
	void stopRun();
	bool addTask(TaskBase* pTaskBase);
	virtual ~ThreadBase();
	void createWorkThread();

protected:
	TaskBase* m_pTaskBase;
	std::atomic_bool m_bFinshed;
	std::mutex m_threadMutex;
	std::condition_variable m_threadConVar;
	std::thread m_thread;

};
