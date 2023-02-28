#pragma once

class TaskBase
{
public:
	virtual int DoTask() = 0;
	virtual void DestroyTask() = 0;
	virtual ~TaskBase()
	{

	}
};

