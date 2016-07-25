#include "Storage.h"
#include "iostream"
#include <functional>

Storage::Storage():AbstractActor(),Runnable()
{
    //enum StorageMessge{EXECUTE_ACTOR,ADD_TIMER, REMOVE_TIMER};
    //std::function<void(AbstractActorPtr)>(this->executeActor);
  //auto f = [this] (AbstractActorPtr actor) {this->executeActor(actor);};
     this->addExecuterOnMessage(StorageMessge::REMOVE_TIMER, std::function<void(unsigned long)>([this] (unsigned long task) {this->removeTask(task);}));
     this->addExecuterOnMessage(StorageMessge::ADD_TIMER, std::function<void(std::shared_ptr<Task>)>([this] (std::shared_ptr<Task> task) {this->addTimer(task);}));
     this->addExecuterOnMessage(StorageMessge::EXECUTE_ACTOR, std::function<void(AbstractActorPtr)>([this] (AbstractActorPtr actor) {this->executeActor(actor);}));
}

void Storage::execute()
{
    auto message = _queueMessage.pop();
    if (_executers.find(message->getID())==_executers.end()){throw NotGetExecutersInActor();};
    _executers.find(message->getID())->second->execute(message);
    //std::cout<<"Storage::execute";
}

void Storage::addTimer(std::shared_ptr<Task> task)
{
    auto timer =std::chrono::system_clock::now()+std::chrono::milliseconds(task->getTimeout());
    if (_timeAndTask.find( (unsigned long)(&(*task)))==_timeAndTask.end())
        _timeAndTask.insert(std::make_pair( (unsigned long)(&(*task)) ,std::make_pair(timer,task)));
}

void Storage::removeTask(unsigned long task)
{
    if (_timeAndTask.find(task)!=_timeAndTask.end())
        _timeAndTask.erase(task);
}

void Storage::executeActor(AbstractActorPtr actor)
{
  //  std::cout<<"actor->execute"<<std::endl;
    actor->execute();
}

void Storage::addMessage(AbstractMessagePtr message)
{
    _queueMessage.push_back(message);
    if (_stop) this->start();
}

void Storage::run()
{
    auto timer =std::chrono::system_clock::now()+std::chrono::milliseconds(1000); //раз в секунду при отсутствии заданий таймер сам тикнет не думаю что это плохо
    while(!_stop)
    {
        try
        {
            std::unique_lock<std::mutex> lk(_lockTimer);
//            std::cout<<"if"<<std::endl;
            if(cv.wait_until(lk, timer, [this](){return _queueMessage.unstableSize()>0;})) //часты ложные срабатывания
            {
//                std::cout<<"in if"<<std::endl;
                ///*flag*/ вытащить из очереди сообщений и исполнить сообщение самого таймера
                if (_queueMessage.unstableSize()>0)
                {
//                    std::cout<<"in if 2"<<std::endl;
                    this->execute();
                }
            }
            /*timeout*/
            for (auto& i:_timeAndTask)
            {
                auto now = std::chrono::system_clock::now();
                if (i.second.first<=now)
                {
                    i.second.second->sendMessage();
                    i.second.first=now+std::chrono::milliseconds(i.second.second->getTimeout());
                    if (!i.second.second->isRepeat()) _timeAndTask.erase(i.first);


                }
                if (timer>i.second.first) timer=i.second.first;
            }
            if (_timeAndTask.size()==0)		timer=std::chrono::system_clock::now()+std::chrono::milliseconds(1000); //раз в секунду при отсутствии заданий таймер сам тикнет не думаю что это плохо
        }
        catch (std::exception& e)
        {
            std::cout<<e.what();
            assert(false);
        }
        catch(...){/*проблемы в хранилище акторов*/ std::cout<<"problem from storage execute";}
    }
}
