#include "ThreadManage.h"
#include "TaskQueue.h"
#include <iostream>

ThreadManage* ThreadManage::m_pThreadManage = new ThreadManage;

//��ʼ��������initThreadNumbers�������߳�
void ThreadManage::init()
{
	for (int i = 0; i < initThreadNumbers; i++)
	{
		creatOneThread();
	}
}

//������1�������̣߳��̸߳�����1
void ThreadManage::creatOneThread()
{
	m_pThreadBase = new ThreadBase;
	if (m_pThreadBase == nullptr)
	{
		std::cout << "����ThreadBase�ڴ��쳣" << std::endl;
		exit(1);
	}

	m_pThreadBase->createWorkThread();

	moveToIdleQueue(m_pThreadBase);
	m_threadCount++;

	m_manageConVar.notify_one();

	std::cout << "������" << m_threadCount <<"�������߳�" << std::endl;
}

//���������������̶߳�����Ĺ����̣߳��Ƶ�æµ����
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

//�����߳�ѭ����ÿ��һ��ʱ��ȥ�ж��Ƿ���Ҫ����̻߳��߼����߳�
void ThreadManage::handleRun()
{
	busySet* pBusy = busySet::getInstance();
	idleQueue* pIdle = idleQueue::getInstance();
	init();

	while (!m_finished)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	//	std::cout << "�����߳�������..." << m_finished << std::endl;

		if (pBusy->empty() && m_threadCount > initThreadNumbers)
		{
			ThreadBase* pThreadBase = pIdle->getidleQueue();
			pThreadBase->stopRun();
			
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			m_threadCount--;
			std::cout << "���ٹ����߳�" << m_threadCount << std::endl;

		}
		if (pIdle->empty() && m_threadCount < maxThreadNumbers)
		{
			creatOneThread();
			std::cout << "���ӹ����߳�" << m_threadCount << std::endl;
		}
	}
	std::cout << "�˳������߳�" << std::endl;
}

//ֹͣ�����߳�
void ThreadManage::stop()
{
	m_finished = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

}

//���������̺߳ͷַ��߳�
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