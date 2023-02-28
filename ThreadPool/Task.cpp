#include <iostream>
#include "Task.h"



int Task::DoTask()
{
	long long int sum = 0;
//	std::cout << "task runing 。。。。" << i << std::endl;
	for (int i = 0; i < m_count; i++)
	{
		sum += i;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	std::cout << "end task. sun = " << sum << std::endl;

	return 1;
}

void Task::DestroyTask()
{
	delete this;
//	std::cout << "DestroyTask "  << std::endl;
}