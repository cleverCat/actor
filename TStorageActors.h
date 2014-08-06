#ifndef TSTORAGEACTORS_H
#define TSTORAGEACTORS_H
#include <map>
#include <chrono>
#include <thread>
#include <atomic>
#include "actor/IActor.h"
#include "actor/Messages.h"

#include <mutex>
#include <utility>
#include <condition_variable>
#include "actor/TTask.h"

#include <boost/any.hpp>

#define HeaderForInlude "QDebug"
#include "actor/baseLib/importWithoutWarning.h"

///   !!!создавать обьект только по умной ссылке
class TStorageActors:public IActor
{
private:
	std::atomic<bool> isRun;
    std::thread  _thread;
 private: ///timer
    std::atomic<bool> _isUpdateTimer;
    std::mutex _lockTimer;
    std::map<unsigned long, std::pair<std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long long int, std::ratio<1ll, 1000000000ll> > >, std::shared_ptr<TTask> > >  _timeAndTask;
    std::condition_variable cv;

public:
    explicit TStorageActors();
    ~TStorageActors();
public:

    //добавляем сообщение в список сообщений на исполнение
    void addMessage(TAbstractMessagePtr message);
//выполняем сообщения  посланные нам
    void execute(const uint TimeOut=0);

    void initThread(); //иницилизируем поток ))
    //выполняем таймер и сообщения
    void executeTask();
   static void run(TStorageActors & threadActor);//тело потока
private:
public :
///добавляем задачу для таймера
   unsigned long addTask(std::shared_ptr<TTask> task);
///убираем задачу у таймера
   void killTask(unsigned long task);

   TestPrimitive test(void)override;

};

typedef std::shared_ptr<TStorageActors> TStorageActorsPtr;

namespace NStrongActorsFunctionOnMessage
{

/// запускаем поток обработки актора
    NOT_USED static void runStorageThread(TAbstractMessagePtr message, IActorPtr currentActor)
    {
        (void)message;
        castActor<TStorageActors>(currentActor)->initThread();
    }
    ///добавляем в таймер задачу ))
    NOT_USED static void addTask(TAbstractMessagePtr message, IActorPtr currentActor)
    {
        castActor<TStorageActors>(currentActor)->addTask(boost::any_cast< std::shared_ptr<TTask> >(message->getData()));
    }
    NOT_USED static void executeActor(TAbstractMessagePtr message, IActorPtr currentActor)
    {
        (void)currentActor;
        try{

            boost::any_cast< IActorPtr >(message->getData())->execute(0); //заставляем актор который нам передали выполнить свою работу )) лол

        }catch(std::exception& ex)
        {
            std::string nameType(typeid(boost::any_cast< IActorPtr >(message->getData())).name());
            qDebug()<<"error from"+QString::fromStdString(nameType);
			throw(ex);
        }
    }
}


#endif // TSTORAGEACTORS_H
