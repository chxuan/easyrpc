/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年02月17日 星期三 22时01分17秒
 ************************************************************************/

#include <iostream>
#include "CThreadManage.h"
#include "CXJob.h"

int main()
{
    CThreadManagePtr manage(new CThreadManage);
    manage->initThreadNum(2);

    for (int i = 0; i < 3; ++i)
    {
        CXJobPtr job(new CXJob);
        manage->run(job, NULL);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(10000));

    for (int i = 0; i < 3; ++i)
    {
        CXJobPtr job(new CXJob);
        manage->run(job, NULL);
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(10000));
    manage->terminateAll();

    return 0;
}
