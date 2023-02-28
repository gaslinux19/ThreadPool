#include "ThreadBase.h"
#include "ThreadManage.h"
#include <thread>

ThreadBase::ThreadBase() :m_bFinshed(false), m_pTaskBase(nullptr)
{
}
ThreadBase::~ThreadBase()
{
}

//设置退出线程循环标志
void ThreadBase::stopRun()
{
	std::unique_lock<std::mutex> ulock(m_threadMutex);
	m_bFinshed = true;
	m_threadConVar.notify_one();
}

//创建工作线程
void ThreadBase::createWorkThread()
{
	m_thread = std::thread(&ThreadBase::runTask, this);
}

bool ThreadBase::addTask(TaskBase* pTaskBase)
{
	std::unique_lock<std::mutex> ulock(m_threadMutex);

	if(m_pTaskBase==nullptr)
	{
		m_pTaskBase = pTaskBase;
		m_threadConVar.notify_one();
		return true;
	}

	return false;
}

//工作线程执行任务
void ThreadBase::runTask()
{
	while (!m_bFinshed)
	{
		std::unique_lock<std::mutex> ulock(m_threadMutex);
		m_threadConVar.wait(ulock, [this] {if (m_pTaskBase == nullptr)return false; else return true; });

		if (m_pTaskBase != nullptr)
		{
			m_pTaskBase->DoTask();
			m_pTaskBase->DestroyTask();

			ThreadManage* tm = ThreadManage::getInstance();
			tm->moveToIdleQueue(this);
			tm->deleteFromBusy(this);

			m_pTaskBase = nullptr;
		}
	}
	delete this;
}
