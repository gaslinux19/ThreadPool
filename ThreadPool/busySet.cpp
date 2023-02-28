#include "busySet.h"


busySet* busySet::m_singerBusy = new busySet;

void busySet::eraseBusySet(ThreadBase* pThreadBase)
{
	std::unique_lock<std::mutex> ulock(m_setMutex);
//	m_setConVar.wait(ulock, [this] {if (m_busySet.empty()) return false; else return true; });

	std::set<ThreadBase*>::iterator it =  m_busySet.find(pThreadBase);
	if (it != m_busySet.end())
	{
		m_busySet.erase(it);
	}
	
}
void busySet::pushBusyset(ThreadBase* pThread)
{
	m_setMutex.lock();
	m_busySet.insert(pThread);
	m_setMutex.unlock();
//	m_setConVar.notify_one();
}

bool busySet::empty()
{
	bool bempty;
	m_setMutex.lock();
	bempty = m_busySet.empty();
	m_setMutex.unlock();
	
	return bempty;
}