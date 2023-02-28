#include <iostream>
#include "ThreadBase.h"
#include "ThreadManage.h"
#include "Task.h"
#include "TaskQueue.h"

int main()
{
 
    ThreadManage* tm = ThreadManage::getInstance();
    tm->run();
  //  std::thread tmanage(&ThreadManage::run,tm);

    taskQueue* pTaskQueue = taskQueue::getInstance();

    for (int i = 0; i < 1000; i++)
    {
     //   std::this_thread::sleep_for(std::chrono::milliseconds(1));
        TaskBase* pTaskBase = new Task(i);
        pTaskQueue->pushTaskQueue(pTaskBase);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    tm->stop();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

}

