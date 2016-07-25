#ifndef STORAGE_H
#define STORAGE_H
#include "iostream"

#include "AbstractActor.h"
#include "Message.h"
#include "storageHelpers/Task.h"
#include "storageHelpers/Runnable.h"

enum StorageMessge{EXECUTE_ACTOR, ADD_TIMER, REMOVE_TIMER};

class Storage:public AbstractActor, public Runnable
{
private:
    std::mutex _lockTimer;
    std::map<unsigned long, std::pair<std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long long int, std::ratio<1ll, 1000000000ll> > >, std::shared_ptr<Task> > >  _timeAndTask;
    std::condition_variable cv;

public:
    Storage();
    void execute(void);
    ///добавляет таймер
    void addTimer(std::shared_ptr<Task> task);

    ///убираем таймер
    void removeTask(unsigned long task);

    ///выполняем актор
    void executeActor(AbstractActorPtr actor);

    void addMessage(AbstractMessagePtr message);

    void run(void);
};

#endif // STORAGE_H
