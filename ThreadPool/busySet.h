#pragma once
#include <set>
#include <mutex>
#include <condition_variable>
#include "ThreadBase.h"

//忙碌线程队列，因为需要查找，用SET实现更好一点

class busySet
{
private:
	busySet() = default;
	busySet(busySet& tq) = default;
	busySet& operator =(busySet& tq) = default;

public:
	static busySet* getInstance()
	{
		return m_singerBusy;
	}

	bool empty();
	void eraseBusySet(ThreadBase* pbusy);
	void pushBusyset(ThreadBase* pThread);

private:
	static busySet* m_singerBusy;
	std::set<ThreadBase*> m_busySet;
	std::mutex m_setMutex;
//	std::condition_variable m_setConVar;
};