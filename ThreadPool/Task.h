#pragma once
#include "TaskBase.h"
#include "ThreadBase.h"

class Task :public TaskBase
{
public:
	Task(int con) :m_count(con)
	{

	}

public:
	virtual int DoTask();
	virtual void DestroyTask();
	virtual ~Task()
	{

	}

private:
	int m_count;
};