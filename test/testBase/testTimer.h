#ifndef __TEST_TIMER__
#define __TEST_TIMER__

#include <memory>

#include <actor/TTask.h>
#include <actor/TStorageActors.h>
#include "actor/test/testBase/TTestPrimitive.h"

namespace NTestTask{

class TTestTaskclass:public IActor
{
    TAbstractMessagePtr _message;

private:TestPrimitive test(void){std::list<TestPrimitive> listTests; return TestPrimitive("not have test", listTests);}///убрал метод чтобы просто не мешался
public:
    TTestTaskclass():_message(nullptr) {}
    ~TTestTaskclass(){}
    void addMessage(TAbstractMessagePtr message){_message=message;}
    void execute(const unsigned int TimeOut=0){(void)TimeOut;}

    TAbstractMessagePtr getMessage(void)const {return _message;}
};


TtestResultAndComment testTask(void)
{
    const bool isRepeat=true;
    std::chrono::milliseconds timeout=std::chrono::milliseconds(100);
    IActorPtr actor(new TTestTaskclass());
    TAbstractMessagePtr message=NMessageFunction::normalMessage(Command::Test);
    TTask task(message, actor, timeout, isRepeat);
    task.sendMessage();
    if (!task.isRepeat()) return std::make_pair(false,"problem from memory repeat in task");
    if (task.getTimeout()!=std::chrono::milliseconds(100)) return std::make_pair(false,"problem from memory timeout in task");
    if (std::static_pointer_cast<TTestTaskclass >(actor)->getMessage()!=message) return  std::make_pair(false,"problem from send message on actor");
   return std::make_pair(true,"");
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief testTimer
/// \return
/// тест Таймера
TestPrimitive getTestTaskTimer(void)
{
    std::list<TestPrimitive> listTests;
    listTests.push_back(TestPrimitive("test_Task",testTask));
    return TestPrimitive("testTimer", listTests);
}

}
#endif
