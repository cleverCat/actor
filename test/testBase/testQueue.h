#ifndef __TEST_QUEUE__
#define __TEST_QUEUE__

#include "actor/TQueue.h"
#include "actor/test/testBase/TTestPrimitive.h"
#include <iostream>
#include <thread>





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Tkoko
{
    T _data;
public:
    Tkoko(T data):_data(data){}
    ~Tkoko(){}
    const T getData(void)const{return _data;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///тестируем очередь в 1 поточной среде
TtestResultAndComment testQueueConfusedInOneThread(void)
{


    TQueue<std::shared_ptr<Tkoko<int> > > q;

    for (int i=0;i<10;i++)
        q.push(std::shared_ptr<Tkoko<int> >(new Tkoko<int>(i)));
    for (int i=0;i<10;i++)
        if (q.pop()->getData()!=i) return std::make_pair(false,"confused element");

    return std::make_pair(true,"");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///запихиваем в очередь
void threadAdd(std::shared_ptr<TQueue<int> > queue)
{
    for (int i = 0; i < 50000; ++i)
    {
        queue->push(i);
    }
}
///вытягиваем из очереди
void threadGet(std::shared_ptr<TQueue<int> > queue, TtestResultAndComment& result)
{
    std::list<int> listGetValue;
    for (int i = 0; i < 50000; ++i)
    {
        auto j=queue->pop();
        listGetValue.push_back(j);
        if (j!=i)
        {
            //for(auto k:listGetValue) std::cout<<k<<std::endl;
            //int siz = queue->unstableSize();
            //for(auto k:queue->_pushList) std::cout<<k<<std::endl;
            //    std::cout<<siz<<"|"<<j<<"|"<<i<<"|"<<queue->pop()<<std::endl;
            //++i;
            //++i;
            result=TtestResultAndComment(false,"confused element");
            return;
        }
    }
    result=TtestResultAndComment(true,"");
}

///тест в многопоточной среде
TtestResultAndComment testQueueConfusedInThreads(void)
{
    TtestResultAndComment result;
    std::shared_ptr<TQueue<int> > q(new TQueue<int>() );
    std::thread t1(threadGet,q,std::ref(result));
    std::thread t0(threadAdd,q);

    t0.join();
    t1.join();
    return result;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief testQueue
/// \return
/// тест очереди
TestPrimitive getTestQueue(void)
{
    std::list<TestPrimitive> listTests;
    listTests.push_back(TestPrimitive("test_confused_in_one_thread",testQueueConfusedInOneThread));
    listTests.push_back(TestPrimitive("test_confused_in_threads",testQueueConfusedInThreads));
    return TestPrimitive("testQueue", listTests);
}

#endif
