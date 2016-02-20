/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年02月17日 星期三 22时01分17秒
 ************************************************************************/

#include <iostream>
#include "CWorkerThread.h"

bool IsReady = false;
boost::condition_variable_any TestCond;
boost::mutex TestMutex;

void test()
{
    //boost::unique_lock<boost::mutex> locker(TestMutex);
//    while (!IsReady)
//    {
//        TestCond.wait(locker);
//    }
    boost::unique_lock<boost::mutex> locker(TestMutex);
    std::cout << "test###########3" << std::endl;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(2000));
}

class A
{
public:
    A()
    {

    }
    ~A()
    {
        std::cout << "##############################" << std::endl;
    }
};

int main()
{
    typedef boost::shared_ptr<A> APtr;
    APtr pA(new A);
    //pA = NULL;
    std::cout << "123" << std::endl;
    return 0;

    boost::thread t(test);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(500));


    {
        boost::unique_lock<boost::mutex> locker(TestMutex);
        std::cout << "!!!!!!!!!!!!!!!!!!" << std::endl;
        //IsReady = true;
        //TestCond.notify_one();
    }


    t.join();

    std::cout << "Hello world" << std::endl;
    std::cout << "main thread id: " << boost::this_thread::get_id() << std::endl;

    CWorkerThreadPtr workThread(new CWorkerThread);
    workThread->start();

    if (workThread->joinable())
    {
        workThread->join();
    }

    return 0;
}
