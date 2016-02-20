/* Copyright(C)
* For free
* All right reserved
* 
*/
/**
* @file CXJob.cpp
* @brief 具体的任务类
* @author highway-9, 787280310@qq.com
* @version 1.1.0
* @date 2016-02-20
*/

#include "CXJob.h"
#include <iostream>

CXJob::CXJob()
    : CJob()
{
    // Do nothing
}

CXJob::~CXJob()
{
    // Do nothing
}

void CXJob::run(void* jobData)
{
    std::cout << "thread id: " << boost::this_thread::get_id() << std::endl;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
}
