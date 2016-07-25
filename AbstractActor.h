#ifndef ABSTRACTACTOR_H
#define ABSTRACTACTOR_H

#include <memory>
#include <map>
#include <functional>

#include "Executer.h"
#include "Message.h"
#include "TQueue.h"


class NotGetExecutersInActor: public std::exception {};

class AbstractActor
{
protected:
    std::map<int, std::shared_ptr<AbstractExecuter> > _executers;//словарь обработчиков сообщений
    TQueue<AbstractMessagePtr> _queueMessage;
public:
    AbstractActor();
    //выполням переданное сообщение
    virtual void execute(void)=0;

    //получаем сообщение
    virtual void addMessage(AbstractMessagePtr message)=0;

    //добовляем обработчик сообщения
    template<typename... Types>
    void addExecuterOnMessage(const int MessageID, std::function<void(Types...)> functor)
    {
        std::shared_ptr<AbstractExecuter> executer = std::make_shared<Executer<Types...> >(Executer<Types...>(functor));
        _executers[MessageID]=executer;
    }

};

typedef std::shared_ptr<AbstractActor> AbstractActorPtr;

template<typename T, typename... Types>
std::shared_ptr<T> getActor(Types ... args){return std::shared_ptr<T>(&args ...);}

#endif // ABSTRACTACTOR_H
