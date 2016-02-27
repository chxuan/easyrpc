/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年02月17日 星期三 22时01分17秒
 ************************************************************************/

#include <iostream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "CThreadManage.h"
#include "CRealJob.h"

void doTask(void* jobData)
{
    std::cout << "Hello world" << std::endl;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(50));
}

int main()
{
    CThreadManagePtr manage(new CThreadManage);
    manage->initThreadNum(10);

    for (int i = 0; i < 100; ++i)
    {
        CRealJobPtr job(new CRealJob(boost::bind(doTask, _1), NULL));
        manage->run(job);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(5000));
    std::cout << "##############END###################" << std::endl;
    return 0;
}
