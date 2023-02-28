#pragma once

#include "ThreadBase.h"
#include "busySet.h"
#include "IdleQueue.h"

const int initThreadNumbers = 4;
const int maxThreadNumbers = 64;

//管理线程，单例类，

class ThreadManage
{
private:
	ThreadManage() :m_finished(false), m_threadCount(0), m_pThreadBase(nullptr)
	{

	}
	ThreadManage(ThreadManage& tManage) = default;
	ThreadManage& operator =(ThreadManage& tManage) = default;

	void creatOneThread();
	void handleRun();
	void init();

public:
	static ThreadManage* getInstance()
	{
		return m_pThreadManage;
	}

	void stop();
	void run();
	void setTask();

	void moveToIdleQueue(ThreadBase* pThreadBase);
	void moveToBusySet(ThreadBase* pThreadBase);
	void deleteFromBusy(ThreadBase* pThreadBase);

private:
	static ThreadManage* m_pThreadManage;

	bool m_finished;
	int m_threadCount;

	std::mutex m_manageMutex;
	std::condition_variable m_manageConVar;
	std::thread m_manage;
	std::thread m_dist;

	ThreadBase* m_pThreadBase;
	
};