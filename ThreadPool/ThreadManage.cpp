#include "ThreadManage.h"
#include "TaskQueue.h"
#include <iostream>

ThreadManage* ThreadManage::m_pThreadManage = new ThreadManage;

//初始化，创建initThreadNumbers个工作线程
void ThreadManage::init()
{
	for (int i = 0; i < initThreadNumbers; i++)
	{
		creatOneThread();
	}
}

//创建给1个工作线程，线程个数加1
void ThreadManage::creatOneThread()
{
	m_pThreadBase = new ThreadBase;
	if (m_pThreadBase == nullptr)
	{
		std::cout << "分配ThreadBase内存异常" << std::endl;
		exit(1);
	}

	m_pThreadBase->createWorkThread();

	moveToIdleQueue(m_pThreadBase);
	m_threadCount++;

	m_manageConVar.notify_one();

	std::cout << "创建第" << m_threadCount <<"个工作线程" << std::endl;
}

//把任务分配给空闲线程队列里的工作线程，移到忙碌队列
void ThreadManage::setTask()
{
	taskQueue* pTaskQueue = taskQueue::getInstance();
	idleQueue* pIdle = idleQueue::getInstance();

	TaskBase* pTaskBase = nullptr;

	while (!m_finished)
	{
		pTaskBase = pTaskQueue->getTaskQueue();

		std::unique_lock<std::mutex> ulock(m_manageMutex);
		m_manageConVar.wait(ulock, [pIdle] {if (!pIdle->empty())return true; else return false; });

		ThreadBase* pThread = pIdle->getidleQueue();
		pThread->addTask(pTaskBase);

		moveToBusySet(pThread);
	}
}

//管理线程循环，每隔一段时间去判断是否需要添加线程或者减少线程
void ThreadManage::handleRun()
{
	busySet* pBusy = busySet::getInstance();
	idleQueue* pIdle = idleQueue::getInstance();
	init();

	while (!m_finished)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	//	std::cout << "管理线程运行中..." << m_finished << std::endl;

		if (pBusy->empty() && m_threadCount > initThreadNumbers)
		{
			ThreadBase* pThreadBase = pIdle->getidleQueue();
			pThreadBase->stopRun();
			
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			m_threadCount--;
			std::cout << "减少工作线程" << m_threadCount << std::endl;

		}
		if (pIdle->empty() && m_threadCount < maxThreadNumbers)
		{
			creatOneThread();
			std::cout << "增加工作线程" << m_threadCount << std::endl;
		}
	}
	std::cout << "退出管理线程" << std::endl;
}

//停止管理线程
void ThreadManage::stop()
{
	m_finished = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

}

//启动管理线程和分发线程
void ThreadManage::run()
{
	m_manage = std::thread(&ThreadManage::handleRun, this);
	m_dist = std::thread(&ThreadManage::setTask, this);
}


void ThreadManage::moveToIdleQueue(ThreadBase* pThreadBase)
{
	idleQueue* pIdle = idleQueue::getInstance();
	pIdle->pushidleQueue(pThreadBase);
	m_manageConVar.notify_one();
}

void ThreadManage::moveToBusySet(ThreadBase* pThreadBase)
{
	busySet* pBusy = busySet::getInstance();
	pBusy->pushBusyset(pThreadBase);
}

void ThreadManage::deleteFromBusy(ThreadBase* pThreadBase)
{
	busySet* pBusy = busySet::getInstance();
	pBusy->eraseBusySet(pThreadBase);
}